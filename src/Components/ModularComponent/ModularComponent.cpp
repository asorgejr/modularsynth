//
// Created by asorgejr on 4/17/2020.
//
#include <memory>
#include <utility>
#include <type_traits>
#include <algorithm>
#include <sstream>
#include "../../app/config.h"
#include "ModularComponent.h"
#include "../../NodeSystem/NodeInfo.h"
#include "../../NodeSystem/Nodes/nodes.h"

DISABLE_WARNING_PUSH
DISABLE_WARNING_LOSS_OF_DATA

namespace modularsynth {
using namespace std;
using namespace nodesystem;

typedef ModularComponent::ModularAudioProcessor MProcessor;
typedef ModularComponent::ModularNodeGraphView MGraphView;

#pragma region ModularComponent

ModularComponent::ModularComponent(sptr<AudioDeviceManager> audioDeviceManager, sptr<MouseListener> sharedMouseListener) 
: Component(),
  IModularComponent(audioDeviceManager, sharedMouseListener),
  modularAudioProcessor(new ModularAudioProcessor(audioDeviceManager)),
  view(make_shared<ModularNodeGraphView>(modularAudioProcessor)),
  keyboardComponent(make_shared<MidiKeyboardComponent>(keyboardState, MidiKeyboardComponent::horizontalKeyboard))
{
  view->setSize(800, 400);
  view->theme = config::Palette::GraphViewTheme;
  toolbar = make_shared<Toolbar>();
  keyboardComponent->setSize(800, 100);
}

ModularComponent::~ModularComponent() = default;

void ModularComponent::paint(Graphics &g) {
  g.fillAll(getUIColourIfAvailable(LookAndFeel_V4::ColourScheme::UIColour::windowBackground));
}

void ModularComponent::resized() {
  auto r = getLocalBounds();
  auto graphViewer = view.get();
  addAndMakeVisible(graphViewer);
  auto keyboard = keyboardComponent.get();
  addAndMakeVisible(keyboard);
  layout();
}

void ModularComponent::layout() {
    auto graphViewer = view.get();

    FlexBox flexBox;
    flexBox.flexDirection = FlexBox::Direction::column;
    flexBox.justifyContent = FlexBox::JustifyContent::flexStart;
    flexBox.alignItems = FlexBox::AlignItems::flexStart;
    flexBox.alignContent = FlexBox::AlignContent::stretch;

    FlexItem tbi(*toolbar);
    tbi.alignSelf = FlexItem::AlignSelf::stretch;
    tbi.minHeight = 40.0f;

    FlexItem gvi(*graphViewer);
    gvi.alignSelf = FlexItem::AlignSelf::stretch;
    gvi.minHeight = getHeight();
    gvi.maxHeight = 500;
    
    FlexItem keys(*keyboardComponent);
    keys.alignSelf = FlexItem::AlignSelf::stretch;
    keys.minHeight = 64;
    
//    flexBox.items.add(tbi);
    flexBox.items.add(gvi);
    flexBox.items.add(keys);

    flexBox.performLayout(getBounds());
}

void ModularComponent::childBoundsChanged(Component *child) {
  layout();
}

#pragma endregion ModularComponent



#pragma region ModularAudioProcessor

MProcessor::ModularAudioProcessor(const sptr<AudioDeviceManager> &audioDeviceManager)
: AudioProcessor(BusesProperties()
  .withInput("Input", AudioChannelSet::stereo(), true)
  .withOutput("Output", AudioChannelSet::stereo(), true)),
  audioDeviceManager(audioDeviceManager),
  mainProcessor(make_shared<AudioProcessorGraph>()) 
{
  auto inputDevice  = MidiInput::getDevices()  [MidiInput::getDefaultDeviceIndex()];
  auto outputDevice = MidiOutput::getDevices() [MidiOutput::getDefaultDeviceIndex()];
  mainProcessor->enableAllBuses();
  this->audioDeviceManager->initialiseWithDefaultDevices(2, 2);
  this->audioDeviceManager->addAudioCallback (&player);                  // [2]
  this->audioDeviceManager->setMidiInputEnabled (inputDevice, true);
  this->audioDeviceManager->addMidiInputCallback (inputDevice, &player); // [3]
  this->audioDeviceManager->setDefaultMidiOutput (outputDevice);
}

#pragma region Audio IO Graph

void MProcessor::initialiseGraph() {
  mainProcessor->clear();

  masterAudioIn  = mainProcessor->addNode(
    static_cast<uptr<AudioProcessor>>(make_unique<AudioGraphIOProcessor>(AudioGraphIOProcessor::audioInputNode)));
  masterAudioOut = mainProcessor->addNode(
    static_cast<uptr<AudioProcessor>>(make_unique<AudioGraphIOProcessor>(AudioGraphIOProcessor::audioOutputNode)));
  masterMidiIn   = mainProcessor->addNode(
    static_cast<uptr<AudioProcessor>>(make_unique<AudioGraphIOProcessor>(AudioGraphIOProcessor::midiInputNode)));
  masterMidiOut  = mainProcessor->addNode(
    static_cast<uptr<AudioProcessor>>(make_unique<AudioGraphIOProcessor>(AudioGraphIOProcessor::midiInputNode)));

  connectAudioNodes();
  connectMidiNodes();
}

void MProcessor::connectAudioNodes() {
  for (int channel = 0; channel < kChannelCount; ++channel)
    mainProcessor->addConnection ({
      { masterAudioIn->nodeID,  channel },
      { masterAudioOut->nodeID, channel }
    });
}

void MProcessor::connectMidiNodes() {
  mainProcessor->addConnection ({
    { masterMidiIn->nodeID,  AudioProcessorGraph::midiChannelIndex },
    { masterMidiOut->nodeID, AudioProcessorGraph::midiChannelIndex }
  });
}

#pragma endregion Audio IO Graph

#pragma region AudioProcessor Implementations

bool MProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const {
  if (layouts.getMainInputChannelSet()  == AudioChannelSet::disabled()
      || layouts.getMainOutputChannelSet() == AudioChannelSet::disabled())
    return false;

  if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
      && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
    return false;

  return layouts.getMainInputChannelSet() == layouts.getMainOutputChannelSet();
}

const String MProcessor::getName() const {
  return String();
}

void MProcessor::prepareToPlay(double sampleRate, int maximumExpectedSamplesPerBlock) {
  auto audioDevice = audioDeviceManager->getCurrentAudioDevice();
  auto spb = audioDevice->getCurrentBufferSizeSamples();
  mainProcessor->setPlayConfigDetails (getMainBusNumInputChannels(),
                                       getMainBusNumOutputChannels(),
                                       sampleRate, config::samplesPerBlock);

  mainProcessor->prepareToPlay (sampleRate, config::samplesPerBlock);

//  initialiseGraph();
}

void MProcessor::releaseResources() {
  mainProcessor->releaseResources();
}

void MProcessor::processBlock(AudioBuffer<float> &buffer, MidiBuffer &midiMessages) {
  for (int i = getTotalNumInputChannels(); i < getTotalNumOutputChannels(); ++i)
    buffer.clear (i, 0, buffer.getNumSamples());

  // updateGraph();

  mainProcessor->processBlock (buffer, midiMessages);
}

double MProcessor::getTailLengthSeconds() const {
  return 0;
}

bool MProcessor::acceptsMidi() const {
  return true;
}

bool MProcessor::producesMidi() const {
  return false;
}

AudioProcessorEditor *MProcessor::createEditor() {
  return nullptr;
}

bool MProcessor::hasEditor() const {
  return false;
}

int MProcessor::getNumPrograms() {
  return 0;
}

int MProcessor::getCurrentProgram() {
  return 0;
}

void MProcessor::setCurrentProgram(int index) {

}

const String MProcessor::getProgramName(int index) {
  return String();
}

void MProcessor::changeProgramName(int index, const String &newName) {

}

void MProcessor::getStateInformation(MemoryBlock &destData) {

}

void MProcessor::setStateInformation(const void *data, int sizeInBytes) {

}

#pragma endregion AudioProcessor Implementations

#pragma endregion ModularAudioProcessor


#pragma region ModularNodeGraphView

MGraphView::ModularNodeGraphView(const sptr<ModularAudioProcessor> &modularAudio)
: GraphViewComponent(),
  modularAudio(modularAudio) 
{
  this->modularAudio->initialiseGraph();
}

void MGraphView::popupMenu(const MouseEvent &e) {
  PopupMenu m;
  auto nodeDefinitions = NodeInfo::getNodeDefinitions();
  auto position = e.getMouseDownPosition().toFloat();
  for (auto i = 0; i < nodeDefinitions.size(); i++) {
    auto mI = i+1;
    m.addItem(mI, nodeDefinitions[i].name);
  }
  auto selection = m.show();
  try {
    auto sI = selection >= 1 ? selection - 1 : 0;
    auto ShareSelfCopy = shared_from_this();
    nodeDefinitions[sI].instantiate(ShareSelfCopy, position);
  } catch (exception &e) {
    cerr << "A shared_ptr could not be created."
         << endl << e.what() << endl;
  }
  
}

void MGraphView::onAfterAddHostNode(const HostNodeComponent *node, const Graph::Node *model) {
  debugHeader("EVENT: ADD HOST NODE FINISHED");
  auto tempF = make_unique<AudioNodeGenerator>();
  if (NodeInfo::tryGetAudioNodeGenerator(model->name, tempF.get())) {
    auto gen = *tempF;
    auto anPtr = gen(modularAudio.get());
    audioNodeTether[node] = anPtr;
    cout << "new AudioNode: " << anPtr << endl;
  } else {
    cerr << "could not add AudioNode. Could not find '" << model->name << "'." << endl;
  }
}

void MGraphView::onBeforeRemoveNode(const NodeComponent *n) {
  debugHeader("EVENT: REMOVE NODE");
  audioNodeTether.erase(n);
  debugGraph();
}

void MGraphView::onBeforeRemoveWire(const WireComponent *w) {
  debugHeader("EVENT: REMOVE WIRE");
  auto n0 = w->startPin->node;
  auto n1 = w->endPin->node;
  auto an0 = audioNodeTether[n0];
  auto an1 = audioNodeTether[n1];
  if (an0 == nullptr || an1 == nullptr) {
    cerr << "ModularNodeGraphView error: AudioNode not initialized." << endl
         << "source node: " << an0 << endl << "target node: " << an1 << endl;
    return;
  }
  for (int i = 0; i < modularAudio->kChannelCount; i++) {
    modularAudio->mainProcessor->removeConnection({ // TODO: random nullptr on Master in/out nodes.
      {an0->nodeID, i}, {an1->nodeID, i}
    });
  }
}

void MGraphView::onAfterRemoveNode() {
  debugHeader("EVENT: REMOVE NODE FINISHED");
  debugGraph();
}

void MGraphView::onAfterAddWire(const NodeComponent::PinComponent *source, const NodeComponent::PinComponent *target) {
  debugHeader("EVENT: REMOVE WIRE FINISHED");
  debugGraph();
  auto n0 = source->node;
  auto n1 = target->node;
  auto an0 = audioNodeTether[n0];
  auto an1 = audioNodeTether[n1];
  if (an0 == nullptr || an1 == nullptr) {
    cerr << "ModularNodeGraphView error: AudioNode not initialized." << endl
    << "source node: " << an0 << endl << "target node: " << an1 << endl;
    return;
  }
  for (int i = 0; i < modularAudio->kChannelCount; i++) {
    modularAudio->mainProcessor->addConnection({
      {an0->nodeID, i}, {an1->nodeID, i}
    });
  }
}

void MGraphView::onAfterRemoveWire() {
  debugHeader("EVENT: REMOVE WIRE FINISHED");
}

void MGraphView::debugGraph() {
  #ifdef NODESYSTEM_DEBUG
  cout << "Graph Debug:" << endl;
  
  cout << "Nodes: " << endl;
  
  try {
    string indent = indentN(_dbgIndent++);
    
    for (int i = 0; i < graph->nodes.size(); i++) {
      if (graph->nodes[i] == nullptr) throw exception("bad node ptr");
      debugNode(*(graph->nodes[i]));

      cout << "Node neighbors: " << endl;
      indent = indentN(_dbgIndent++);

      vector<const Graph::Node *> neighbors;
      graph->targets(graph->nodes[i].get(), neighbors);

      for_each(neighbors.begin(), neighbors.end(), [&](auto &nn) -> void {
        if (nn != nullptr) {
          cout << indent + "name: '" << nn->name << "' | '" << nn << "'" << endl;
        }
      });
      indent = indentN(_dbgIndent--);
    }
    
    _dbgIndent--;
    
  } catch (exception &e) {
    cout << "debugGraph error: " << e.what() << endl;
  }
  
  #endif // NODESYSTEM_DEBUG
}

void MGraphView::debugNodeComponent(const NodeComponent &n, bool recurseUpstream) {
#ifdef NODESYSTEM_DEBUG
  
  cout << "NodeComponent debug: " << endl;
  cout << "name (Component): " << n.getName() << endl;
  cout << "selected: " << (n.selected ? "true" : "false") << endl;
  cout << "node model: " << endl;
  cout << "is valid: ";
  try {
    auto m = n.model;
    if (m == nullptr) throw exception();
    cout << "true" << endl;
    cout << "node model: " << endl;
    _dbgIndent++;
    debugNode(*m);
    _dbgIndent--;
  } catch (exception &e) {
    cout << "false" << endl;
  } catch (...) {
    cout << "an unhandled exception occurred." << endl;
  }
  
#endif // NODESYSTEM_DEBUG
}

void MGraphView::debugNode(const Graph::Node &n, bool recurseUpstream) {
#ifdef NODESYSTEM_DEBUG
  auto indent = indentN(_dbgIndent);
  cout << indent + "node name: " << n.name << endl;
  indent = indentN(_dbgIndent++);
  cout << indent + "total in pins: " << n.ins.size() << endl;
  std::for_each(n.ins.begin(), n.ins.end(), [&](auto &p) {
    if (p == nullptr) throw exception();
    if (p->node != nullptr) {
      _dbgIndent++;
      debugPin(*(p.get()), recurseUpstream);
      _dbgIndent--;
    }
  });
  cout << indent + "total out pins: " << n.outs.size() << endl;
  std::for_each(n.outs.begin(), n.outs.end(), [&](auto &p){
    if (p->node != nullptr) {
      _dbgIndent++;
      debugPin(*(p.get()), recurseUpstream);
      _dbgIndent--;
    }
  });
  cout << indent + "node listeners: [" << endl;
  for (auto &listener : n.listeners) {
    indent = indentN(_dbgIndent + 1);
    cout << indent << listener << "," << endl;
  }
  indent = indentN(--_dbgIndent);
  cout << indent + "] " << endl;
#endif // NODESYSTEM_DEBUG
}

void MGraphView::debugNodeConnectors(const Graph::Node &n, bool recurseUpstream) {

}

void MGraphView::debugPin(const Graph::Pin &p, bool recurseUpstream) {
  auto indent = indentN(_dbgIndent);
  cout << indent + "pin " << p.order << ": " << endl;
  
  auto pinType = p.pinType == Graph::PinType::In ? "PinType::In" : "PinType::Out";
  indent = indentN(_dbgIndent + 1);
  
  cout << indent + "pinType: " << pinType << endl;
  if (p.node == nullptr) {
    cout << "error accessing node at pin: '" << &p << "'." << endl;
  }
  cout << indent + "node: " << p.node << endl;
}

void MGraphView::debugHeader(const string &title) {
  const int width = 60;
  int margin = width - title.size() != 0 ? (width - title.size()) / 2 : 0;
  string message;
  for (int i = 0; i < width; i++) {
    message += "=";
  }
  message += "\n";
  for (int i = 0; i < margin; i++) {
    message += " ";
  }
  message += title;
  for (int i = 0; i < margin; i++) {
    message += " ";
  }
  message += "\n";
  for (int i = 0; i < width; i++) {
    message += "=";
  }
  message += "\n";
  cout << message;
}

string MGraphView::indentN(int x) {
  string indent;
  for (int i = 0; i < x; i++) indent += "\t";
  return indent;
}



#pragma endregion ModularNodeGraphView

}

DISABLE_WARNING_POP
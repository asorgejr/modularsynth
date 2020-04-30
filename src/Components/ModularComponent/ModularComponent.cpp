//
// Created by asorgejr on 4/17/2020.
//
#include <memory>
#include <utility>
#include "../../config.h"
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
  view(make_shared<ModularNodeGraphView>(modularAudioProcessor->mainProcessor))
{
  view->setSize(800, 400);
  view->theme = config::Palette::GraphViewTheme;
  toolbar = make_shared<Toolbar>();
}

ModularComponent::~ModularComponent() = default;

void ModularComponent::paint(Graphics &g) {
  g.fillAll(getUIColourIfAvailable(LookAndFeel_V4::ColourScheme::UIColour::windowBackground));
}

void ModularComponent::resized() {
  auto r = getLocalBounds();
  auto graphViewer = view.get();
  addAndMakeVisible(graphViewer);
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
    
    //flexBox.items.add(tbi);
    flexBox.items.add(gvi);

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
  mainProcessor(make_shared<AudioProcessorGraph>()) {}

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
  for (int channel = 0; channel < 2; ++channel)
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

  // initialiseGraph();
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
  return false;
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

MGraphView::ModularNodeGraphView(const sptr<AudioProcessorGraph> &audioGraph)
: GraphViewComponent(),
  audioGraph(audioGraph) {}

void MGraphView::popupMenu(const MouseEvent &e) {
  PopupMenu m;
  auto position = e.getMouseDownPosition().toFloat();
  for (auto i = 0; i < NodeInfo::NodeDefinitions.size(); i++) {
    auto mI = i+1;
    m.addItem(mI, NodeInfo::NodeDefinitions[i].name);
  }
  auto selection = m.show();
  auto sliderPanel = [&]() {
    addHostNode(std::make_unique<SliderPanel>(), 1, 1, 150, 150, position);
  };
  auto textPanel = [&]() {
    addHostNode(std::make_unique<TextPanel>(), 1, 0, 200, 100, position);
  };
  auto embeddedGraphView = [&]() {
    addHostNode(std::make_unique<EmbeddedGraphView>(), 1, 1, 400, 400, position);
  };
  try {
    auto sI = selection >= 1 ? selection - 1 : 0;
    auto ShareSelfCopy = shared_from_this();
    NodeInfo::NodeDefinitions[sI].instantiate(ShareSelfCopy, position);
  } catch (exception &e) {
    cerr << "A shared_ptr could not be created."
         << endl << e.what() << endl;
  }

  #pragma endregion ModularNodeGraphView
  
}

}

DISABLE_WARNING_POP
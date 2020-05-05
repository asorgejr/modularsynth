//
// Created by asorgejr on 4/17/2020.
//

#pragma once
#include <JuceHeader.h>
#include <nodesystem/nodesystem.h>

#include "../../Models/IModularComponent.h"
#include "../../common.h"

using namespace nodesystem;

namespace modularsynth {

class ModularComponent : public Component, public IModularComponent {
  
public:
  
  #pragma region ModularAudioProcessor
  
  struct ModularAudioProcessor : public AudioProcessor {

    sptr<AudioProcessorGraph> mainProcessor;
    
    AudioNode::Ptr masterAudioIn;
    AudioNode::Ptr masterAudioOut;
    AudioNode::Ptr masterMidiIn;
    AudioNode::Ptr masterMidiOut;
    
    const int kChannelCount = 2;

    std::vector<AudioNode::Ptr> audioNodes;
    
    // Standalone Required Members
    sptr<AudioDeviceManager> audioDeviceManager;
    AudioProcessorPlayer player;
    
    explicit ModularAudioProcessor(const sptr<AudioDeviceManager> &audioDeviceManager);
    
    void initialiseGraph();
    
    void connectAudioNodes();

    void connectMidiNodes();

    #pragma region AudioProcessor Methods
    
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;

    const String getName() const override;

    void prepareToPlay(double sampleRate, int maximumExpectedSamplesPerBlock) override;

    void releaseResources() override;

    void processBlock(AudioBuffer<float> &buffer, MidiBuffer &midiMessages) override;

    double getTailLengthSeconds() const override;

    bool acceptsMidi() const override;

    bool producesMidi() const override;

    AudioProcessorEditor *createEditor() override;

    bool hasEditor() const override;

    int getNumPrograms() override;

    int getCurrentProgram() override;

    void setCurrentProgram(int index) override;

    const String getProgramName(int index) override;

    void changeProgramName(int index, const String &newName) override;

    void getStateInformation(MemoryBlock &destData) override;

    void setStateInformation(const void *data, int sizeInBytes) override;

    #pragma endregion AudioProcessor Methods
    
  };

  #pragma endregion ModularAudioProcessor

  #pragma region ModularNodeGraphView
  
  struct ModularNodeGraphView
  : public GraphViewComponent,
    public Shareable<ModularNodeGraphView>
  {
    
    sptr<ModularAudioProcessor> modularAudio;
    std::vector<sptr<NodeComponent>> modules;
    std::map<const NodeComponent*, AudioNode::Ptr> audioNodeTether;

    explicit ModularNodeGraphView(const sptr<ModularAudioProcessor> &modularAudio);
    
    void popupMenu(const MouseEvent &e) override;

    // TODO: implement this. It will be used to recall presets.
    void initializeGraphFromPreset();

    #pragma region GraphViewComponent Methods

    void onAfterAddHostNode(const HostNodeComponent *node, const Graph::Node *model) override;

    void onBeforeRemoveNode(const NodeComponent *n) override;

    void onAfterRemoveNode() override;

    void onAfterAddWire(const NodeComponent::PinComponent *source, const NodeComponent::PinComponent *target) override;

    void onBeforeRemoveWire(const WireComponent *w) override;

    void onAfterRemoveWire() override;
  
  protected:
    
    void debugGraph();
  
    void debugNodeComponent(const NodeComponent &n, bool recurseUpstream= false);
    
    void debugNode(const Graph::Node &p, bool recurseUpstream= false);
    
    void debugNodeConnectors(const Graph::Node &n, bool recurseUpstream= false);
    
    void debugPin(const Graph::Pin &p, bool recurseUpstream= false);
    
    static void debugHeader(const std::string &title);
    
  private:
    
    int _dbgIndent = 0;
    
    static std::string indentN(int x);

    #pragma endregion GraphViewComponent Methods

  };

  #pragma endregion ModularNodeGraphView

  #pragma region Public Members
  
public:
  
  sptr<ModularAudioProcessor> modularAudioProcessor;
  
  sptr<ModularNodeGraphView> view;
  
  sptr<Toolbar> toolbar;
  
  std::vector<uptr<AudioNode>> audioNodes;

  #pragma endregion Public Members

  #pragma region NonPublic Members

protected:

  MidiKeyboardState keyboardState;

  sptr<MidiKeyboardComponent> keyboardComponent;
  
  #pragma endregion NonPublic Members

  #pragma region Public Methods
  
public:
  
  ModularComponent(sptr<AudioDeviceManager> audioDeviceManager, sptr<MouseListener> sharedMouseListener);
  
  ~ModularComponent() override;

  void paint(Graphics &g) override;

  void resized() override;
  
  void layout();
  
  void childBoundsChanged(Component* child) override;
  
  #pragma region Public Methods
  
private:
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ModularComponent);

};

}
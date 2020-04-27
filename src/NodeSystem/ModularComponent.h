//
// Created by asorgejr on 4/17/2020.
//

#pragma once
#include <JuceHeader.h>
#include <modularsynth/IModularComponent.h>
#include <nodesystem/nodesystem.h>
#include "../common.h"
#include "../Utilities.h"

namespace modularsynth {
using AudioGraphIOProcessor = AudioProcessorGraph::AudioGraphIOProcessor;
using AudioNode = AudioProcessorGraph::Node;
using NodeID = AudioProcessorGraph::NodeID;

class ModularComponent : nodesystem::Shareable<ModularComponent>,
  public IModularComponent
{
  
public:
  
  #pragma region ModularAudioProcessor
  
  struct ModularAudioProcessor 
  : public AudioProcessor,
    public nodesystem::Shareable<ModularAudioProcessor>
  {

    sptr<AudioDeviceManager>
    audioDeviceManager;

    sptr<AudioProcessorGraph>
    mainProcessor;
    
    AudioNode::Ptr masterAudioIn;
    AudioNode::Ptr masterAudioOut;
    AudioNode::Ptr masterMidiIn;
    AudioNode::Ptr masterMidiOut;
    
    ModularAudioProcessor(sptr<AudioDeviceManager> audioDeviceManager);
    
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
  
  struct ModularNodeGraphView : 
    public nodesystem::Shareable<ModularNodeGraphView>,
    public nodesystem::GraphViewComponent
  {
    explicit ModularNodeGraphView(sptr<AudioProcessorGraph> audioGraph);
    
    sptr<AudioProcessorGraph> audioGraph;
    
    void popupMenu(const MouseEvent &e) override;
    
  };
  
  sptr<ModularAudioProcessor>
  modularAudioProcessor;
  
  sptr<ModularNodeGraphView>
  view;
  
  sptr<Toolbar>
  toolbar;
  
  std::vector<std::unique_ptr<AudioNode>>
  audioNodes;
  
public:
  ModularComponent(sptr<AudioDeviceManager> audioDeviceManager, sptr<MouseListener> sharedMouseListener);
  
  ~ModularComponent() override;

  void paint(Graphics &g) override;

  void resized() override;
  
  void layout();
  
  void childBoundsChanged(Component* child) override;
  
  // void getView(const ModularNodeGraphView *self, const std::function<void(sptr<ModularNodeGraphView>)> &func);

  void initialiseGraph();

};

}
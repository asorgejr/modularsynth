//
// Created by asorgejr on 4/17/2020.
//

#pragma once
#include <JuceHeader.h>
#include <nodesystem/nodesystem.h>

#include "../../Models/IModularComponent.h"
#include "../../common.h"
#include "../../Utilities.h"

namespace modularsynth {
using AudioGraphIOProcessor = AudioProcessorGraph::AudioGraphIOProcessor;
using AudioNode = AudioProcessorGraph::Node;
using NodeID = AudioProcessorGraph::NodeID;

class ModularComponent : public Component, public IModularComponent {
  
public:
  
  #pragma region ModularAudioProcessor
  
  struct ModularAudioProcessor : public AudioProcessor {

    sptr<AudioDeviceManager>
    audioDeviceManager;

    sptr<AudioProcessorGraph>
    mainProcessor;
    
    AudioNode::Ptr masterAudioIn;
    AudioNode::Ptr masterAudioOut;
    AudioNode::Ptr masterMidiIn;
    AudioNode::Ptr masterMidiOut;
    
    ModularAudioProcessor(const sptr<AudioDeviceManager> &audioDeviceManager);
    
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
  
  struct ModularNodeGraphView
  : public nodesystem::GraphViewComponent,
    public Shareable<ModularNodeGraphView>
  {
    
    explicit ModularNodeGraphView(const sptr<AudioProcessorGraph> &audioGraph);
    
    sptr<AudioProcessorGraph> audioGraph;
    
    void popupMenu(const MouseEvent &e) override;
    
  };
  
  sptr<ModularAudioProcessor>
  modularAudioProcessor;
  
  sptr<ModularNodeGraphView>
  view;
  
  sptr<Toolbar>
  toolbar;
  
  std::vector<uptr<AudioNode>>
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
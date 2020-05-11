//
// Created by asorgejr on 5/5/2020.
//

#pragma once
#include <JuceHeader.h>
#include "../common.h"
#include "NodeSystem/nodesystem.h"

struct ModularAudioProcessor : public AudioProcessor {

private:

  AudioNode::Ptr masterAudioIn;
  AudioNode::Ptr masterAudioOut;
  AudioNode::Ptr masterMidiIn;
  AudioNode::Ptr masterMidiOut;
  
public:
  
  sptr<AudioProcessorGraph> mainProcessor;
  
  sptr<Graph> graph;

  const int kChannelCount = 2;

  uptr<UndoManager> undoManager;

  // Standalone App Required Members
  sptr<AudioDeviceManager> audioDeviceManager;
  

  explicit ModularAudioProcessor(const sptr<AudioDeviceManager> &audioDeviceManager);
  
  void initialiseMasterNodes();

  void initialiseGraph();

  void connectAudioNodes();

  void connectMidiNodes();
  
  AudioNode::Ptr getMasterAudioIn();
  
  AudioNode::Ptr getMasterAudioOut();
  
  AudioNode::Ptr getMasterMidiIn();
  
  AudioNode::Ptr getMasterMidiOut();

  #pragma region AudioProcessor Methods

  bool isBusesLayoutSupported(const BusesLayout &layouts) const override;

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

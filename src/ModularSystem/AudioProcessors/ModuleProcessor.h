
#pragma once
#include <JuceHeader.h>
#include "../ModularAudioProcessor.h"
#include "ModuleParameterView.h"

typedef std::vector<AudioProcessorParameter*> ParameterList;

typedef std::function<AudioProcessorValueTreeState*()> ModuleValueTreeGenerator;

using namespace std;

class ModuleProcessor : public AudioProcessor {
  
public:
  
  //==============================================================================
  
  explicit ModuleProcessor(AudioProcessorValueTreeState &state) {
  }
  
  ~ModuleProcessor() {}

  //==============================================================================
  
  void prepareToPlay (double, int) override {}
  
  void releaseResources() override {}
  
  void processBlock (AudioSampleBuffer& buffer, MidiBuffer&) override {}

  //==============================================================================
  
  ModuleParameterView * createEditor() override          { return new ModuleParameterView(*this); }
  
  bool hasEditor() const override                        { return true; }

  //==============================================================================
  

  const String getName() const override                  { return {}; }

  bool acceptsMidi() const override                      { return false; }
  
  bool producesMidi() const override                     { return false; }
  
  double getTailLengthSeconds() const override           { return 0; }

  //==============================================================================
  
  int getNumPrograms() override                          { return 0; }
  
  int getCurrentProgram() override                       { return 0; }
  
  void setCurrentProgram (int) override                  {}
  
  const String getProgramName (int) override             { return {}; }
  
  void changeProgramName (int, const String&) override   {}

  //==============================================================================
  
  void getStateInformation (MemoryBlock& destData) override             {}
  
  void setStateInformation (const void* data, int sizeInBytes) override {}

  //==============================================================================
  
  virtual const Identifier getId() = 0;

private:
  //==============================================================================
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ModuleProcessor)
};


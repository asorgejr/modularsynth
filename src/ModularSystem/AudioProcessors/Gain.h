//
// Created by asorgejr on 4/25/2020.
//

#pragma once
#include "ModuleProcessor.h"

class Gain : public ModuleProcessor {
  
private:
  const Identifier kIdentifier { Identifier("gain") };
  const string kPhaseId = "gain";
  const string kGainId = "invertPhase";

  AudioProcessorValueTreeState parameters;
  
  float previousGain;

  atomic<float>* phaseParameter;
  atomic<float> *gainParameter;
  
public:
  
  Gain(ModularAudioProcessor *parent)
  : parameters(*this, parent->undoManager.get(), getId(), {
      make_unique<AudioParameterFloat>(
        kGainId,
        "Gain",
        NormalisableRange<float>(-60.0f, 6.0f),
        -10.0f),
      make_unique<AudioParameterBool>(
        kPhaseId,
        "Invert Phase",
        false)
    }),
    ModuleProcessor((AudioProcessorValueTreeState &) parameters)
  {
    gainParameter = parameters.getRawParameterValue(kGainId);
    gain.setGainDecibels(*gainParameter);
  }
  
  const String getName() const override { return "Gain"; }

  void prepareToPlay(double sampleRate, int samplesPerBlock) override {
    dsp::ProcessSpec spec { sampleRate, static_cast<uint32>(samplesPerBlock), 2 };
    gain.prepare(spec);
  }

  void processBlock(AudioSampleBuffer& buffer, MidiBuffer& midiMessages) override {
    dsp::AudioBlock<float> block(buffer);
    dsp::ProcessContextReplacing<float> context(block);
    gain.process(context);
  }

  void reset() override {
    gain.reset();
  }

  ModuleParameterView * createEditor() override {
    return new ModuleParameterView(*this);
  }

  const Identifier getId() override {
    return kIdentifier;
  }

private:
  
  dsp::Gain<float> gain;
  
};

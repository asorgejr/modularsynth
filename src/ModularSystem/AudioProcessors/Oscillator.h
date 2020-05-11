//
// Created by asorgejr on 4/25/2020.
//

#pragma once
#include "ModuleProcessor.h"

class Oscillator : public ModuleProcessor {
  
private:
  const Identifier kIdentifier { Identifier("oscillator") };
  const string kFreqId = "frequency";
  const string kInvertId = "invert";
  
  atomic<float> *freqParam;
  
  AudioProcessorValueTreeState parameters;
  
public:
  
  explicit Oscillator(ModularAudioProcessor *parent) 
  : parameters(*this, parent->undoManager.get(), getId(), {
    make_unique<AudioParameterFloat>(
      kFreqId,
      "Frequency",
      NormalisableRange<float>(0.f, 22000.f),
      440.f),
    make_unique<AudioParameterBool>(
      kInvertId,
      "Invert",
      false)
  }),
    ModuleProcessor((AudioProcessorValueTreeState &) parameters)
  {
    freqParam = parameters.getRawParameterValue(kFreqId);
    oscillator.setFrequency(*freqParam);
    oscillator.initialise ([] (float x) { return std::sin(x); });
  }
  
  const String getName() const override { return "Oscillator"; }

  void prepareToPlay(double sampleRate, int samplesPerBlock) override {
    cout << "Oscillator::prepareToPlay" << endl;
    dsp::ProcessSpec spec { sampleRate, static_cast<uint32>(samplesPerBlock) };
    oscillator.prepare(spec);
  }

  void processBlock(AudioSampleBuffer& buffer, MidiBuffer& midiMessages) override {
    dsp::AudioBlock<float> block(buffer);
    dsp::ProcessContextReplacing<float> context(block);
    oscillator.process(context);
  }

  void reset() override {
    oscillator.reset();
  }

  ModuleParameterView * createEditor() override { 
    return new ModuleParameterView(*this);
  }

  const Identifier getId() override {
    return kIdentifier;
  }
  
  dsp::Oscillator<float> oscillator;
  
};

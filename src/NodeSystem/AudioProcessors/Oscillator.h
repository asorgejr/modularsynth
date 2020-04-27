//
// Created by asorgejr on 4/25/2020.
//

#pragma once
#include "../ProcessorBase.h"


class Oscillator  : public ProcessorBase {
  
public:
  
  Oscillator() {
    oscillator.setFrequency (440.0f);
    oscillator.initialise ([] (float x) { return std::sin (x); });
  }
  
  const String getName() const override { return "Oscillator"; }

  void prepareToPlay (double sampleRate, int samplesPerBlock) override {
    dsp::ProcessSpec spec { sampleRate, static_cast<uint32> (samplesPerBlock) };
    oscillator.prepare (spec);
  }

  void processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages) override {
    dsp::AudioBlock<float> block (buffer);
    dsp::ProcessContextReplacing<float> context (block);
    oscillator.process (context);
  }

  void reset() override {
    oscillator.reset();
  }

private:
  
  dsp::Oscillator<float> oscillator;
  
};

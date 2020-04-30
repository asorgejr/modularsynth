//
// Created by asorgejr on 4/25/2020.
//

#pragma once
#include "ProcessorBase.h"

class Gain : ProcessorBase {
  
public:
  
  Gain() {
    gain.setGainDecibels (-6.0f);
  }
  
  const String getName() const override { return "Gain"; }

  void prepareToPlay (double sampleRate, int samplesPerBlock) override {
    dsp::ProcessSpec spec { sampleRate, static_cast<uint32> (samplesPerBlock), 2 };
    gain.prepare (spec);
  }

  void processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages) override {
    dsp::AudioBlock<float> block (buffer);
    dsp::ProcessContextReplacing<float> context (block);
    gain.process (context);
  }

  void reset() override {
    gain.reset();
  }

private:
  
  dsp::Gain<float> gain;
  
};

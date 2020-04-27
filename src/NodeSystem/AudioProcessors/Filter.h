//
// Created by asorgejr on 4/25/2020.
//

#pragma once
#include "../ProcessorBase.h"

class Filter : ProcessorBase {
  
public:
  
  Filter() {}
  
  const String getName() const override { return "Filter"; }

  void prepareToPlay (double sampleRate, int samplesPerBlock) override {
    *filter.state = *dsp::IIR::Coefficients<float>::makeHighPass (sampleRate, 1000.0f);

    dsp::ProcessSpec spec { sampleRate, static_cast<uint32> (samplesPerBlock), 2 };
    filter.prepare (spec);
  }

  void processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages) override {
    dsp::AudioBlock<float> block (buffer);
    dsp::ProcessContextReplacing<float> context (block);
    filter.process (context);
  }

  void reset() override {
    filter.reset();
  }

private:
  
  dsp::ProcessorDuplicator<dsp::IIR::Filter<float>, dsp::IIR::Coefficients<float>> filter;
  
};



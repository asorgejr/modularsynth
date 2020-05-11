//
// Created by asorgejr on 4/25/2020.
//

#pragma once
#include "ModuleProcessor.h"

class Filter : public ModuleProcessor {

private:
  const Identifier kIdentifier { Identifier("filter") };
  const string kFreqId = "frequency";
  
  atomic<float> *freqParam;

  AudioProcessorValueTreeState parameters;
  
public:
  
  Filter(ModularAudioProcessor *parent) 
  : parameters(*this, parent->undoManager.get(), getId(), {
      make_unique<AudioParameterFloat>(
        kFreqId,
        "Frequency",
        NormalisableRange<float>(0.f, 22000.f),
        440.f
      )
    }),
    ModuleProcessor((AudioProcessorValueTreeState &) parameters) 
  {
    freqParam = parameters.getRawParameterValue(kFreqId);
  }
  
  const String getName() const override { return "Filter"; }

  void prepareToPlay (double sampleRate, int samplesPerBlock) override {
    *filter.state = *dsp::IIR::Coefficients<float>::makeHighPass (sampleRate, *freqParam);

    dsp::ProcessSpec spec { sampleRate, static_cast<uint32>(samplesPerBlock), 2 };
    filter.prepare(spec);
  }

  void processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages) override {
    dsp::AudioBlock<float> block (buffer);
    dsp::ProcessContextReplacing<float> context (block);
    filter.process (context);
  }

  void reset() override {
    filter.reset();
  }

  ModuleParameterView * createEditor() override {
    return new ModuleParameterView(*this);
  }

  const Identifier getId() override {
    return kIdentifier;
  }

private:
  
  dsp::ProcessorDuplicator<dsp::IIR::Filter<float>, dsp::IIR::Coefficients<float>> filter;
  
};



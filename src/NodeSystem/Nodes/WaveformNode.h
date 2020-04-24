//
// Created by asorgejr on 4/17/2020.
//

#pragma once
#include <JuceHeader.h>
#include <modularsynth/Module.h>
namespace modularsynth {

namespace node {

class WaveformNode : Module {
  Array<AudioChannelSet> InputBuses() override {
    return Array<AudioChannelSet>();
  }

  Array<AudioChannelSet> OutputBuses() override {
    return Array<AudioChannelSet>();
  }

};

}

}
//
// Created by asorgejr on 4/17/2020.
//

#pragma once
#include <JuceHeader.h>

namespace modularsynth {

class Module : public AudioProcessor {
public:
  Module();
  ~Module() override = default;

  struct ModuleShape {
    Rectangle<float> rect = Rectangle<float>(0, 0, 80, 50);
    int cornerSize = 5;
    int lineThickness = 5;
  }
  Shape = ModuleShape();

protected:
  virtual Array<AudioChannelSet> InputBuses() {
    return Array<AudioChannelSet>();
  };
  virtual Array<AudioChannelSet> OutputBuses() {
    return Array<AudioChannelSet>();
  };
};

}
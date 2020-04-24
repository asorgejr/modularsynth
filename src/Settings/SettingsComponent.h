//
// Created by asorgejr on 4/21/2020.
//
#pragma once
#include <JuceHeader.h>
#include "AudioSettingsComponent.h"

namespace modularsynth {

class SettingsComponent : public IModularComponent {
public:
  SettingsComponent(std::shared_ptr<AudioDeviceManager> audioDeviceManager, std::shared_ptr<MouseListener> sharedMouseListener);

  ~SettingsComponent() override;

  void paint(Graphics &g) override;

  void resized() override;

private:
  std::shared_ptr<AudioSettingsComponent> _audioSettingsComponent;

};

}
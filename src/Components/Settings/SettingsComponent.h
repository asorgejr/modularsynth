//
// Created by asorgejr on 4/21/2020.
//
#pragma once
#include <JuceHeader.h>
#include "../../common.h"
#include "AudioSettingsComponent.h"
#include "../../Services/UserSettings.h"

namespace modularsynth {

class SettingsComponent : public Component, public IModularComponent {

#pragma region Public Members
  
public:
  
  sptr<UserSettings> userSettings;
  
#pragma endregion Public Members

  #pragma region Public Methods
  
public:
  
  SettingsComponent(const sptr<UserSettings> &userSettings,
                    const sptr<AudioDeviceManager> &audioDeviceManager,
                    const sptr<MouseListener> &sharedMouseListener);

  ~SettingsComponent() override;

  void paint(Graphics &g) override;

  void resized() override;

  sptr<AudioSettingsComponent> getAudioSettingsComponent();

  #pragma endregion Public Methods

private:
  
  sptr<AudioSettingsComponent> _audioSettingsComponent;

};

}

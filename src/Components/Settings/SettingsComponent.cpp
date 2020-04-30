//
// Created by asorgejr on 4/21/2020.
//

#include "SettingsComponent.h"

#include <utility>
using namespace std;

namespace modularsynth {

SettingsComponent::SettingsComponent(const sptr<UserSettings> &userSettings,
    const sptr<AudioDeviceManager> &audioDeviceManager,
    const sptr<MouseListener> &sharedMouseListener)
: Component(),
  IModularComponent(audioDeviceManager, sharedMouseListener),
  _audioSettingsComponent(new AudioSettingsComponent(userSettings, audioDeviceManager, sharedMouseListener))
{
  this->userSettings = userSettings;
  addAndMakeVisible(_audioSettingsComponent.get());
}

SettingsComponent::~SettingsComponent() {

}

void SettingsComponent::paint(Graphics &g) {
  Component::paint(g);
}

void SettingsComponent::resized() {
  Component::resized();
}

sptr<AudioSettingsComponent> SettingsComponent
::getAudioSettingsComponent() {
  return _audioSettingsComponent;
}

}

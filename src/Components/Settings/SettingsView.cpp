//
// Created by asorgejr on 4/21/2020.
//

#include "SettingsView.h"

#include <utility>
using namespace std;

namespace modularsynth {

SettingsView::SettingsView(const sptr<UserSettings> &userSettings,
    const sptr<AudioDeviceManager> &audioDeviceManager,
    const sptr<MouseListener> &sharedMouseListener)
: Component(),
  IModularComponent(audioDeviceManager, sharedMouseListener),
  _audioSettingsComponent(new AudioSettingsComponent(userSettings, audioDeviceManager, sharedMouseListener))
{
  this->userSettings = userSettings;
  addAndMakeVisible(_audioSettingsComponent.get());
}

SettingsView::~SettingsView() {

}

void SettingsView::paint(Graphics &g) {
  Component::paint(g);
}

void SettingsView::resized() {
  Component::resized();
}

sptr<AudioSettingsComponent> SettingsView
::getAudioSettingsComponent() {
  return _audioSettingsComponent;
}

}

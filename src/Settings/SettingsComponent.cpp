//
// Created by asorgejr on 4/21/2020.
//

#include "SettingsComponent.h"
using namespace std;

namespace modularsynth {

SettingsComponent::SettingsComponent(shared_ptr<AudioDeviceManager> audioDeviceManager,
  shared_ptr<MouseListener> sharedMouseListener)
  : IModularComponent(move(audioDeviceManager), move(sharedMouseListener)),
  _audioSettingsComponent(    new AudioSettingsComponent(
      this->audioDeviceManager,
      this->sharedMouseListener))
{
  
}

SettingsComponent::~SettingsComponent() {

}

void SettingsComponent::paint(Graphics &g) {
  Component::paint(g);
}

void SettingsComponent::resized() {
  Component::resized();
}

}
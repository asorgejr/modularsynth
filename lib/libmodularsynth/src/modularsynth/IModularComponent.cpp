//
// Created by asorgejr on 4/21/2020.
//

#pragma once

#include "../../include/modularsynth/IModularComponent.h"

#include <utility>
using namespace std;

namespace modularsynth {

IModularComponent::IModularComponent(std::shared_ptr<AudioDeviceManager> audioDeviceManager, std::shared_ptr<MouseListener> sharedMouseListener)
  : Component(),
    audioDeviceManager(move(audioDeviceManager)),
    sharedMouseListener(move(sharedMouseListener)) {
}

void IModularComponent::mouseDown(const MouseEvent &event) {
  sharedMouseListener->mouseDown(event);
}

void IModularComponent::mouseUp(const MouseEvent &event) {
  sharedMouseListener->mouseUp(event);
}

void IModularComponent::mouseMove(const MouseEvent &event) {
  sharedMouseListener->mouseMove(event);
}

void IModularComponent::mouseDrag(const MouseEvent &event) {
  sharedMouseListener->mouseDrag(event);
}

void IModularComponent::mouseDoubleClick(const MouseEvent &event) {
  sharedMouseListener->mouseDoubleClick(event);
}

void IModularComponent::mouseEnter(const MouseEvent &event) {
  sharedMouseListener->mouseEnter(event);
}

void IModularComponent::mouseExit(const MouseEvent &event) {
  sharedMouseListener->mouseExit(event);
}

void IModularComponent::mouseMagnify(const MouseEvent &event, float scaleFactor) {
  sharedMouseListener->mouseMagnify(event, scaleFactor);
}

void IModularComponent::mouseWheelMove(const MouseEvent &event, const MouseWheelDetails &wheel) {
  sharedMouseListener->mouseWheelMove(event, wheel);
}


}


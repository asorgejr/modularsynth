
#pragma once
#include <iostream>
#include "../../Models/IModularComponent.h"
#include "../../Utilities.h"
#include "../../AudioLiveScrollingDisplay.h"
#include "../../Synthesizer/AudioSource.h"


namespace modularsynth {

//==============================================================================
class SynthKeyboardComponent : public Component, public IModularComponent {
public:
  explicit SynthKeyboardComponent(std::shared_ptr<AudioDeviceManager> audioDeviceManager, std::shared_ptr<MouseListener> sharedMouseListener);

  ~SynthKeyboardComponent() override;

  //==============================================================================
  void paint(Graphics &g) override;

  void resized() override;

private:
  // if this PIP is running inside the demo runner, we'll use the shared device manager instead
  // AudioDeviceManager audioDeviceManager;

  MidiKeyboardState _keyboardState;
  AudioSourcePlayer _audioSourcePlayer;
  SynthAudioSource _synthAudioSource{_keyboardState };
  MidiKeyboardComponent _keyboardComponent{_keyboardState, MidiKeyboardComponent::horizontalKeyboard };

  ToggleButton _sineButton{ "Use sine wave" };
  ToggleButton _sampledButton{ "Use sampled sound" };

  LiveScrollingAudioDisplay _liveAudioDisplayComp;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SynthKeyboardComponent)
};

}
#include "Utilities.h"
#include "SynthKeyboardComponent.h"

using namespace std;

namespace modularsynth {

SynthKeyboardComponent::SynthKeyboardComponent(shared_ptr<AudioDeviceManager> audioDeviceManager, shared_ptr<MouseListener> sharedMouseListener)
  : IModularComponent(move(audioDeviceManager), move(sharedMouseListener))
{
  addAndMakeVisible(_keyboardComponent);

  addAndMakeVisible(_sineButton);
  _sineButton.setRadioGroupId(321);
  _sineButton.setToggleState(true, dontSendNotification);
  _sineButton.onClick = [this] { _synthAudioSource.setUsingSineWaveSound(); };

  addAndMakeVisible(_sampledButton);
  _sampledButton.setRadioGroupId(321);
  _sampledButton.onClick = [this] { _synthAudioSource.setUsingSampledSound(); };

  addAndMakeVisible(_liveAudioDisplayComp);
  this->audioDeviceManager->addAudioCallback(&_liveAudioDisplayComp);
  this->_audioSourcePlayer.setSource(&_synthAudioSource);

  #ifndef JUCE_DEMO_RUNNER
  RuntimePermissions::request(
    RuntimePermissions::recordAudio,
    [this](bool granted) {
      int numInputChannels = granted ? 2 : 0;
      this->audioDeviceManager->initialise(numInputChannels, 2, nullptr, true, {}, nullptr);
  });
  #endif

  this->audioDeviceManager->addAudioCallback(&_audioSourcePlayer);
  this->audioDeviceManager->addMidiInputDeviceCallback({}, &(_synthAudioSource.midiCollector));

  setOpaque(true);
  setSize(640, 480);
}

SynthKeyboardComponent::~SynthKeyboardComponent() {
  _audioSourcePlayer.setSource(nullptr);
  audioDeviceManager->removeMidiInputDeviceCallback({}, &(_synthAudioSource.midiCollector));
  audioDeviceManager->removeAudioCallback(&_audioSourcePlayer);
  audioDeviceManager->removeAudioCallback(&_liveAudioDisplayComp);
}

void SynthKeyboardComponent::paint(Graphics &g) {
  g.fillAll(getUIColourIfAvailable(LookAndFeel_V4::ColourScheme::UIColour::windowBackground));
}

void SynthKeyboardComponent::resized() {
  _keyboardComponent.setBounds(8, 96, getWidth() - 16, 64);
  _sineButton.setBounds(16, 176, 150, 24);
  _sampledButton.setBounds(16, 200, 150, 24);
  _liveAudioDisplayComp.setBounds(8, 8, getWidth() - 16, 64);
}
}
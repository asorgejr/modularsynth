// AudioSettingsComponent.h
// compile with: /LD /clr /doc

#include "AudioSettingsComponent.h"

#include <memory>
#include "../../app/config.h"

using namespace std;

namespace modularsynth {
using namespace config;

AudioSettingsComponent::AudioSettingsComponent(std::shared_ptr<AudioDeviceManager> audioDeviceManager, std::shared_ptr<MouseListener> sharedMouseListener) 
: IModularComponent(move(audioDeviceManager), move(sharedMouseListener)) 
{
  setOpaque(true);
  
  RuntimePermissions::request(RuntimePermissions::recordAudio,
    [this](bool granted) {
      int numInputChannels = granted ? 2 : 0;
      this->audioDeviceManager->initialise(numInputChannels, 2, nullptr, true, {}, nullptr);
  });
  
  audioSetupComp = std::make_unique<AudioDeviceSelectorComponent>(
    *this->audioDeviceManager,
    0, 
    256, 
    0, 
    256, 
    true, 
    true, 
    true, 
    false
  );
  
  addAndMakeVisible(*audioSetupComp);

  addAndMakeVisible(diagnosticsBox);
  diagnosticsBox.setMultiLine(true);
  diagnosticsBox.setReturnKeyStartsNewLine(true);
  diagnosticsBox.setReadOnly(true);
  diagnosticsBox.setScrollbarsShown(true);
  diagnosticsBox.setCaretVisible(false);
  diagnosticsBox.setPopupMenuEnabled(true);

  this->audioDeviceManager->addChangeListener(this);

  logMessage("Audio device diagnostics:\n");
  dumpDeviceInfo();

  this->setSize(kDefaultWidth, kDefaultHeight);
}


AudioSettingsComponent::~AudioSettingsComponent() {
  audioDeviceManager->removeChangeListener(this);
}

void AudioSettingsComponent::paint(Graphics &g) {
  g.fillAll(getUIColourIfAvailable(CS::UIColour::windowBackground));
}

void AudioSettingsComponent::resized() {
  auto r = getLocalBounds().reduced(4);
  audioSetupComp->setBounds(r.removeFromTop(proportionOfHeight(0.65f)));
  diagnosticsBox.setBounds(r);
}


void AudioSettingsComponent::dumpDeviceInfo() {
  logMessage("--------------------------------------");
  logMessage("Current audio device type: "
    + (audioDeviceManager->getCurrentDeviceTypeObject() != nullptr
      ? audioDeviceManager->getCurrentDeviceTypeObject()->getTypeName()
      : "<none>")
  );

  if (AudioIODevice *device = audioDeviceManager->getCurrentAudioDevice()) {
    logMessage("Current audio device: " + device->getName().quoted());
    logMessage("Sample rate: " + String(device->getCurrentSampleRate()) + " Hz");
    logMessage("Block size: " + String(device->getCurrentBufferSizeSamples()) + " samples");
    logMessage("Output Latency: " + String(device->getOutputLatencyInSamples()) + " samples");
    logMessage("Input Latency: " + String(device->getInputLatencyInSamples()) + " samples");
    logMessage("Bit depth: " + String(device->getCurrentBitDepth()));
    logMessage("Input channel names: " + device->getInputChannelNames().joinIntoString(", "));
    logMessage("Active input channels: " + getListOfActiveBits(device->getActiveInputChannels()));
    logMessage("Output channel names: " + device->getOutputChannelNames().joinIntoString(", "));
    logMessage("Active output channels: " + getListOfActiveBits(device->getActiveOutputChannels()));
  } else {
    logMessage("No audio device open");
  }
}

void AudioSettingsComponent::logMessage(const String &m) {
  diagnosticsBox.moveCaretToEnd();
  diagnosticsBox.insertTextAtCaret(m + newLine);
}

void AudioSettingsComponent::changeListenerCallback(ChangeBroadcaster *) {
  dumpDeviceInfo();
}

void AudioSettingsComponent::lookAndFeelChanged() {
  diagnosticsBox.applyFontToAllText(diagnosticsBox.getFont());
}

String AudioSettingsComponent::getListOfActiveBits(const BigInteger &b) {
  StringArray bits;

  for (int i = 0; i <= b.getHighestBit(); ++i)
    if (b[i])
      bits.add(String(i));

  return bits.joinIntoString(", ");
}

}
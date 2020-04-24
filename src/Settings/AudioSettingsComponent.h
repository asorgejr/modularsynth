// AudioSettingsComponent.h
// compile with: /LD /clr /doc

#pragma once
#include "../Utilities.h"
#include <JuceHeader.h>
#include <modularsynth/IModularComponent.h>

namespace modularsynth {

class AudioSettingsComponent : public ChangeListener, public IModularComponent {
public:
  explicit AudioSettingsComponent(std::shared_ptr<AudioDeviceManager> audioDeviceManager, std::shared_ptr<MouseListener> sharedMouseListener);

  ~AudioSettingsComponent() override;

  void paint(Graphics &g) override;

  void resized() override;

  void dumpDeviceInfo();

  void logMessage(const String &m);

private:
  typedef LookAndFeel_V4 LF;
  typedef LF::ColourScheme CS;

  std::unique_ptr<AudioDeviceSelectorComponent> audioSetupComp;
  TextEditor diagnosticsBox;

  void changeListenerCallback(ChangeBroadcaster *) override;

  void lookAndFeelChanged() override;

  static String getListOfActiveBits(const BigInteger &b);

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioSettingsComponent)
};

}

// AudioSettingsComponent.h
// compile with: /LD /clr /doc

#pragma once
#include <JuceHeader.h>
#include "../../Models/IModularComponent.h"
#include "../../common.h"
#include "../../Services/UserSettings.h"
#include "../../Utilities.h"

namespace modularsynth {

class AudioSettingsComponent : public ChangeListener, public Component, public IModularComponent {
  
public:
  
  explicit AudioSettingsComponent(const sptr<UserSettings> &userSettings, const sptr<AudioDeviceManager> &audioDeviceManager, const sptr<MouseListener> &sharedMouseListener);

  ~AudioSettingsComponent() override;

  void paint(Graphics &g) override;

  void resized() override;

  void dumpDeviceInfo();

  void logMessage(const String &m);

private:
  
  typedef LookAndFeel_V4 LF;
  typedef LF::ColourScheme CS;

  uptr<AudioDeviceSelectorComponent> audioSetupComp;

  sptr<UserSettings> userSettings;
  
  TextEditor diagnosticsBox;

  void changeListenerCallback(ChangeBroadcaster *) override;

  void lookAndFeelChanged() override;

  static String getListOfActiveBits(const BigInteger &b);

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioSettingsComponent)
};

}

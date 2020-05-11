// MainComponent.h
// compile with: /LD /clr /doc

#pragma once
#include <memory>
#include <JuceHeader.h>
// #include <juce_audio_plugin_client/Standalone/juce_StandaloneFilterWindow.h>
#include <nameof.hpp>

#include "common.h"

#include "Models/IModularComponent.h"
#include "Services/UserSettings.h"
#include "ModularSystem/ModularComponent.h"
#include "Components/Settings/SettingsView.h"
#include "Components/SynthKeyboard/SynthKeyboardComponent.h"


//==============================================================================
namespace modularsynth {

typedef LookAndFeel_V4 LV4;

class MainComponent
: public Component,
  public ChangeListener,
  public AudioProcessorPlayer
{

public:
  
  uptr<LV4> look { dynamic_cast<LookAndFeel_V4 *> (&LookAndFeel::getDefaultLookAndFeel()) };

#pragma region Protected Members
  
protected:

  struct Layout {
    inline static const std::string kModularGraphName = "_" + (std::string) NAMEOF_TYPE(ModularComponent);
    inline static const std::string kModularGraphTabName = "Node Graph";
    inline static const std::string kSettingsComponentName = "_" + (std::string) NAMEOF_TYPE(AudioSettingsComponent);
    inline static const std::string kSettingsTabName = "Audio Settings";
    inline static const std::string kTabComponentName = "_TabbedComponent";
    inline static const int16 kModularGraphTabIndex = 0;
    inline static const int16 kettingsTabIndex = 1;
    inline static const int32 TabHeight = 30;
    static const TabbedButtonBar::Orientation kTabOrient = TabbedButtonBar::Orientation::TabsAtTop;
    static Rectangle<int32> DefaultRect() { return Rectangle<int32> (_defaultRect); }
  private:
    inline static Rectangle<int32> _defaultRect = Rectangle<int32>(0, 0, config::kDefaultWidth, config::kDefaultHeight);
  };

  static const int32 kMaxPosition = 130944;
  
#pragma endregion Protected Members

#pragma region Private Members
  
private:
  sptr<AudioDeviceManager> _audioDeviceManager;
  sptr<MouseListener> _mouseListener { std::make_shared<MouseListener>() };
  sptr<SettingsView> _settingsView;
  sptr<TabbedComponent> _tabComponent;
  sptr<ModularComponent> _modularComponent;
  sptr<UserSettings> _userSettings;
  MidiKeyboardState _keyboardState;
  
  PropertySet *_settings; // TODO: see if this can be uptr.
  
#pragma endregion Private Members

#pragma region Public Methods
  
public:
  
  explicit MainComponent(const std::string &name = "MainComponent");
  
  ~MainComponent() override;

  void paint(Graphics &g) override;
  
  void resized() override;

  void mouseMove(const MouseEvent& event) override;
  
  void mouseUp(const MouseEvent &event) override;
  
  void mouseDown(const MouseEvent &event) override;
  
  void changeListenerCallback(ChangeBroadcaster *) override;
  
  void lookAndFeelChanged() override {}
  
  void audioProcessorConfig();
  
  ModularAudioProcessor *getModularProcessor() {
    return _modularComponent->modularAudioProcessor.get();
  }

#pragma endregion Public Methods
  
#pragma region Private Methods

private:

  template<typename TValue>
  static void constrainBounds(Rectangle<TValue> &bounds);
  
  void instantiateChildren(int32 width, int32 height);
  
  static void debugChildComponent(const Component &comp, const std::string &compName = "");
  
#pragma endregion Private Methods
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)

};


} // namespace modularsynth

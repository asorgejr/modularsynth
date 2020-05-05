// MainComponent.h
// compile with: /LD /clr /doc

#pragma once
#include <memory>
#include <JuceHeader.h>
#include <nameof.hpp>
#include <nodesystem/GraphViewComponent.h>

#include "../app/config.h"
#include "../Utilities.h"
#include "../common.h"

#include "../Models/IModularComponent.h"
#include "../Services/UserSettings.h"
#include "ModularComponent/ModularComponent.h"
#include "Settings/SettingsComponent.h"
#include "SynthKeyboard/SynthKeyboardComponent.h"


//==============================================================================
namespace modularsynth {

class MainComponent : public Component, public ChangeListener {

#pragma region Protected Members
  
protected:
  typedef LookAndFeel_V4::ColourScheme CS;

  struct Layout {
    inline static const std::string kModularGraphName = "_" + (std::string) NAMEOF_TYPE(ModularComponent);
    inline static const std::string kModularGraphTabName = "Node Graph";
    inline static const std::string kSettingsComponentName = "_" + (std::string) NAMEOF_TYPE(AudioSettingsComponent);
    inline static const std::string kSettingsTabName = "Audio Settings";
    inline static const std::string kSynthComponentName = "_" + (std::string) NAMEOF_TYPE(SynthKeyboardComponent);
    inline static const std::string kSynthTabName = "Synth";
    inline static const std::string kTabComponentName = "_TabbedComponent";
    inline static const int16 kSynthKeyboardTabIndex = 0;
    inline static const int16 kModularNodeGraphTabIndex = 1;
    inline static const int16 kAudioSettingsTabIndex = 2;
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
  sptr<AudioDeviceManager> _audioDeviceManager { std::make_shared<AudioDeviceManager>() };
  sptr<MouseListener> _mouseListener { std::make_shared<MouseListener>() };
  ComboBox _midiInputList;
  MidiKeyboardState _keyboardState;
  sptr<SettingsComponent> _settingsView;
  // sptr<SynthKeyboardComponent> _synthKeyboardView;
  sptr<TabbedComponent> _tabComponent;
  sptr<ModularComponent> _modularNodeGraph;
  sptr<UserSettings> _userSettings;
  Rectangle<float> floatBounds;
  inline static int _curImage = 0;
  
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

#pragma endregion Public Methods
  
#pragma region Private Methods

private:

  template<typename TValue>
  static void constrainBounds(Rectangle<TValue> &bounds);
  
  void instantiateChildren(int32 width, int32 height);
  
  void debugChildren(const std::string &title = "");
  
  static void debugChildComponent(const Component &comp, const std::string &compName = "");
  
#pragma endregion Private Methods
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)

};


} // namespace modularsynth

/*
  ==============================================================================

    This file was auto-generated and contains the startup code for a PIP.

  ==============================================================================
*/
#include <chrono>
#include <JuceHeader.h>

#include "../src/MainComponent.h"

#include "config.h"

using namespace modularsynth;
using namespace config;

class Application : public JUCEApplication {
public:
  //==============================================================================
  Application() { 
    config::Init(); // Initializes all config static members.
  }
  const String getApplicationName() override    { return config::kApplicationName; }
  const String getApplicationVersion() override { return config::kApplicationVersion; }

  void initialise(const String &) override {
    mainWindow.reset(
      new MainWindow(config::kApplicationName, new MainComponent(config::kApplicationName), *this));
  }
  
  void shutdown() override {
     mainWindow = nullptr;
  }

private:
  class MainWindow : public DocumentWindow {
  public:
    MainWindow(const String &name, Component *c, JUCEApplication &a) : DocumentWindow(
        name,
        Desktop::getInstance()
        .getDefaultLookAndFeel()
        .findColour(backgroundColourId),
        TitleBarButtons::allButtons
      ), app(a)
    {
      setUsingNativeTitleBar(true);
      setContentOwned(c, true);

    #if JUCE_ANDROID || JUCE_IOS
      setFullScreen(true);
    #else
      setResizable(kResizable, false);
      setResizeLimits(kMinWidth, kMinHeight, kMaxWidth, kMaxHeight);
      centreWithSize(getWidth(), getHeight());
    #endif
      setVisible(true);
    }

    void closeButtonPressed() override {
      app.systemRequestedQuit();
    }

  private:
    JUCEApplication &app;

    //==============================================================================
    //JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainWindow)
  };

  std::unique_ptr<MainWindow> mainWindow;
};

//==============================================================================
START_JUCE_APPLICATION(Application)

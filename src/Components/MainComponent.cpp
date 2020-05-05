#include <typeinfo>
#include <string>
#include <array>
#include <nameof.hpp>

#include "MainComponent.h"

namespace modularsynth {
using namespace std;
using namespace config;

MainComponent::MainComponent(const std::string &name)
: Component() {
  string newName = name + "_" + getComponentID().toStdString();
  this->setName(newName);
  auto tabHeight = getParentHeight() - Layout::TabHeight;
  auto width = getParentWidth();
  cout << "window dimensions: (" << width << ", " << tabHeight << ")" << endl;
  setOpaque(true);
  addMouseListener(_mouseListener.get(), false); //TODO: figure this out.
  setRepaintsOnMouseActivity(true);
  setLookAndFeel(new LookAndFeel_V4(config::Palette::colorScheme));
  
  instantiateChildren(width, tabHeight);
  
  setSize(kDefaultWidth, kDefaultHeight);
  debugChildren("Constructor");
}

MainComponent::~MainComponent() {
  removeAllChildren();
}

void MainComponent::paint(Graphics& g) {
  g.fillAll(getUIColourIfAvailable(CS::UIColour::windowBackground));
}

void MainComponent::instantiateChildren(int32 width, int32 height) {
  auto thisName = this->getName();
  
  _userSettings = make_shared<UserSettings>(_audioDeviceManager);
  auto audioSettings = _userSettings->audioDeviceSetup;
  
  _settingsView = make_shared<SettingsComponent>(_userSettings, _audioDeviceManager, _mouseListener);
//  _synthKeyboardView = make_shared<SynthKeyboardComponent>(_audioDeviceManager, _mouseListener);
  
  _audioDeviceManager->setAudioDeviceSetup(audioSettings, true);
  
  _tabComponent = make_shared<TabbedComponent>(TabbedButtonBar::Orientation::TabsAtTop);
  
  // addAndMakeVisible (_midiInputList);
  addAndMakeVisible(*_tabComponent);
  _tabComponent->addMouseListener(_mouseListener.get(), true);
  _modularNodeGraph = make_shared<ModularComponent>(_audioDeviceManager, _mouseListener);
  
  // Initialize Values for each child.
  _tabComponent->setName(thisName + Layout::kTabComponentName);
  _tabComponent->setBounds(0, 0, width, height);
  
  _settingsView->setName(thisName + Layout::kSettingsComponentName);
//  _synthKeyboardView->setName(thisName + Layout::kSynthComponentName);
  _modularNodeGraph->setName(thisName + Layout::kModularGraphName);
  
//  _tabComponent->addTab(Layout::kSynthTabName,
//                       Palette::TabActiveColor,
//                       _synthKeyboardView.get(),
//                       false,
//                       Layout::kSynthKeyboardTabIndex);
  _tabComponent->addTab(Layout::kModularGraphTabName,
                       Palette::TabActiveColor,
                       _modularNodeGraph.get(),
                       false,
                       Layout::kModularNodeGraphTabIndex);
  _tabComponent->addTab(Layout::kSettingsTabName,
                       Palette::TabActiveColor,
                       _settingsView.get(),
                       false,
                       Layout::kAudioSettingsTabIndex);
  _tabComponent->getTabbedButtonBar().getTabButton(Layout::kSynthKeyboardTabIndex)->setEnabled(true);
  // _tabComponent->setTabBarDepth(_tabComponent->getChildren().size() - 1);
  _tabComponent->toFront(false);
  _tabComponent->setVisible(true);
  
}



void MainComponent::resized() {
  debugChildren("resized");
  auto r = getBounds().reduced(4);
  constrainBounds<int32>(r);
  floatBounds = getBounds().toFloat();
  try {
    _tabComponent->setBoundsToFit(r, Justification::centred, false);
  } catch (std::exception &e) {
    _tabComponent->setBounds(Layout::DefaultRect());
    Logger::writeToLog("MainComponent: " + (string)e.what());
  }
}



void MainComponent::mouseMove(const MouseEvent& event) {
   _mouseListener->mouseMove(event);
}

void MainComponent::mouseUp(const MouseEvent& event) {
  _mouseListener->mouseUp(event);
};

void MainComponent::mouseDown(const MouseEvent &event) {
  _mouseListener->mouseDown(event);
}


template<class T>
void MainComponent::constrainBounds(Rectangle<T> &bounds) {
  const auto maxPosition = static_cast<T>(kMaxPosition);
  Point<T> boundsPosition = bounds.getPosition();
  T bPosX = boundsPosition.getX();
  T bPosY = boundsPosition.getY();

  if (bounds.getWidth() > maxWidth<T>())
    bounds.setWidth(maxWidth<T>());
  if (bounds.getHeight() > maxHeight<T>())
    bounds.setHeight(maxHeight<T>());
  if (std::abs(bPosX) > maxPosition)
    boundsPosition.setX(bPosX >= 0 ? maxPosition : -maxPosition);
  if (std::abs(bPosY) > maxPosition)
    boundsPosition.setY(bPosY >= 0 ? maxPosition : -maxPosition);
}

void MainComponent::debugChildren(const string &title) {
//  cout << "\n\nDebug Child Components: '" << title << "'" << endl;
//  debugChildComponent(_tabComponent, "_tabComponent");
//  debugChildComponent(_audioSettingsView, "_audioSettingsView");
//  debugChildComponent(_synthKeyboardView, "_synthKeyboardView");
}

void MainComponent::debugChildComponent(const Component &comp, const std::string &compName) {
  cout << "Component Debug:" << endl
       << "expected name = '" << compName << "', "
       << "actual name = '" << comp.getName() << "' " << endl
       << "isEnabled = " << comp.isEnabled() << endl
       << "isVisible = " << comp.isVisible() << endl
       << "width = " << comp.getWidth() << endl
       << "height = " << comp.getHeight()
       << endl;
  string compType = typeid(comp).name();
  if(compType == "modularsynth::AudioSettingsComponent" || compType == "modularsynth::SynthKeyboardComponent") {
    cout << "heloo component. name = " << compType << endl;
  }
}

void MainComponent::changeListenerCallback(ChangeBroadcaster *) {}



}




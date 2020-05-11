#include <typeinfo>
#include <string>
#include <array>
#include <nameof.hpp>

#include "MainComponent.h"

namespace modularsynth {
using namespace std;
using namespace config;

MainComponent::MainComponent(const string &name)
: Component(),
  _audioDeviceManager(make_shared<AudioDeviceManager>()),
  _userSettings(make_shared<UserSettings>(_audioDeviceManager))
{
  string newName = name + "_" + getComponentID().toStdString();
  this->setName(newName);
  look->setColourScheme(Palette::colorScheme);
  auto tabHeight = getParentHeight() - Layout::TabHeight;
  auto width = getParentWidth();
  cout << "window dimensions: (" << width << ", " << tabHeight << ")" << endl;
  setOpaque(true);
  addMouseListener(_mouseListener.get(), false);
  setRepaintsOnMouseActivity(true);
  setLookAndFeel(new LookAndFeel_V4(config::Palette::colorScheme));
  
//  _userSettings = make_shared<UserSettings>(_audioDeviceManager);
  const auto audioSettings = _userSettings->audioDeviceSetup;

  audioProcessorConfig();
  instantiateChildren(width, tabHeight);

  // Very important: this must come after instantiateChildren().
  _audioDeviceManager->setAudioDeviceSetup(audioSettings, true); 
  
  setSize(kDefaultWidth, kDefaultHeight);
}

MainComponent::~MainComponent() {
  removeAllChildren();
}

void MainComponent::paint(Graphics& g) {
//  g.fillAll(getUIColourIfAvailable(CS::UIColour::windowBackground));
  g.fillAll(config::Palette::WindowBackground);
}

void MainComponent::instantiateChildren(int32 width, int32 height) {
  auto thisName = this->getName();
  
  _settingsView = make_shared<SettingsView>(_userSettings, _audioDeviceManager, _mouseListener);
  
  _tabComponent = make_shared<TabbedComponent>(TabbedButtonBar::Orientation::TabsAtTop);
  
  addAndMakeVisible(*_tabComponent);
  _tabComponent->addMouseListener(_mouseListener.get(), true);
  _modularComponent = make_shared<ModularComponent>(_audioDeviceManager, _mouseListener);
  
  // Initialize Values for each child.
  _tabComponent->setName(thisName + Layout::kTabComponentName);
  _tabComponent->setBounds(0, 0, width, height);
  
  _settingsView->setName(thisName + Layout::kSettingsComponentName);
  _modularComponent->setName(thisName + Layout::kModularGraphName);
  
  _tabComponent->addTab(
    Layout::kModularGraphTabName,
    Palette::TabActiveColor,
    _modularComponent.get(),
    false,
    Layout::kModularGraphTabIndex);
  _tabComponent->addTab(
    Layout::kSettingsTabName,
    Palette::TabActiveColor,
    _settingsView.get(),
    false,
    Layout::kettingsTabIndex);
  
  _tabComponent->getTabbedButtonBar().getTabButton(Layout::kModularGraphTabIndex)->setEnabled(true);
  _tabComponent->toFront(false);
  _tabComponent->setVisible(true);
}



void MainComponent::resized() {
  auto r = getLocalBounds();
  // constrainBounds<int32>(r);
  _tabComponent->setBounds(r);
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

void MainComponent::audioProcessorConfig() {
  auto inputDevice  = MidiInput::getDevices()  [MidiInput::getDefaultDeviceIndex()];
  auto outputDevice = MidiOutput::getDevices() [MidiOutput::getDefaultDeviceIndex()];
  cout << "MidiInputDevice = " << inputDevice << " MidiOutputDevice = " << outputDevice << endl;
  _audioDeviceManager->initialiseWithDefaultDevices(2, 2);
  // _audioDeviceManager->addAudioCallback(this);
  _audioDeviceManager->setMidiInputEnabled(inputDevice, true);
  // _audioDeviceManager->addMidiInputCallback(inputDevice, this);
  _audioDeviceManager->setDefaultMidiOutput(outputDevice);
}



}




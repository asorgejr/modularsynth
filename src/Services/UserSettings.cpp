//
// Created by asorgejr on 4/27/2020.
//
#include <nameof.hpp>
#include "UserSettings.h"

using namespace std;

namespace modularsynth {

using Loc = File::SpecialLocationType;

UserSettings::UserSettings(const sptr<AudioDeviceManager> &audioDeviceManager) {
  _audioDeviceManager = audioDeviceManager;
  getSettingsFile();
  pushAudioDeviceSetup();
  audioDeviceListener = make_shared<UserSettings::AudioDeviceListener>(this);
}

UserSettings::~UserSettings() {
  _audioDeviceManager->removeChangeListener(audioDeviceListener.get());
}

void UserSettings::getSettingsFile() {
  File appDir = getAppDirectoryPath();
  
  if (appDir.exists()) {
    auto settingsFile = File(getAppDirectoryPath() + File::getSeparatorString() + kSettingsFileName);
    
    if (settingsFile.exists()) {
      _settingsFile = settingsFile;
      readSettings();
      return;
    }
    settingsFile.create();
    _settingsFile = settingsFile;
    writeSettings();
    return;
  }
  initializeAppSettings();
}

void UserSettings::initializeAppSettings() {
  File appDir = getAppDirectoryPath();
  appDir.createDirectory();
  auto settingsFile = File(appDir.addTrailingSeparator(kSettingsFileName));
  settingsFile.create();
  writeSettings();
}

void UserSettings::readSettings() {
  String data = _settingsFile.createInputStream()->readEntireStreamAsString();
  var result = JSON::parse(data);
  
  audioDeviceSetup.outputDeviceName = result.hasProperty("outputDeviceName")
    ? result.getProperty("outputDeviceName", audioDeviceSetup.outputDeviceName).toString()
    : audioDeviceSetup.outputDeviceName;
  
  audioDeviceSetup.inputDeviceName = result.hasProperty("inputDeviceName")
    ? result.getProperty("inputDeviceName", audioDeviceSetup.inputDeviceName).toString()
    : audioDeviceSetup.inputDeviceName;
  
  audioDeviceSetup.sampleRate = result.hasProperty("sampleRate")
    ? (double) result.getProperty("sampleRate", audioDeviceSetup.sampleRate)
    : audioDeviceSetup.sampleRate;
  
  audioDeviceSetup.bufferSize = result.hasProperty("bufferSize")
    ? (int) result.getProperty("bufferSize", audioDeviceSetup.bufferSize)
    : audioDeviceSetup.bufferSize;
  
  audioDeviceSetup.inputChannels = result.hasProperty("inputChannels")
    ? (BigInteger)((int) result.getProperty("inputChannels", audioDeviceSetup.inputChannels.toInteger()))
    : audioDeviceSetup.inputChannels.toInteger();
  
  audioDeviceSetup.outputChannels = result.hasProperty("outputChannels")
    ? (BigInteger)((int) result.getProperty("outputChannels", audioDeviceSetup.outputChannels.toInteger()))
    : audioDeviceSetup.outputChannels.toInteger();
  
  audioDeviceSetup.useDefaultInputChannels = result.hasProperty("useDefaultInputChannels")
    ? (bool) result.getProperty("useDefaultInputChannels", audioDeviceSetup.useDefaultInputChannels)
    : audioDeviceSetup.useDefaultInputChannels;
  
  audioDeviceSetup.useDefaultOutputChannels = result.hasProperty("useDefaultOutputChannels")
    ? (bool) result.getProperty("useDefaultOutputChannels", audioDeviceSetup.useDefaultOutputChannels)
    : audioDeviceSetup.useDefaultOutputChannels;
}

void UserSettings::writeSettings() {
  try {
    var obj (new DynamicObject());
    obj.getDynamicObject()->setProperty("outputDeviceName", audioDeviceSetup.outputDeviceName);
    obj.getDynamicObject()->setProperty("inputDeviceName", audioDeviceSetup.inputDeviceName);
    obj.getDynamicObject()->setProperty("sampleRate", audioDeviceSetup.sampleRate);
    obj.getDynamicObject()->setProperty("bufferSize", audioDeviceSetup.bufferSize);
    obj.getDynamicObject()->setProperty("inputChannels", audioDeviceSetup.inputChannels.toInteger());
    obj.getDynamicObject()->setProperty("outputChannels", audioDeviceSetup.outputChannels.toInteger());
    obj.getDynamicObject()->setProperty("useDefaultInputChannels", audioDeviceSetup.useDefaultInputChannels);
    obj.getDynamicObject()->setProperty("useDefaultOutputChannels", audioDeviceSetup.useDefaultOutputChannels);

    _settingsFile.replaceWithData("", 0);
    OutputStream *stream = _settingsFile.createOutputStream(sizeof(UserSettings));
    JSON::writeToStream(*stream, obj);
    
    delete stream;
  } catch (exception &e) {
    cout << "UserSettings::writeSettings error: " << e.what() << endl;
    LOG("UserSettings exception: Could not write to settings file.");
  }
}


void UserSettings::pushAudioDeviceSetup() {
  if (audioDeviceSetup.outputDeviceName == (String) "") return;
  _audioDeviceManager->setAudioDeviceSetup(audioDeviceSetup, true);
  _audioDeviceManager->sendChangeMessage();
}

void UserSettings::pullAudioDeviceSetup() {
  audioDeviceSetup = _audioDeviceManager->getAudioDeviceSetup();
}

#pragma region AudioDeviceListener

UserSettings::AudioDeviceListener::AudioDeviceListener(UserSettings *userSettings)
: ChangeListener(),
  userSettings(userSettings) 
{
  userSettings->_audioDeviceManager->addChangeListener(this);
}

void UserSettings::AudioDeviceListener
::changeListenerCallback(ChangeBroadcaster *source) {
  userSettings->pullAudioDeviceSetup();
  userSettings->writeSettings();
}

#pragma endregion AudioDeviceListener
  
}
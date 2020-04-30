//
// Created by asorgejr on 4/27/2020.
//

#pragma once
#include <JuceHeader.h>
#include "../config.h"
#include "../common.h"
#include "../Models/FunctionListener.h"

namespace modularsynth {
typedef AudioDeviceManager::AudioDeviceSetup AudioDeviceSetup;

class UserSettings {
  
#pragma region Public Members
  
public:
  
  const String kSettingsFileName = "settings.json";

#pragma region Save Data
  
  AudioDeviceSetup audioDeviceSetup { AudioDeviceSetup() };

#pragma endregion Save Data

#pragma endregion Public Members

#pragma region Private Members
  
private:
  
  File _settingsFile;
  
  sptr<AudioDeviceManager> _audioDeviceManager;

  sptr<FunctionListener> _audioDeviceListener;
  
#pragma endregion Private Members
  
#pragma region Public Methods
  
public:
  
  explicit UserSettings(const sptr<AudioDeviceManager> &audioDeviceManager);
  
  ~UserSettings();

  void readSettings();
  
  void writeSettings();
  
  static String getAppDirectoryPath() {
    auto path = File::getSpecialLocation(File::SpecialLocationType::userApplicationDataDirectory).getFullPathName();
    return path + File::getSeparatorString() + config::kApplicationName;
  }
  
#pragma endregion Public Methods

#pragma region Private Methods
  
private:

  void getSettingsFile();
  
  void pushAudioDeviceSetup();
  
  void pullAudioDeviceSetup();
  
  void initializeAppSettings();

#pragma endregion Private Methods
  
#pragma region AudioDeviceListener

public:
  
struct AudioDeviceListener : public ChangeListener {

    UserSettings *userSettings;
    
    explicit AudioDeviceListener(UserSettings *userSettings);

    ~AudioDeviceListener() override = default;

    void changeListenerCallback(ChangeBroadcaster *source) override;

  };

  sptr<AudioDeviceListener> audioDeviceListener;
  
#pragma endregion AudioDeviceListener

};

}
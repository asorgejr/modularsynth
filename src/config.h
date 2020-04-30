// config.h
// compile with: /LD /clr /doc

#pragma once

#include <JuceHeader.h>
#include <string>
#include <chrono>

#include "ModularAPI.h"
#include "config/Layout.h"
#include "config/Palette.h"

DISABLE_WARNING_PUSH
DISABLE_WARNING_LOSS_OF_DATA

namespace modularsynth::config {

/// The permanent name of the application.
const std::string kApplicationName = "ModularSynth";
/// The version of the application.
const std::string kApplicationVersion = "0.1.0";

const int32 samplesPerBlock = 4096;

const std::string kResourcesDirName = "Resources";
const std::string kAudioDirName = "audio";
const std::string kIconsDirName = "icons";
const std::string kImagesDirName = "images";



inline File getResourcesDirectory() noexcept {
  auto currentFile = File::getSpecialLocation(File::SpecialLocationType::currentApplicationFile);
  auto resourceDir = currentFile.getParentDirectory().getChildFile(kResourcesDirName);

  if (resourceDir.exists()) return resourceDir;

  int numTries = 0; // keep track of the number of parent directories so we don't go on endlessly
  auto resDirName = (String) kResourcesDirName;

  while (currentFile.getFileName() != resDirName
  && numTries++ < 15) currentFile = currentFile.getParentDirectory();

  return currentFile;
}

inline File getImagesDir() {
  auto ret = getResourcesDirectory().getChildFile(kImagesDirName);
  return ret;
}

inline File getAudioDir() {
  auto ret = getResourcesDirectory().getChildFile(kAudioDirName);
  return ret;
}

inline File getIconsDir() {
  auto ret = getResourcesDirectory().getChildFile(kIconsDirName);
  return ret;
}

static void Init() {
  Palette();
  Layout();
}

}

DISABLE_WARNING_POP
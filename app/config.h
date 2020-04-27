// config.h
// compile with: /LD /clr /doc

#pragma once

#include <JuceHeader.h>
#include <string>
#include <chrono>
#include <modularsynth/ModularAPI.h>
#include "config/Layout.h"
#include "config/Palette.h"

DISABLE_LOD_WARNINGS

namespace modularsynth::config {

/// The permanent name of the application.
const std::string kApplicationName = "ModularSynth";
/// The version of the application.
const std::string kApplicationVersion = "0.1.0";

const int32 samplesPerBlock = 4096;

static void Init() {
  Palette();
  Layout();
}

}
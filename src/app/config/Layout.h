//
// Created by asorgejr on 4/21/2020.
//

#pragma once
#include <JuceHeader.h>

namespace modularsynth::config {

/// The default height of the window.
const juce::int32 kDefaultHeight = 720;
/// The default width of the window.
const int32 kDefaultWidth = 1200;
/// The maximum height a window can be.
const int32 kMaxHeight = 10000;
/// The minimum height a window can be.
const int32 kMinHeight = 480;
/// The maximum width a window can be.
const int32 kMaxWidth = 10000;
/// The minimum width a window can be.
const int32 kMinWidth = 600;

const bool kResizable = true;

template<typename TValue>
static constexpr TValue maxWidth() { return static_cast<TValue>(kMaxWidth); }

template<typename TValue>
static constexpr TValue maxHeight() { return static_cast<TValue>(kMaxHeight); }



/// View Layout Defaults <br>
/// <p>This class keeps values of all default layout options for ModularSynth.</p>
class Layout {
public:
  /// The default orientation of the tab button bar.

private:
  inline static Rectangle<int32> _defaultRect
    = Rectangle<int32>(0, 0, kDefaultWidth, kDefaultHeight);
};

}
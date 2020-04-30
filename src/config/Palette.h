//
// Created by asorgejr on 4/21/2020.
//

#pragma once
#include <JuceHeader.h>
#include <nodesystem/GraphViewTheme.h>
#include "../config.h"

namespace modularsynth::config {

/// Palette | Color Palette Defaults <br>
/// <p>This class keeps static values of all default colors and shades for ModularSynth.</p>
class Palette {
  typedef LookAndFeel_V4::ColourScheme::UIColour UIColor;

public:
  inline static Colour const TabActiveColor = Colour::fromFloatRGBA(0.5f, 0.5f, 0.5f, 1.f);
  inline static Colour const TabInactiveColor = Colour::fromFloatRGBA(0.3f, 0.3f, 0.3f, 1.f);
  inline static Colour const DefaultFill = Colours::slategrey;
  inline static Colour const DefaultText = Colours::ivory;
  inline static Colour const HighlightedFill = DefaultFill;
  inline static Colour const HighlightedText = Colours::steelblue;
  inline static Colour const MenuBackground = Colour(0xFF242424);
  inline static Colour const MenuText = DefaultText;
  inline static Colour const NumColors = DefaultText;
  inline static Colour const Outline = Colours::whitesmoke;
  inline static Colour const WidgetBackground = MenuBackground;
  inline static Colour const WindowBackground = MenuBackground;
  inline static LookAndFeel_V4::ColourScheme colorScheme = LookAndFeel_V4::getDarkColourScheme();
  inline static nodesystem::GraphViewTheme GraphViewTheme = {
    .edgeStrokeWidth = 3.0f,
    .hostComponentDragAreaHeight = 30,
    .nodeHeight = 40,
    .pinHeight = 7,
    .pinSpacing = 10,
    .pinWidth = 10,
    .cBackground = 0xFF242424,
    .cEdge = 0xFF767676,
    .cEdgeSelected = 0xFFE27E2A,
    .cNodeBackground = 0xCF4F4F4F,
    .cNodeBackgroundHover = 0xCF656464,
    .cNodeBackgroundSelected = 0xCFDE4F18,
    .cPinBackground = 0xFF767676,
    .cPinBackgroundSelected = 0xFFE27E2A,
    .cSelectionBackground = 0x3D555555
  };

  inline static void SetColorScheme() {
    colorScheme.setUIColour(UIColor::defaultFill, DefaultFill);
    colorScheme.setUIColour(UIColor::defaultText, DefaultText);
    colorScheme.setUIColour(UIColor::highlightedFill, HighlightedFill);
    colorScheme.setUIColour(UIColor::highlightedText, HighlightedText);
    colorScheme.setUIColour(UIColor::menuBackground, MenuBackground);
    colorScheme.setUIColour(UIColor::menuText, MenuText);
    // colorScheme.setUIColour(UIColor::numColours, NumColors);
    colorScheme.setUIColour(UIColor::outline, Outline);
    colorScheme.setUIColour(UIColor::widgetBackground, WidgetBackground);
    colorScheme.setUIColour(UIColor::windowBackground, WindowBackground);
  }


  // File and image resources set in constructor 
  inline static File CWD = File();
  inline static File ResourcesDir = File();

  Palette() { // Must be initialized within a JUCE application constructor.
    CWD = File::getCurrentWorkingDirectory();
    ResourcesDir = File(CWD.getFullPathName() + "/Assets");
    SetColorScheme();
  }

};

}
//
// Created by asorgejr on 4/22/2020.
//
#pragma once
#include <JuceHeader.h>
#include <nodesystem/nodesystem.h>
#include "../NodeInfo.h"

namespace nodesystem {

struct SliderPanel : public Shareable<SliderPanel>, public Component,
  public GraphNodeEditor, public Slider::Listener {
  
  SliderPanel();

  ~SliderPanel() override;

  void paint(Graphics &g) override;

  void resized() override;

  Graph::Node *model;

  Slider slider;

  Component *guest() override;

  void setModel(Graph::Node *model) override;

  void onData(const Graph::Node *sourceNode, const Graph::Pin *sourcePin, const var &data) override;

  void sliderValueChanged(Slider *slider) override;

private:

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SliderPanel)

};



struct TextPanel : public Shareable<TextPanel>, public Component, public GraphNodeEditor {
  TextPanel();

  void paint(Graphics &g) override;

  Graph::Node *model;

  String text = "I am a hosted component";


  Component *guest() override;

  void setModel(Graph::Node *model) override;

  void onData(const Graph::Node *sourceNode, const Graph::Pin *sourcePin, const var &data) override;

private:

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TextPanel)

};


}
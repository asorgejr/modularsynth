//
// Created by asorgejr on 4/22/2020.
//

#include "NodePanel.h"


namespace nodesystem {

#pragma region SliderPanel

SliderPanel::SliderPanel() : slider(Slider::Rotary, Slider::TextBoxBelow) {
  slider.addListener(this);
  addAndMakeVisible(slider);
}

SliderPanel::~SliderPanel() {
  slider.removeListener(this);
}

void SliderPanel::paint(Graphics &g) {
  g.fillAll(Colours::white);
}

void SliderPanel::resized() {
  auto bounds = getLocalBounds();
  auto width = bounds.getWidth() * 0.7f;
  auto height = bounds.getHeight() * 0.7f;
  slider.setRange(1.0f, 16.0f, 1.0f);
  slider.centreWithSize((int32)width, (int32)height);
}

Component *SliderPanel::guest() { return this; }

void SliderPanel::setModel(Graph::Node *model) { this->model = model; }

void SliderPanel::onData(const Graph::Node *sourceNode, const Graph::Pin *sourcePin, const var &data) {
  slider.setValue(data);
}

void SliderPanel::sliderValueChanged(Slider *slider) {
  if (model) {
    model->publish(slider->getValue());
  }
}

#pragma endregion SliderPanel


#pragma region TextPanel

TextPanel::TextPanel() {}

void TextPanel::paint(Graphics &g) {
  g.fillAll(Colours::white);
  g.setColour(Colours::black);
  g.drawText(text, getLocalBounds(), Justification::centred, true);
}

void TextPanel::onData(const Graph::Node *sourceNode, const Graph::Pin *sourcePin, const var &data) {
  text = data.toString();
  repaint();
}

Component* TextPanel::guest() { return this; }

void TextPanel::setModel(Graph::Node *model) {
  this->model = model;
}

#pragma endregion TextPanel
}
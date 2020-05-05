//
// Created by asorgejr on 4/22/2020.
//
#pragma once
#include <JuceHeader.h>
#include <nodesystem/nodesystem.h>

namespace nodesystem {

struct SliderPanel : public Component, public NodeViewComponent, public Slider::Listener {

  Graph::Node *model;

  Slider slider;
  
  SliderPanel() 
  : slider(Slider::Rotary, Slider::TextBoxBelow)
  {
    slider.addListener(this);
    addAndMakeVisible(slider);
  }

  ~SliderPanel() override  {
    slider.removeListener(this);
  }

  void paint(Graphics &g) override {
    g.fillAll(Colours::white);
  }

  void resized() override {
    auto bounds = getLocalBounds();
    auto width = bounds.getWidth() * 0.7f;
    auto height = bounds.getHeight() * 0.7f;
    slider.setRange(1.0f, 16.0f, 1.0f);
    slider.centreWithSize((int32)width, (int32)height);
  }

  Component *guest() override { return this; }

  void setModel(Graph::Node *model) override { this->model = model; }

  void onData(const Graph::Node *sourceNode, const Graph::Pin *sourcePin, const var &data) override {
    slider.setValue(data);
  }

  void sliderValueChanged(Slider *slider) override {
    if (model) {
      model->publish(slider->getValue());
    }
  }

  HostNodeDefinition getNodeDefinition() override {
    return HostNodeDefinition("Slider", 1, 1, 150, 150,
      [&](const sptr<GraphViewComponent> &host, const Point<float> &pos) -> NodeComponent* {
        return host->addHostNode(std::make_unique<SliderPanel>(), pos);
      });
  }

private:

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SliderPanel)

};



struct TextPanel : public Component, public NodeViewComponent {
  
  Graph::Node *model {};

  String text = "I am a hosted component";
  
  TextPanel() {}

  void paint(Graphics &g) override {
    g.fillAll(Colours::white);
    g.setColour(Colours::black);
    g.drawText(text, getLocalBounds(), Justification::centred, true);
  }
  
  Component *guest() override { return this; }

  void setModel(Graph::Node *model) override {
    this->model = model;
  }

  void onData(const Graph::Node *sourceNode, const Graph::Pin *sourcePin, const var &data) override {
    text = data.toString();
    repaint();
  }

  HostNodeDefinition getNodeDefinition() override {
    return HostNodeDefinition("Text", 1, 0, 200, 100,
      [](const sptr<GraphViewComponent> &host, const Point<float> &pos) -> NodeComponent* {
        return host->addHostNode(std::make_unique<TextPanel>(), pos);
    });
  }

private:

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TextPanel)

};


}
#pragma once

#include "../include/nodesystem/Graph.h"
#include "../include/nodesystem/GraphViewComponent.h"


struct TextPanel : public Component, public GraphNodeEditor {

  Graph::Node* model;

  String text = "I am a hosted component";

  TextPanel() { }

  void paint(Graphics& g) override {
    g.fillAll(Colours::white);
    g.setColour(Colours::black);
    g.drawText(text, getLocalBounds(), Justification::centred, true);
  }


  Component* guest() override {
    return this;
  }

  void setModel(Graph::Node* model) override {
    this->model = model;
  }

  void onData(const Graph::Node* sourceNode, const Graph::Pin* sourcePin, const var& data) override {
    text = data.toString();
    repaint();
  }

private:

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TextPanel)

};


struct SliderPanel : public Component, public GraphNodeEditor, public Slider::Listener {

  Graph::Node* model;

  Slider slider;

  SliderPanel() : slider(Slider::Rotary, Slider::TextBoxBelow) {
    slider.addListener(this);
    addAndMakeVisible(slider);
  }

  ~SliderPanel() {
    slider.removeListener(this);
  }

  void paint(Graphics& g) override {
    g.fillAll(Colours::white);
  }


  void resized() override {
    auto bounds = getLocalBounds();
    slider.setRange(1.0f, 16.0f, 1.0f);
    slider.centreWithSize(bounds.getWidth() * 0.7f, bounds.getWidth() * 0.7f);
  }

  Component* guest() override {
    return this;
  }

  void setModel(Graph::Node* model) override {
    this->model = model;
  }

  void onData(const Graph::Node* sourceNode, const Graph::Pin* sourcePin, const var& data) override {
    slider.setValue(data);
  }

  void sliderValueChanged(Slider* slider) override {
    if (model) {
      model->publish(slider->getValue());
    }
  }

private:

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SliderPanel)


};


struct OutListener : public Graph::NodeListener {

  Graph::Node* outsideWorld;

  void onData(const Graph::Node* sourceNode, const Graph::Pin* sourcePin, const var& data) {
    if (outsideWorld) outsideWorld->publish(data);
  }
};


struct EmbeddedGraphView : public GraphNodeEditor {

  struct View : public GraphViewComponent {


    View() {
      theme.initialScaleFactor = 0.7f;
    }

    ~View() {
      //printf("nodes: %lu, edges:%lu\n", nodes.size(), edges.size());
    }
    void popupMenu(const MouseEvent& e) override {
      PopupMenu m;
      auto position = e.getMouseDownPosition().toFloat();
      m.addItem(1, "1 x 1");
      m.addItem(2, "2 x 2");
      m.addItem(3, "4 x 4");
      m.addItem(4, "Slider");
      m.addItem(5, "Text");

      auto selection = m.show();

      auto sliderPanel = [&]() {
        addHostNode(std::make_unique<SliderPanel>(), 1, 1, 150, 150, position);
      };

      auto textPanel = [&]() {
        addHostNode(std::make_unique<TextPanel>(), 1, 0, 200, 100, position);
      };


      switch (selection) {
        case 1:
          addNode("1 x 1", 1, 1, position);
          break;
        case 2:
          addNode("2 x 2", 2, 2, position);
          break;
        case 3:
          addNode("4 x 4", 4, 4, position);
          break;
        case 4:
          sliderPanel();
          break;
        case 5:
          textPanel();
          break;

        default:
          break;
      }
    }
  };



  std::unique_ptr<View> view;
  std::unique_ptr<OutListener> listener;
  NodeComponent* in;
  NodeComponent* out;

  EmbeddedGraphView() {
    view = std::make_unique<View>();
    view->theme.cBackground = 0xFF111111;
    in = view->addNode("in", 0, 1, Point<float>(50.0f, 50.0f));
    out = view->addNode("out", 1, 0, Point<float>(50.0f, 100.0f));

    listener = std::make_unique<OutListener>();
    out->model->addListener(listener.get());
  }

  ~EmbeddedGraphView() {
    out->model->removeListener(listener.get());
  }


  Component* guest() override {
    return view.get();
  }

  void setModel(Graph::Node* model) override {
    listener->outsideWorld = model;
  }

  void onData(const Graph::Node* sourceNode, const Graph::Pin* sourcePin, const var& data) override {
    in->model->publish(data);
  }


};

struct CustomGraphView : public GraphViewComponent {

  void popupMenu(const MouseEvent& e) override {
    PopupMenu m;
    auto position = e.getMouseDownPosition().toFloat();
    m.addItem(1, "1 x 1");
    m.addItem(2, "2 x 2");
    m.addItem(3, "4 x 4");
    m.addItem(4, "Slider");
    m.addItem(5, "Text");
    m.addItem(6, "Graph");

    auto selection = m.show();

    auto sliderPanel = [&]() {
      addHostNode(std::make_unique<SliderPanel>(), 1, 1, 150, 150, position);
    };

    auto textPanel = [&]() {
      addHostNode(std::make_unique<TextPanel>(), 1, 0, 200, 100, position);
    };

    auto embeddedGraphView = [&]() {
      addHostNode(std::make_unique<EmbeddedGraphView>(), 1, 1, 400, 400, position);
    };


    switch (selection) {
      case 1:
        addNode("1 x 1", 1, 1, position);
        break;
      case 2:
        addNode("2 x 2", 2, 2, position);
        break;
      case 3:
        addNode("4 x 4", 4, 4, position);
        break;
      case 4:
        sliderPanel();
        break;
      case 5:
        textPanel();
        break;
      case 6:
        embeddedGraphView();
        break;
      default:
        break;
    }
  }
};

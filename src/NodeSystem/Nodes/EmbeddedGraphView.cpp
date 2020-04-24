//
// Created by asorgejr on 4/22/2020.
//

#include "EmbeddedGraphView.h"

using namespace std;

namespace nodesystem {

EmbeddedGraphView::EmbeddedGraphView() {
  view = std::make_unique<View>();
  view->theme.cBackground = 0xFF111111;
  in = view->addNode("in", 0, 1, Point<float>(50.0f, 50.0f));
  out = view->addNode("out", 1, 0, Point<float>(50.0f, 100.0f));

  listener = std::make_unique<OutListener>();
  out->model->addListener(listener.get());
}

EmbeddedGraphView::~EmbeddedGraphView() {
  out->model->removeListener(listener.get());
}

Component *EmbeddedGraphView::guest() { return view.get(); }

void EmbeddedGraphView::setModel(Graph::Node *model) { listener->outsideWorld = model; }

void EmbeddedGraphView::onData(const Graph::Node *sourceNode, const Graph::Pin *sourcePin, const var &data) {
  in->model->publish(data);
}

EmbeddedGraphView::View::View() {
  theme.initialScaleFactor = 0.7f;
}

EmbeddedGraphView::View::~View() {
  //printf("nodes: %lu, wires:%lu\n", nodes.size(), wires.size());
}

void EmbeddedGraphView::View::popupMenu(const MouseEvent &e) {
  PopupMenu m;
  auto position = e.getMouseDownPosition().toFloat();
  for (int32 i = 0; i < NodeInfo::NodeDefinitions.size(); i++) {
    m.addItem(i + 1, NodeInfo::NodeDefinitions[i].name);
  }
//  m.addItem(1, "1 x 1");
//  m.addItem(2, "2 x 2");
//  m.addItem(3, "4 x 4");
//  m.addItem(4, "Slider");
//  m.addItem(5, "Text");

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

}
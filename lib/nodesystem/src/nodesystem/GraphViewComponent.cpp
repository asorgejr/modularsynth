#include "../../include/nodesystem/GraphViewComponent.h"

using namespace std;

namespace nodesystem {

GraphViewComponent::GraphViewComponent() {
  graph = make_unique<Graph>();
  selector = make_unique<SelectionComponent>(theme.cSelectionBackground);
  wireDrawer = make_unique<UnboundWireComponent>();
  mouseListener = make_unique<ChildrenMouseListener>(this);
  setWantsKeyboardFocus(true);
}

GraphViewComponent::~GraphViewComponent() {};

void GraphViewComponent::paint(Graphics &g) {
  g.fillAll(Colour(theme.cBackground));
}

void GraphViewComponent::resized() {}


NodeComponent *GraphViewComponent::addNode(const string &name, const int ins, const int outs, const Point<float> position) {
  auto maxNumPins = max(ins, outs);
  auto w = (maxNumPins * theme.pinWidth) + ((maxNumPins + 1) * theme.pinSpacing);

  auto model = graph->addNode(name, ins, outs);

  auto node = std::make_unique<NodeComponent>(theme, model);
  node->setBounds(0, 0, w, theme.nodeHeight);
  node->translation = AffineTransform::translation(position);
  node->scale = AffineTransform::scale(theme.initialScaleFactor);
  node->setTransform(node->translation.followedBy(node->scale));
  node->addMouseListener(mouseListener.get(), true);
  addAndMakeVisible(node.get());

  auto ptr = node.get();
  nodes.push_back(move(node));

  assertions();

  return ptr;
}

HostNodeComponent 
*GraphViewComponent::addHostNode(unique_ptr<GraphNodeEditor> editor,
  const int ins, const int outs,
  const int width, const int height,
  const Point<float> position)
{
  const auto model = graph->addNode("GraphNodeEditor", ins, outs);
  auto node = make_unique<HostNodeComponent>(theme, model, move(editor));
  node->setBounds(0, 0, width, height);
  node->translation = AffineTransform::translation(position);
  node->scale = AffineTransform::scale(theme.initialScaleFactor);
  node->setTransform(node->translation.followedBy(node->scale));
  node->addMouseListener(mouseListener.get(), true);
  addAndMakeVisible(node.get());

  auto ptr = node.get();
  nodes.push_back(move(node));

  assertions();

  return ptr;
}


void GraphViewComponent::removeNode(NodeComponent *n) {
  n->removeMouseListener(mouseListener.get());
  // we need to do this instead of directly erasing because we have to remove the corresponding UI component as well
  vector<WireComponent*> wiresToDelete;
  for (auto &w : wires) {
    if (w.get()) {
      if ((w->startPin && w->startPin->node == n) || (w->endPin && w->endPin->node == n)) {
        wiresToDelete.push_back(w.get());
      }
    }
  }
  for (auto &w : wiresToDelete) {
    removeWire(w);
  }


  auto ref = remove_if(
    nodes.begin(), nodes.end(), [&](auto &current) -> bool { return current.get() == n; }
  );

  if (ref != nodes.end()) {
    auto n = (*ref).get();
    graph->removeNode(n->model);
    removeChildComponent(n);
    nodes.erase(ref, nodes.end()); // TODO: access violation
  }

  assertions();

}

WireComponent *GraphViewComponent::addWire(NodeComponent::PinComponent *source, NodeComponent::PinComponent *target) {
  const auto model = graph->addWire(source->model, target->model);

  auto wire = make_unique<WireComponent>(theme, source, target, model);

  calculateWireBounds(wire.get());
  wire->addMouseListener(mouseListener.get(), false);
  addAndMakeVisible(wire.get());
  wire->toBack();
  auto ptr = wire.get();
  wires.push_back(move(wire));

  assertions();

  return ptr;
}

void GraphViewComponent::removeWire(WireComponent *w) {
  w->removeMouseListener(mouseListener.get());
  auto ref = remove_if(wires.begin(), wires.end(),
     [&](auto &current) -> bool {
       return current.get() == w;
  });

  if (ref != wires.end()) {
    auto w = (*ref).get();
    // TODO: fix this. Throws Access Violation exception. 
    //  Make it so this method can detect when node is mid-stream with both in and out connections.
    graph->removeWire(w->model); 
    removeChildComponent(w);
    wires.erase(ref, end(wires));
  }
  assertions();
}

void GraphViewComponent::assertions() const {
  jassert(graph->wires.size() == wires.size());
  jassert(graph->nodes.size() == nodes.size());
}


void GraphViewComponent::pinMouseDrag(NodeComponent::PinComponent *pin, const MouseEvent &e) {
  auto relativeEvent = e.getEventRelativeTo(this);
  auto position = relativeEvent.getPosition();
  wireDrawer->startPin = pin;
  wireDrawer->endPin = nullptr;
  wireDrawer->currentEndPosition = position;
  wireDrawer->calculateBounds(getLocalPoint(pin, Point<int>(theme.pinWidth / 2, theme.pinHeight / 2)), position);
  addAndMakeVisible(wireDrawer.get());
}

void GraphViewComponent::pinMouseUp(NodeComponent::PinComponent *pin, const MouseEvent &e) {
  auto relativeEvent = e.getEventRelativeTo(this);
  auto position = relativeEvent.getPosition();

  wireDrawer->currentEndPosition = position;

  removeChildComponent(wireDrawer.get());

}

void GraphViewComponent::pinMouseEnter(NodeComponent::PinComponent *pin, const MouseEvent &e) {
  auto relativeEvent = e.getEventRelativeTo(this);
  auto position = relativeEvent.getPosition();


  if (wireDrawer->currentEndPosition == position) {
    drawConnector(pin);
  }

}

void GraphViewComponent::drawConnector(NodeComponent::PinComponent *pin) {
  if (wireDrawer->startPin && wireDrawer->startPin != pin) {

    wireDrawer->endPin = pin;

    if (isLegalWire(wireDrawer->startPin, wireDrawer->endPin)) {
      addWire(wireDrawer->startPin, wireDrawer->endPin);
    }

  }
}

bool GraphViewComponent::isLegalWire(NodeComponent::PinComponent *start, NodeComponent::PinComponent *end) {
  auto existing = find_if(wires.begin(), wires.end(), [&](auto &w) -> bool { return w->isConnecting(start, end); });
  return existing == wires.end() && start->model->pinType != end->model->pinType && start->node != end->node;
}

void GraphViewComponent::nodeMouseDrag(NodeComponent *node, const MouseEvent &e) {
  node->selected = true;
  node->repaint();

  if (!e.mods.isShiftDown() && !nodeMultiSelectionOn) {
    for (auto &n : nodes) {
      if (n.get() != node) {
        n->selected = false;
        n->repaint();
      }
    }
  }


  auto offset = e.getOffsetFromDragStart();
  node->setTransform(node->scale.followedBy(node->translation.translated(offset * node->scaleFactor)));

  for (auto &n : nodes) {
    if (n.get() == node || !(n->selected)) continue;

    // using node->scaleFactor here is not a mistake, we need to move according to the dragged node scale
    n->setTransform(n->scale.followedBy(n->translation.translated(offset * node->scaleFactor)));

  }

  for (auto &w : wires) {
    calculateWireBounds(w.get());
    w->repaint();
  }

}

void GraphViewComponent::nodeMouseDown(NodeComponent *node, const MouseEvent &e) {
  recordState();

  if (!e.mods.isShiftDown() && nodeMultiSelectionOn && !node->selected) {
    nodeMultiSelectionOn = false;
  }

  node->selected = !node->selected;
  node->repaint();

  if (!e.mods.isShiftDown() && !nodeMultiSelectionOn) {
    for (auto &n : nodes) {
      if (n.get() != node) {
        n->selected = false;
        n->repaint();
      }
    }

    ////
    for (auto &w : wires) {
      w->selected = false;
      w->repaint();
    }
    ////

  } else {
    nodeMultiSelectionOn = true;
  }

}

void GraphViewComponent::nodeMouseUp(NodeComponent *node, const MouseEvent &e) {
  recordState();
}

void GraphViewComponent::wireMouseDown(WireComponent *wire, const MouseEvent &e) {
  if (!e.mods.isShiftDown() && wireMultiSelectionOn && !wire->selected) {
    wireMultiSelectionOn = false;
  }

  wire->selected = !wire->selected;
  wire->repaint();

  if (!e.mods.isShiftDown() && !wireMultiSelectionOn) {

    for (auto &w : wires) {
      if (w.get() != wire) {
        w->selected = false;
        w->repaint();
      }
    }

    ////
    for (auto &n : nodes) {
      n->selected = false;
      n->repaint();
    }
    ////

  } else {
    wireMultiSelectionOn = true;
  }
}


void GraphViewComponent::calculateWireBounds(WireComponent *wire) {
  auto startPin = wire->startPin;
  auto endPin = wire->endPin;

  auto startPinTopLeft = getLocalPoint(startPin, Point<int>(0, 0));
  auto endPinTopLeft = getLocalPoint(endPin, Point<int>(0, 0));

  auto startPinBottomRight = getLocalPoint(startPin, Point<int>(startPin->getWidth(), startPin->getHeight()));
  auto endPinBottomRight = getLocalPoint(endPin, Point<int>(endPin->getWidth(), endPin->getHeight()));


  auto left = std::min(startPinTopLeft.x, endPinTopLeft.x);
  auto right = std::max(startPinBottomRight.x, endPinBottomRight.x);
  auto top = std::min(startPinTopLeft.y, endPinTopLeft.y);
  auto bottom = std::max(startPinBottomRight.y, endPinBottomRight.y);

  auto w = right - left;
  auto h = bottom - top;

  wire->inverted = !(left == startPinTopLeft.x);


  wire->setBounds(left, top, w, h);
}

void GraphViewComponent::wireMouseUp(WireComponent *wire, const MouseEvent &e) {}

void GraphViewComponent::recordState() {
  for (auto &n : nodes) {
    auto p = getLocalPoint(n.get(), Point<int>(0, 0));
    n->translation = AffineTransform::translation(p);
    n->scale = AffineTransform::scale(n->scaleFactor);
  }

}

void GraphViewComponent::mouseDown(const MouseEvent &e) {
  if (e.mods.isPopupMenu()) {
    popupMenu(e);
    return;
  }

  // node selection
  for (auto &n: nodes) {
    n->selected = false;
    n->repaint();
  }
  nodeMultiSelectionOn = false;

  // wire selection
  for (auto &w: wires) {
    w->selected = false;
    w->repaint();
  }
  wireMultiSelectionOn = false;

  // selector
  auto position = e.getMouseDownPosition();
  selector->setBounds(position.x, position.y, 10, 10);
  addAndMakeVisible(selector.get());
}

void GraphViewComponent::mouseDrag(const MouseEvent &e) {
  // draw a selection box
  auto position = e.getMouseDownPosition();
  auto offset = e.getOffsetFromDragStart();

  selector->calculateBounds(position, offset);

  // handle node selection
  auto numSelectedNodes = 0;
  for (auto &n : nodes) {
    auto selected = selector->getBounds().intersects(n->getBounds().transformedBy(n->getTransform()));
    n->selected = selected;
    n->repaint();
    if (selected) ++numSelectedNodes;
  }

  nodeMultiSelectionOn = numSelectedNodes > 0;


  // handle wire selection
  auto numSelectedWires = 0;
  for (auto &w : wires) {
    auto intersection = selector->getBounds().getIntersection(w->getBounds());
    auto selected = intersection == w->getBounds() || intersection.getWidth() >= w->getBounds().getWidth();
    w->selected = selected;
    w->repaint();
    if (selected) ++numSelectedWires;
  }

  wireMultiSelectionOn = numSelectedWires > 0;

}

void GraphViewComponent::mouseUp(const MouseEvent &e) {
  removeChildComponent(selector.get());
}


bool GraphViewComponent::keyPressed(const KeyPress &key) {
  auto code = key.getKeyCode();
  auto commandDown = key.getModifiers().isCommandDown();

  if (code == KeyPress::deleteKey || code == KeyPress::backspaceKey) {
    removeSelected();
  }

    // cmd '0'
  else if (code == 48 && commandDown) {
    zoomToOriginalSize();
  }

    // cmd '+' or cmd '='
  else if ((code == 43 || code == 61) && commandDown) {
    zoomIn();
  }

    // cmd '-'
  else if (code == 45 && commandDown) {
    zoomOut();
  }

    // cmd 'a'
  else if (code == 65 && commandDown) {
    selectAll();
  }

    // cmd 'd'
  else if (code == 68 && commandDown) {
    duplicate();
  }

  return true;
}

void GraphViewComponent::zoomIn() {
  recordState();
  for (auto &n : nodes) {
    if (n->selected && n->scaleFactor <= theme.scaleMax) {
      n->scaleFactor += theme.scaleStep;
      n->scale = AffineTransform::scale(n->scaleFactor);
      n->setTransform(n->scale.followedBy(n->translation));
    }
  }

  for (auto &w : wires) {
    calculateWireBounds(w.get());
    w->repaint();
  }
}

void GraphViewComponent::zoomOut() {
  recordState();
  for (auto &n : nodes) {
    if (n->selected && n->scaleFactor >= theme.scaleMin) {
      n->scaleFactor -= theme.scaleStep;
      n->scale = AffineTransform::scale(n->scaleFactor);
      n->setTransform(n->scale.followedBy(n->translation));
    }

  }

  for (auto &w : wires) {
    calculateWireBounds(w.get());
    w->repaint();
  }
}

void GraphViewComponent::zoomToOriginalSize() {
  recordState();
  for (auto &n : nodes) {
    if (n->selected) {
      n->scaleFactor = theme.initialScaleFactor;
      n->scale = AffineTransform::scale(n->scaleFactor);
      n->setTransform(n->scale.followedBy(n->translation));
    }
  }

  for (auto &w : wires) {
    calculateWireBounds(w.get());
    w->repaint();
  }
}

// TODO: usually throws a fat exception: Access Violation when calling removeNode()
void GraphViewComponent::removeSelected() {
  // remove wires first
  vector<WireComponent *> wiresToDelete;
  for (auto &w : wires) {
    if (w->selected) {
      wiresToDelete.push_back(w.get());
    }
  }
  for (auto &w : wiresToDelete) {
    removeWire(w);
  }
  nodeMultiSelectionOn = false;

  // then remove nodes
  vector<NodeComponent *> nodesToDelete;

  for (auto &n : nodes) {
    if (n->selected) {
      nodesToDelete.push_back(n.get());
    }
  }
  for (auto &n : nodesToDelete) removeNode(n);
  wireMultiSelectionOn = false;
}

void GraphViewComponent::selectAll() {
  for (auto &n : nodes) {
    n->selected = true;
  }
  nodeMultiSelectionOn = true;

  for (auto &w : wires) {
    w->selected = true;
  }
  wireMultiSelectionOn = true;

  repaint();
}

void GraphViewComponent::duplicate() {
  vector<tuple<NodeComponent *, int, int>> nodesToAdd;

  for (auto &n : nodes) {
    if (n->selected) {
      nodesToAdd.push_back(tuple<NodeComponent *, int, int>(n.get(), n->ins.size(), n->outs.size()));
    }
  }

  for (auto &t : nodesToAdd) {
    NodeComponent *n;
    int ins = 0, outs = 0;
    tie(n, ins, outs) = t;
    if (dynamic_cast<HostNodeComponent *>(n)) {
      printf("not supported for now\n");
    } else {
      addNode(n->model->name + "-copy", ins, outs);
    }
  }
}


GraphViewComponent::ChildrenMouseListener::ChildrenMouseListener(GraphViewComponent *view) : view(view) {}

void GraphViewComponent::ChildrenMouseListener::mouseDown(const MouseEvent &e) {
  if (auto node = dynamic_cast<NodeComponent *>(e.originalComponent)) {
    view->nodeMouseDown(node, e);
  } else if (auto wire = dynamic_cast<WireComponent *>(e.originalComponent)) {
    view->wireMouseDown(wire, e);
  }
}

void GraphViewComponent::ChildrenMouseListener::mouseUp(const MouseEvent &e) {
  if (auto node = dynamic_cast<NodeComponent *>(e.originalComponent)) {
    view->nodeMouseUp(node, e);
  } else if (auto pin = dynamic_cast<NodeComponent::PinComponent *>(e.originalComponent)) {
    view->pinMouseUp(pin, e);
  } else if (auto wire = dynamic_cast<WireComponent *>(e.originalComponent)) {
    view->wireMouseUp(wire, e);
  }
}

void GraphViewComponent::ChildrenMouseListener::mouseDrag(const MouseEvent &e) {
  if (auto node = dynamic_cast<NodeComponent *>(e.originalComponent)) {
    view->nodeMouseDrag(node, e);
  } else if (auto pin = dynamic_cast<NodeComponent::PinComponent *>(e.originalComponent)) {
    view->pinMouseDrag(pin, e);
  }
}

void GraphViewComponent::ChildrenMouseListener::mouseEnter(const MouseEvent &e) {
  if (auto pin = dynamic_cast<NodeComponent::PinComponent *>(e.originalComponent)) {
    view->pinMouseEnter(pin, e);
  }
}

}
//
// Created by asorgejr on 5/5/2020.
//

#include <algorithm>
#include "ModularGraphView.h"
#include "NodeSystem/NodeInfo.h"
#include "ModularAudioProcessor.h"

DISABLE_WARNING_PUSH
DISABLE_WARNING_LOSS_OF_DATA

using namespace std;


ModularGraphView::
ModularGraphView(ModularAudioProcessor *modularAudio, const sptr<Graph> &graph)
: AudioProcessorEditor(modularAudio),
  modularAudio(modularAudio)
{
  this->graph = graph;
  selector = make_unique<SelectionComponent>(theme.cSelectionBackground);
  wireDrawer = make_unique<UnboundWireComponent>();
  mouseListener = make_unique<ChildrenMouseListener>(this);
  setWantsKeyboardFocus(true);
  this->modularAudio->initialiseGraph();
}

ModularGraphView::~ModularGraphView() {
  
}

#pragma region UI Methods

void ModularGraphView::
popupMenu(const MouseEvent &e) {
  PopupMenu m;
  auto nodeDefinitions = NodeInfo::getNodeDefinitions();
  auto position = e.getMouseDownPosition().toFloat();
  for (auto i = 0; i < nodeDefinitions.size(); i++) {
    auto mI = i + 1;
    m.addItem(mI, nodeDefinitions[i].name);
  }
  auto selection = m.show();
  try {
    auto sI = selection >= 1 ? selection - 1 : 0;
    nodeDefinitions[sI].instantiate(this, position);
  } catch (exception &ex) {
    cerr << "A shared_ptr could not be created."
         << endl << ex.what() << endl;
  }

}

void ModularGraphView::
paint(Graphics &g) {
  g.fillAll(Colour(theme.cBackground));
}

void ModularGraphView::
resized() {}

#pragma endregion UI Methods

#pragma region Add Methods

NodeComponent * ModularGraphView::
addNode(const uptr<Module> &module, const Point<float> position) {
  onBeforeAddNode(module, position);
  auto def = module->getNodeDefinition();
  auto maxNumPins = max(def.ins, def.outs);
  auto w = (maxNumPins * theme.pinWidth) + ((maxNumPins + 1) * theme.pinSpacing);
  auto model = graph->addNode(def);

  auto node = std::make_unique<NodeComponent>(theme, model, def);

  node->setBounds(0, 0, w, theme.nodeHeight);
  node->translation = AffineTransform::translation(position);
  node->scale = AffineTransform::scale(theme.initialScaleFactor);
  node->setTransform(node->translation.followedBy(node->scale));
  node->addMouseListener(mouseListener.get(), true);
  addAndMakeVisible(node.get());

  auto ptr = node.get();
  nodes.push_back(move(node));

  onAfterAddNode(ptr, model);

  assertions();

  return ptr;
}

WireComponent * ModularGraphView::
addWire(NodeComponent::PinComponent *source, NodeComponent::PinComponent *target) {
  onBeforeAddWire(source, target);
  const auto model = graph->addWire(source->model, target->model);
  auto wire = make_unique<WireComponent>(theme, source, target, model);

  calculateWireBounds(wire.get());
  wire->addMouseListener(mouseListener.get(), false);
  addAndMakeVisible(wire.get());
  wire->toBack();
  auto ptr = wire.get();
  wires.push_back(move(wire));

  onAfterAddWire(source, target);

  assertions();
  return ptr;
}

void ModularGraphView::
onBeforeAddNode(const unique_ptr<Module> &view, const Point<float> &position) {

}

void ModularGraphView::
onAfterAddNode(const NodeComponent *node, const Graph::Node *model) {
  debugHeader("EVENT: ADD HOST NODE FINISHED");
  auto tempF = make_unique<AudioNodeGenerator>();
  if (NodeInfo::tryGetAudioNodeGenerator(model->name, tempF.get())) {
    auto gen = *tempF;
    auto anPtr = gen(modularAudio);
    audioNodeTether[node->model] = anPtr;
    cout << "new AudioNode: " << anPtr << endl;
  } else {
    cerr << "could not add AudioNode. Could not find '" << model->name << "'." << endl;
  }
  debugGraphCount();
}

void ModularGraphView::
onBeforeAddWire(const NodeComponent::PinComponent *source, const NodeComponent::PinComponent *target) {

}

void ModularGraphView::
onAfterAddWire(const NodeComponent::PinComponent *source, const NodeComponent::PinComponent *target) {
  debugHeader("EVENT: REMOVE WIRE FINISHED");
  // debugGraph();
  auto n0 = source->node->model;
  auto n1 = target->node->model;
  auto an0 = audioNodeTether[n0];
  auto an1 = audioNodeTether[n1];
  if (an0 == nullptr || an1 == nullptr) {
    cerr << "ModularGraphView error: AudioNode not initialized." << endl
         << "source node: " << an0 << endl << "target node: " << an1 << endl;
    return;
  }
  for (int i = 0; i < modularAudio->kChannelCount; i++) {
    modularAudio->mainProcessor->addConnection({
      {an0->nodeID, i},
      {an1->nodeID, i}
    });
  }
  debugAudioGraph();
  debugGraphCount(); // TODO remove this
}

#pragma endregion Add Methods

#pragma region Remove Methods

void ModularGraphView::
removeNode(NodeComponent *node) {
  onBeforeRemoveNode(node);
  node->removeMouseListener(mouseListener.get());
  // we need to do this instead of directly erasing because we have to remove the corresponding UI component as well
  vector<WireComponent *> wiresToDelete;
  for (auto &w : wires) {
    if ((w->startPin && w->startPin->node == node) || (w->endPin && w->endPin->node == node)) {
      wiresToDelete.push_back(w.get());
    }
  }
  for (auto &w : wiresToDelete) {
    removeWire(w);
    debugGraphCount(); // TODO remove this
  }
  
  int removeIndex = 0;
  for (auto &current : nodes) {
    auto n = current.get();
    if (n == node) {
      graph->removeNode(n->model);
      removeChildComponent(n);
      break;
    }
    removeIndex++;
    if (removeIndex == nodes.size()) removeIndex = -1;
  }

  if (removeIndex >= 0 && removeIndex < nodes.size())
    nodes.erase(nodes.begin() + removeIndex);
  
  onAfterRemoveNode();
  assertions();
}

void ModularGraphView::
removeWire(WireComponent *wire) {
  onBeforeRemoveWire(wire);
  wire->removeMouseListener(mouseListener.get());

  int removeIndex = 0;
  for (auto &current : wires) {
    if (current.get() == wire) {
      auto w = current.get();
      graph->removeWire(w->model);
      removeChildComponent(w);
      break;
    }
    removeIndex++;
    if (removeIndex == wires.size()) removeIndex = -1;
  }

  if (removeIndex >= 0 && removeIndex < wires.size())
    wires.erase(wires.begin() + removeIndex);

  onAfterRemoveWire();
  assertions();
}


void ModularGraphView::
onBeforeRemoveNode(const NodeComponent *n) {
  debugHeader("EVENT: REMOVE NODE");
  audioNodeTether.erase(n->model);
  // debugGraph();
  debugGraphCount(); // TODO remove this
}

void ModularGraphView::
onAfterRemoveNode() {
  debugHeader("EVENT: REMOVE NODE FINISHED");
  // debugGraph();
  debugGraphCount(); // TODO remove this
}

void ModularGraphView::
onBeforeRemoveWire(const WireComponent *w) {
  debugHeader("EVENT: REMOVE WIRE");
  auto n0 = w->startPin->node->model;
  auto n1 = w->endPin->node->model;
  auto an0 = audioNodeTether[n0];
  auto an1 = audioNodeTether[n1];
  if (an0 == nullptr || an1 == nullptr) {
    cerr << "ModularGraphView error: AudioNode not initialized." << endl
         << "source node: " << an0 << endl << "target node: " << an1 << endl;
    return;
  }
  for (int i = 0; i < modularAudio->kChannelCount; i++) {
    modularAudio->mainProcessor->removeConnection({ // TODO: random nullptr on Master in/out nodes.
      {an0->nodeID, i},
      {an1->nodeID, i}
    });
  }
  debugGraphCount(); // TODO remove this
}

void ModularGraphView::
onAfterRemoveWire() {
  debugHeader("EVENT: REMOVE WIRE FINISHED");
  debugGraphCount(); // TODO remove this
}

#pragma endregion Remove Methods

void ModularGraphView::
assertions() const {
  jassert(graph->wires.size() == wires.size());
  jassert(graph->nodes.size() == nodes.size());
}

void ModularGraphView::
duplicate() {
  vector<tuple<NodeComponent *, int, int>> nodesToAdd;

  for (auto &n : nodes) {
    if (n->selected) {
      nodesToAdd.emplace_back(n.get(), n->ins.size(), n->outs.size());
    }
  }

  for (auto &t : nodesToAdd) {
    NodeComponent *n;
    int ins = 0, outs = 0;
    tie(n, ins, outs) = t;
    if (dynamic_cast<NodeComponent *>(n)) {
      printf("not supported for now\n");
    } else {
      NodeDefinition def = *(n->definition.get());
      def.name = def.name + "-copy";
      def.instantiate(this, getBounds().getCentre().toFloat());
    }
  }
}

void ModularGraphView::removeSelected() {
  parameterWindow->viewSelected(nullptr);
  nodeSelection.clear();
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

void ModularGraphView::selectAll() {
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

#pragma region Pin Mouse Events

void ModularGraphView::
pinMouseDrag(NodeComponent::PinComponent *pin, const MouseEvent &e) {
  auto relativeEvent = e.getEventRelativeTo(this);
  auto position = relativeEvent.getPosition();
  wireDrawer->startPin = pin;
  wireDrawer->endPin = nullptr;
  wireDrawer->currentEndPosition = position;
  wireDrawer->calculateBounds(getLocalPoint(pin, Point<int>(theme.pinWidth / 2, theme.pinHeight / 2)), position);
  addAndMakeVisible(wireDrawer.get());
}

void ModularGraphView::
pinMouseUp(NodeComponent::PinComponent *pin, const MouseEvent &e) {
  auto relativeEvent = e.getEventRelativeTo(this);
  auto position = relativeEvent.getPosition();

  wireDrawer->currentEndPosition = position;

  removeChildComponent(wireDrawer.get());

}

void ModularGraphView::
pinMouseEnter(NodeComponent::PinComponent *pin, const MouseEvent &e) {
  auto relativeEvent = e.getEventRelativeTo(this);
  auto position = relativeEvent.getPosition();


  if (wireDrawer->currentEndPosition == position) {
    drawConnector(pin);
  }

}

#pragma endregion Pin Mouse Events

#pragma region Wire Methods

void ModularGraphView::
calculateWireBounds(WireComponent *wire) {
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

  wire->inverted = left != startPinTopLeft.x;
  
  wire->setBounds(left, top, w, h);
}

void ModularGraphView::
wireMouseDown(WireComponent *wire, const MouseEvent &e) {
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

    for (auto &n : nodes) {
      n->selected = false;
      n->repaint();
    }

  } else {
    wireMultiSelectionOn = true;
  }
}

void ModularGraphView::
wireMouseUp(WireComponent *wire, const MouseEvent &e) {}

void ModularGraphView::
drawConnector(NodeComponent::PinComponent *pin) {
  if (wireDrawer->startPin && wireDrawer->startPin != pin) {
    wireDrawer->endPin = pin;
    if (isLegalWire(wireDrawer->startPin, wireDrawer->endPin))
      addWire(wireDrawer->startPin, wireDrawer->endPin);
  }
}

bool ModularGraphView::
isLegalWire(NodeComponent::PinComponent *start, NodeComponent::PinComponent *end) {
  auto existing = find_if(wires.begin(), wires.end(), [&](auto &w) -> bool { return w->isConnecting(start, end); });
  auto isEndInput = end->model->pinType == PinType::In;
  for (auto &w : graph->wires) {
    if (isEndInput) {
      if (w->source == end->model || w->target == end->model) return false;
    } else {
      if (w->source == start->model || w->target == start->model) return false;
    }
  }
  return existing == wires.end()
    && start->model->pinType != end->model->pinType
    && start->node != end->node
    && start->model->flowType == end->model->flowType;
}


#pragma endregion Wire Methods

#pragma region Node Mouse Events

void ModularGraphView::
nodeMouseDrag(NodeComponent *node, const MouseEvent &e) {
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

void ModularGraphView::
nodeMouseDown(NodeComponent *node, const MouseEvent &e) {
  recordState();
  
  auto nodeSlice = remove_if(nodeSelection.begin(), nodeSelection.end(), 
    [&](const NodeComponent *n) -> bool { return n == node; });
  if (nodeSlice != nodeSelection.end()) {
    nodeSelection.push_front(*nodeSlice);
  } else {
    nodeSelection.push_front(node);
  }

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

#ifdef NODESYSTEM_DEBUG
  for (auto &n : nodes) {
    auto p = (NodeComponent*) n.get();
    if (n->selected)
      cout << "[graph-view-selected] node: address: " << n.get()
           << " model address: " << p->model << endl;
  }
#endif

}

void ModularGraphView::
nodeMouseUp(NodeComponent *node, const MouseEvent &e) {
  if (!nodeSelection.empty())
    parameterWindow->viewSelected(nodeSelection.front());
  recordState();
}

#pragma endregion Node Mouse Events

#pragma region Input and Mouse Events

void ModularGraphView::
mouseDown(const MouseEvent &e) {
  nodeSelection.clear();
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

void ModularGraphView::
mouseDrag(const MouseEvent &e) {
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

void ModularGraphView::
mouseUp(const MouseEvent &e) {
  if (!nodeSelection.empty()) {
    parameterWindow->viewSelected(nodeSelection.front());
  } else {
    parameterWindow->viewSelected(nullptr);
  }
  
  removeChildComponent(selector.get());
}

bool ModularGraphView::keyPressed(const KeyPress &key) {
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

#pragma endregion Input and Mouse Events

#pragma region View Events

void ModularGraphView::
zoomIn() {
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

void ModularGraphView::
zoomOut() {
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

void ModularGraphView::
zoomToOriginalSize() {
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

void ModularGraphView::
setParameterWindow(ModuleParameterWindow *window) {
  parameterWindow = window;
}

#pragma endregion View Events

void ModularGraphView::
recordState() {
  for (auto &n : nodes) {
    auto p = getLocalPoint(n.get(), Point<int>(0, 0));
    n->translation = AffineTransform::translation(p);
    n->scale = AffineTransform::scale(n->scaleFactor);
  }

}

#pragma region Debug Methods

void ModularGraphView::
debugGraph() {
  #ifdef NODESYSTEM_DEBUG
  cout << "Graph Debug:" << endl;

  cout << "Nodes: " << endl;

  try {
    string indent = indentN(_dbgIndent++);

    for (int i = 0; i < graph->nodes.size(); i++) {
      if (graph->nodes[i] == nullptr) throw exception("bad node ptr");
      debugNode(*(graph->nodes[i]));

      cout << "Node neighbors: " << endl;
      indent = indentN(_dbgIndent++);

      vector<const Graph::Node *> neighbors;
      graph->targets(graph->nodes[i].get(), neighbors);

      for_each(neighbors.begin(), neighbors.end(), [&](auto &nn) -> void {
        if (nn != nullptr) {
          cout << indent + "name: '" << nn->name << "' | '" << nn << "'" << endl;
        }
      });
      indent = indentN(_dbgIndent--);
    }

    _dbgIndent--;

  } catch (exception &e) {
    cout << "debugGraph error: " << e.what() << endl;
  }

  #endif // NODESYSTEM_DEBUG
}

void ModularGraphView::
debugNodeComponent(const NodeComponent &n, bool recurseUpstream) {
#ifdef NODESYSTEM_DEBUG

  cout << "NodeComponent debug: " << endl;
  cout << "name (Component): " << n.getName() << endl;
  cout << "selected: " << (n.selected ? "true" : "false") << endl;
  cout << "node model: " << endl;
  cout << "is valid: ";
  try {
    auto m = n.model;
    if (m == nullptr) throw exception();
    cout << "true" << endl;
    cout << "node model: " << endl;
    _dbgIndent++;
    debugNode(*m);
    _dbgIndent--;
  } catch (exception &e) {
    cout << "false" << endl;
  } catch (...) {
    cout << "an unhandled exception occurred." << endl;
  }

#endif // NODESYSTEM_DEBUG
}

void ModularGraphView::
debugNode(const Graph::Node &n, bool recurseUpstream) {
#ifdef NODESYSTEM_DEBUG
  auto indent = indentN(_dbgIndent);
  cout << indent + "node name: " << n.name << endl;
  indent = indentN(_dbgIndent++);
  cout << indent + "total in pins: " << n.ins.size() << endl;
  std::for_each(n.ins.begin(), n.ins.end(), [&](auto &p) {
    if (p == nullptr) throw exception();
    if (p->node != nullptr) {
      _dbgIndent++;
      debugPin(*(p.get()), recurseUpstream);
      _dbgIndent--;
    }
  });
  cout << indent + "total out pins: " << n.outs.size() << endl;
  std::for_each(n.outs.begin(), n.outs.end(), [&](auto &p) {
    if (p->node != nullptr) {
      _dbgIndent++;
      debugPin(*(p.get()), recurseUpstream);
      _dbgIndent--;
    }
  });
  cout << indent + "node listeners: [" << endl;
  for (auto &listener : n.listeners) {
    indent = indentN(_dbgIndent + 1);
    cout << indent << listener << "," << endl;
  }
  indent = indentN(--_dbgIndent);
  cout << indent + "] " << endl;
#endif // NODESYSTEM_DEBUG
}

void ModularGraphView::
debugNodeConnectors(const Graph::Node &n, bool recurseUpstream) {

}

void ModularGraphView::
debugPin(const Graph::Pin &p, bool recurseUpstream) {
#ifdef NODESYSTEM_DEBUG
  auto indent = indentN(_dbgIndent);
  cout << indent + "pin " << p.order << ": " << endl;

  auto pinType = p.pinType == Graph::PinType::In ? "PinType::In" : "PinType::Out";
  indent = indentN(_dbgIndent + 1);

  cout << indent + "pinType: " << pinType << endl;
  if (p.node == nullptr) {
    cout << "error accessing node at pin: '" << &p << "'." << endl;
  }
  cout << indent + "node: " << p.node << endl;
#endif
}

void ModularGraphView::
debugAudioGraph() {
#ifdef NODESYSTEM_DEBUG
  cout << "DEBUG AUDIO GRAPH: " << endl;
  auto audioConnections = modularAudio->mainProcessor->getConnections();
  for (AudioProcessorGraph::Connection con : audioConnections) {
    auto source = con.source;
    auto dest = con.destination;
    auto sourceNode = modularAudio->mainProcessor->getNodeForId(source.nodeID);
    auto sourceName = sourceNode->getProcessor()->getName();
    auto destNode = modularAudio->mainProcessor->getNodeForId(dest.nodeID);
    auto destName = destNode->getProcessor()->getName();
    cout << sourceName << " -> " << destName << " | ";
  }
  cout << endl;
#endif
}

void ModularGraphView::
debugGraphCount() {
#ifdef NODESYSTEM_DEBUG
  cout << "ModularGraphView Node Count: " << endl
  << "graph->nodes.size() = " << graph->nodes.size() << endl
  << "graph->wires.size() = " << graph->wires.size() << endl
  << "nodes.size() = " << nodes.size() << endl
  << "wires.size() = " << wires.size() << endl;
#endif
}

void ModularGraphView::
debugHeader(const string &title) {
#ifdef NODESYSTEM_DEBUG
  const int width = 60;
  int margin = width - title.size() != 0 ? (width - title.size()) / 2 : 0;
  string message;
  for (int i = 0; i < width; i++) {
    message += "=";
  }
  message += "\n";
  for (int i = 0; i < margin; i++) {
    message += " ";
  }
  message += title;
  for (int i = 0; i < margin; i++) {
    message += " ";
  }
  message += "\n";
  for (int i = 0; i < width; i++) {
    message += "=";
  }
  message += "\n";
  cout << message;
#endif
}

string ModularGraphView::
indentN(int x) {
  string indent;
  for (int i = 0; i < x; i++) indent += "\t";
  return indent;
}



#pragma endregion Debug Methods

DISABLE_WARNING_POP
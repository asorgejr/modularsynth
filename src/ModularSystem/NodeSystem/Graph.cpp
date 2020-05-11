//
// Created by asorgejr on 5/5/2020.
//

#include "Graph.h"
#include "NodeDefinition.h"

using namespace std;

Pin::Pin(const PinType &pinType, const Node *node, const int order, const FlowDataType &flowType)
: IGraphObject(reinterpret_cast<uint32>(this)),
  pinType(pinType), node(node), order(order), flowType(flowType) {}

void Pin::
flow(const var &data) const {
  if (pinType == PinType::In) {
    #ifdef NODESYSTEM_DEBUG
    printf("[flow] in-pin:%d data:%s\n", order, data.toString().toStdString().c_str());    //
    #endif
    node->flow(this, data);
  } else {
    #ifdef NODESYSTEM_DEBUG
    printf("[flow] out-pin:%d data:%s\n", order, data.toString().toStdString().c_str());    //
    #endif
    std::vector<const Pin *> targets;
    node->graph->targets(this, targets);
    for (auto &p : targets) p->flow(data);
  }
}

Node::Node(const Graph *graph, const NodeDefinition &def)
: IGraphObject(reinterpret_cast<uint32>(this)),
  graph(graph),
  type_name(def.type_name),
  name(def.name)
{
  jassert(def.inTypes.size() == def.ins && def.outTypes.size() == def.outs);
  for (auto i = 0; i < def.ins; i++) {
    auto pin = make_unique<Pin>(PinType::In, this, i, def.inTypes[i]);
    ins.push_back(move(pin));
  }
  for (auto i = 0; i < def.outs; i++) {
    auto pin = make_unique<Pin>(PinType::Out, this, i, def.outTypes[i]);
    outs.push_back(move(pin));
  }
}

void Node::
addListener(Graph::NodeListener *listener) {
  listeners.push_back(listener);
}

void Node::
removeListener(Graph::NodeListener *listener) {
  if (listeners.empty()) return;
  auto removeItr = std::remove_if( // TODO: access violation
    listeners.begin(),
    listeners.end(),
    [&](auto &current) -> bool {
      return current == listener;
    });
  if (removeItr != listeners.end()) {
    listeners.erase(removeItr, listeners.end());
  }
}

void Node::
flow(const Graph::Pin *source, const var &data) const {
  for (auto &l : listeners) l->onData(this, source, data);
  publish(data); // TODO: commented out by original author. Check that this is safe.
}

void Node::
publish(const var &data) const {
#ifdef NODESYSTEM_DEBUG
  printf("[publish] node:%s data:%s\n", name.c_str(), data.toString().toStdString().c_str());
#endif
  for (auto &p : outs) {
    p->flow(data);
  }
}

Node * Graph::
addNode(const NodeDefinition &def) {
#ifdef NODESYSTEM_DEBUG
  printf("[graph-add-node] name:%s, ins: %d, outs:%d\n", def.name.c_str(), def.ins, def.outs);  //
#endif
  auto node = std::make_unique<Node>(this, def);
  auto ptr = node.get();
  nodes.push_back(std::move(node));
  return ptr;
}

void Graph::
removeNode(const Graph::Node *node) { //works
  #ifdef NODESYSTEM_DEBUG
  printf("[graph-remove-node] name:%s\n", node->name.c_str());
  #endif
  // remove all connected wires
  std::vector<const Wire *> wiresToRemove;
  for (auto &e : wires) {
    if (e->source->node == node || e->target->node == node) {
      wiresToRemove.push_back(e.get());
    }
  }
  for (auto &e : wiresToRemove) {
    removeWire(e);
  }
  // remove the node
  auto removeItr = std::remove_if(
    nodes.begin(),
    nodes.end(),
    [&](auto &current) -> bool { return current.get() == node; });
  nodes.erase(removeItr);
  report();
}

const Graph::Wire * Graph::
addWire(const Graph::Pin *source, const Graph::Pin *target) {
  #ifdef NODESYSTEM_DEBUG
  printf("[graph-add-wire] source-node:%s source-pin:%d, target-node:%s, target-pin:%d\n",
    source->node->name.c_str(), source->order, target->node->name.c_str(), target->order);
  #endif
  auto wire = std::make_unique<Wire>(source, target);
  auto ptr = wire.get();
  wires.push_back(std::move(wire));
  return ptr;
}

void Graph::
removeWire(const Graph::Wire *wire) {
  #ifdef NODESYSTEM_DEBUG
  printf("[graph-remove-wire] source-node:%s source-pin:%d, target-node:%s, target-pin:%d\n",
    wire->source->node->name.c_str(),
    wire->source->order,
    wire->target->node->name.c_str(),
    wire->target->order);
  #endif
  auto removeItr = std::remove_if(
    wires.begin(),
    wires.end(),
    [&](auto &current) -> bool { return current.get() == wire; });
  wires.erase(removeItr, end(wires));
  report();
}

void Graph::
targets(const Graph::Pin *source, std::vector<const Pin *> &pins) const {
  pins.clear();
  for (auto &w : wires) {
    if (w->source == source) pins.push_back(w->target);
  }
}

void Graph::
targets(const Graph::Node *source, std::vector<const Node *> &nodes) const {
  nodes.clear();
  for (auto &w : wires) {
    if (w->source->node == source) nodes.push_back(w->target->node);
  }
}

void Graph::
dfs(const Graph::Node *node, const std::function<void(const Node *)> &visit) const {
  std::vector<const Node *> nodes;
  targets(node, nodes);
  for (auto &n : nodes) dfs(n, visit);
  visit(node);
}

void Graph::
bfs(const Graph::Node *node, const function<void(const Node *)> &visit) const {
  visit(node);
  std::vector<const Node *> nodes;
  targets(node, nodes);
  for (auto &n : nodes) bfs(n, visit);
}

void Graph::
report() const {
  #ifdef NODESYSTEM_DEBUG
  printf("#nodes:%llu, #wires:%llu\n", nodes.size(), wires.size());
  #endif
}

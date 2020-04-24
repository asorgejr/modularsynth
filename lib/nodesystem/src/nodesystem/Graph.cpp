//
// Created by asorgejr on 4/21/2020.
//

#include "../../include/nodesystem/Graph.h"

using namespace std;

namespace nodesystem {

Graph::Pin::Pin(const PinType &pinType, const Node *node, const int order) : pinType(pinType), node(node), order(order) {}

void Graph::Pin::flow(const var &data) const {
  if (pinType == PinType::In) {
    //printf("[flow] in-pin:%d data:%s\n", order, data.stringValue.c_str());
    node->flow(this, data);
  } else {
    //printf("[flow] out-pin:%d data:%s\n", order, data.stringValue.c_str());
    vector<const Pin *> targets;
    node->graph->targets(this, targets);
    for (auto &p : targets) p->flow(data);
  }
}


Graph::Node::Node(const Graph *graph, const string &name, const int numIns, const int numOuts) : graph(graph), name(name) {
  for (auto i = 0; i < numIns; i++) {
    auto pin = make_unique<Pin>(PinType::In, this, i);
    ins.push_back(move(pin));
  }
  for (auto i = 0; i < numOuts; i++) {
    auto pin = make_unique<Pin>(PinType::Out, this, i);
    outs.push_back(move(pin));
  }
}

void Graph::Node::addListener(NodeListener *listener) {
  listeners.push_back(listener);
}

void Graph::Node::removeListener(NodeListener *listener) {
  if (listeners.empty()) return;
  auto removeItr = remove_if( // TODO: access violation
    listeners.begin(),
    listeners.end(),
    [&](auto &current) -> bool {
      return current == listener;
    });
  if (removeItr != listeners.end()) {
    listeners.erase(removeItr, listeners.end());
  }
}

void Graph::Node::flow(const Pin *source, const var &data) const {
  for (auto &l : listeners) l->onData(this, source, data);
  //publish(data);
}

void Graph::Node::publish(const var &data) const {
  //printf("[publish] node:%s data:%s\n", name.c_str(), data.stringValue.c_str());
  for (auto &p : outs) {
    p->flow(data);
  }
}

Graph::Wire::Wire(const Pin *source, const Pin *target) : source(source), target(target) {}


Graph::Node *Graph::addNode(const std::string &name, const int numIns, const int numOuts) {
  //printf("[graph-add-node] name:%s, ins: %d, outs:%d\n", name.c_str(), numIns, numOuts);
  auto node = make_unique<Node>(this, name, numIns, numOuts);
  auto ptr = node.get();
  nodes.push_back(move(node));
  return ptr;
}

void Graph::removeNode(const Node *node) { //works
  printf("[graph-remove-node] name:%s\n", node->name.c_str());
  // remove all connected wires
  vector<const Wire *> wiresToRemove;
  for (auto &e : wires) {
    if (e->source->node == node || e->target->node == node) {
      wiresToRemove.push_back(e.get());
    }
  }
  for (auto &e : wiresToRemove) {
    removeWire(e);
  }
  // remove the node
  auto removeItr = remove_if(
    nodes.begin(),
    nodes.end(),
    [&](auto &current) -> bool { return current.get() == node; });
  nodes.erase(removeItr);
  //report();
}

const Graph::Wire *Graph::addWire(const Pin *source, const Pin *target) {
  //printf("[graph-add-wire] source-node:%s source-pin:%d, target-node:%s, target-pin:%d\n",
  //       source->node->name.c_str(), source->order, target->node->name.c_str(), target->order);
  auto wire = make_unique<Wire>(source, target);
  auto ptr = wire.get();
  wires.push_back(move(wire));
  return ptr;
}

void Graph::removeWire(const Wire *wire) {
  printf("[graph-remove-wire] source-node:%s source-pin:%d, target-node:%s, target-pin:%d\n",
         wire->source->node->name.c_str(),
         wire->source->order,
         wire->target->node->name.c_str(),
         wire->target->order);
  auto removeItr = remove_if(
    begin(wires),
    end(wires),
    [&](auto &current) -> bool { return current.get() == wire; });
  wires.erase(removeItr, end(wires));
  //report();
}

void Graph::targets(const Pin *source, std::vector<const Pin *> &pins) const {
  pins.clear();
  for (auto &e : wires) {
    if (e->source == source) pins.push_back(e->target);
  }
}

void Graph::targets(const Node *source, vector<const Node *> &nodes) const {
  nodes.clear();
  for (auto &e : wires) {
    if (e->source->node == source) nodes.push_back(e->target->node);
  }
}

void Graph::dfs(const Node *node, const function<void(const Node *)> &visit) const {
  vector<const Node *> nodes;
  targets(node, nodes);
  for (auto &n : nodes) dfs(n, visit);
  visit(node);
}

void Graph::bfs(const Node *node, const function<void(const Node *)> &visit) const {
  visit(node);
  vector<const Node *> nodes;
  targets(node, nodes);
  for (auto &n : nodes) bfs(n, visit);
}

void Graph::report() const {
  printf("#nodes:%llu, #wires:%llu\n", nodes.size(), wires.size());
}

}
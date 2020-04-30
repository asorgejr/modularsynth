#pragma once

#include <JuceHeader.h>
#include <memory>
#include <string>
#include <vector>
#include <functional>

namespace nodesystem {

/**
 * This defines the data model of the node graph system.
 */
struct Graph {

  enum class PinType;
  struct Pin;
  struct Node;
  struct Wire;

  std::vector<std::unique_ptr<Node>> nodes;
  std::vector<std::unique_ptr<Wire>> wires;

  #pragma region Graph Type Definitions

  #pragma region Pin

  enum class PinType {
    In, Out
  };

  struct Pin {

    const PinType pinType;

    const Node *node;

    const int order;

    Pin(const PinType &pinType, const Node *node, const int order) : pinType(pinType), node(node), order(order) {}

    void flow(const var &data) const {
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

  };

  #pragma endregion Pin

  #pragma region Node

  #pragma region NodeListener

  /**
   * This type 
   */
  struct NodeListener {

    virtual ~NodeListener() {}

    virtual void onData(const Node *sourceNode, const Pin *sourcePin, const var &data) {}

  };

  #pragma endregion NodeListener
  
  struct Node {

    const Graph *graph;
    
    const std::string name;
    
    std::vector<std::unique_ptr<Pin>> ins;
    
    std::vector<std::unique_ptr<Pin>> outs;
    
    std::vector<NodeListener *> listeners;

    Node(const Graph *graph, const std::string &name, const int numIns, const int numOuts) 
    : graph(graph), 
      name(name) 
    {
      for (auto i = 0; i < numIns; i++) {
        auto pin = std::make_unique<Pin>(PinType::In, this, i);
        ins.push_back(std::move(pin));
      }
      for (auto i = 0; i < numOuts; i++) {
        auto pin = std::make_unique<Pin>(PinType::Out, this, i);
        outs.push_back(std::move(pin));
      }
    }

    void addListener(NodeListener *listener) {
      listeners.push_back(listener);
    }

    void removeListener(NodeListener *listener) {
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

    void flow(const Pin *source, const var &data) const {
      for (auto &l : listeners) l->onData(this, source, data);
      //publish(data);
    }

    void publish(const var &data) const {
#ifdef NODESYSTEM_DEBUG
      printf("[publish] node:%s data:%s\n", name.c_str(), data.toString().toStdString().c_str());
#endif
      for (auto &p : outs) {
        p->flow(data);
      }
    }
    
  };

  #pragma endregion Node

  #pragma region Wire
  
  struct Wire {

    const Pin *source;
    
    const Pin *target;

    Wire(const Pin *source, const Pin *target) : source(source), target(target) {}
    
  };

  #pragma endregion Wire

  #pragma endregion Graph Type Definitions
  
  Node *addNode(const std::string &name, const int numIns, const int numOuts) {

#ifdef NODESYSTEM_DEBUG
    printf("[graph-add-node] name:%s, ins: %d, outs:%d\n", name.c_str(), numIns, numOuts);  //
#endif
    auto node = std::make_unique<Node>(this, name, numIns, numOuts);
    auto ptr = node.get();
    nodes.push_back(std::move(node));
    return ptr;
  }

  void removeNode(const Node *node) { //works
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
    auto removeItr = remove_if(
      nodes.begin(),
      nodes.end(),
      [&](auto &current) -> bool { return current.get() == node; });
    nodes.erase(removeItr);
    //report();
  }

  const Wire *addWire(const Pin *source, const Pin *target) {
#ifdef NODESYSTEM_DEBUG
    printf("[graph-add-wire] source-node:%s source-pin:%d, target-node:%s, target-pin:%d\n",
      source->node->name.c_str(), source->order, target->node->name.c_str(), target->order);
#endif
    auto wire = std::make_unique<Wire>(source, target);
    auto ptr = wire.get();
    wires.push_back(std::move(wire));
    return ptr;
  }

  void removeWire(const Wire *wire) {
#ifdef NODESYSTEM_DEBUG
    printf("[graph-remove-wire] source-node:%s source-pin:%d, target-node:%s, target-pin:%d\n",
      wire->source->node->name.c_str(),
      wire->source->order,
      wire->target->node->name.c_str(),
      wire->target->order);
#endif
    auto removeItr = remove_if(
      begin(wires),
      end(wires),
      [&](auto &current) -> bool { return current.get() == wire; });
    wires.erase(removeItr, end(wires));
    //report();
  }

  void targets(const Pin *source, std::vector<const Pin *> &pins) const {
    pins.clear();
    for (auto &e : wires) {
      if (e->source == source) pins.push_back(e->target);
    }
  }

  void targets(const Node *source, std::vector<const Node *> &nodes) const {
    nodes.clear();
    for (auto &e : wires) {
      if (e->source->node == source) nodes.push_back(e->target->node);
    }
  }

  void dfs(const Node *node, const std::function<void(const Node *)> &visit) const {
	  std::vector<const Node *> nodes;
    targets(node, nodes);
    for (auto &n : nodes) dfs(n, visit);
    visit(node);
  }

  void bfs(const Node *node, const std::function<void(const Node *)> &visit) const {
    visit(node);
		std::vector<const Node *> nodes;
    targets(node, nodes);
    for (auto &n : nodes) bfs(n, visit);
  }

  void report() const {
#ifdef NODESYSTEM_DEBUG
    printf("#nodes:%llu, #wires:%llu\n", nodes.size(), wires.size());
#endif
  }

};

}
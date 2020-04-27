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

  struct NodeListener {
    virtual ~NodeListener() {}
    virtual void onData(const Node *sourceNode, const Pin *sourcePin, const var &data) {}
  };

  std::vector<std::unique_ptr<Node>> nodes;
  std::vector<std::unique_ptr<Wire>> wires;

  enum class PinType {
    In, Out
  };

  struct Pin {
    const PinType pinType;
    const Node *node;
    const int order;

    Pin(const PinType &pinType, const Node *node, const int order);

    void flow(const var &data) const;
  };

  struct Node {

    const Graph *graph;
    const std::string name;
    std::vector<std::unique_ptr<Pin>> ins;
    std::vector<std::unique_ptr<Pin>> outs;
    std::vector<NodeListener *> listeners;

    Node(const Graph *graph, const std::string &name, const int numIns, const int numOuts);

    void addListener(NodeListener *listener);

    void removeListener(NodeListener *listener);

    void flow(const Pin *source, const var &data) const;

    void publish(const var &data) const;
  };

  struct Wire {

    const Pin *source;
    const Pin *target;

    Wire(const Pin *source, const Pin *target);
  };

  Node *addNode(const std::string &name, const int numIns, const int numOuts);

  void removeNode(const Node *node);

  const Wire *addWire(const Pin *source, const Pin *target);

  void removeWire(const Wire *wire);

  void targets(const Pin *source, std::vector<const Pin *> &pins) const;

  void targets(const Node *source, std::vector<const Node *> &nodes) const;

  void dfs(const Node *node, const std::function<void(const Node *)> &visit) const;

  void bfs(const Node *node, const std::function<void(const Node *)> &visit) const;

  void report() const;

};

}
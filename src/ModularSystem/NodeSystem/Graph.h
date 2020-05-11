#pragma once

#include <JuceHeader.h>
#include <memory>
#include <string>
#include <vector>
#include <functional>
#include "../../ModularAPI.h"

DISABLE_WARNING_PUSH
DISABLE_WARNING_TRUNCATION

struct NodeDefinition;

struct IGraphObject {
  const uint32 id;
  explicit IGraphObject(const uint32 &id) : id(id) {}
};

/**
 * This defines the data model of the node graph system.
 */
struct Graph {

  enum class FlowDataType {
    Audio, CV, Trigger
  };

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

  struct Pin : IGraphObject {

    const PinType pinType;

    const Node *node;

    const int order;
    
    const FlowDataType flowType;

    Pin(const PinType &pinType, const Node *node, const int order, const FlowDataType &flowType);

    void flow(const var &data) const;

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
  
  struct Node : IGraphObject {

    const Graph *graph;
    
    const std::string type_name;
    
    const std::string name;
    
    std::vector<std::unique_ptr<Pin>> ins;
    
    std::vector<std::unique_ptr<Pin>> outs;
    
    std::vector<NodeListener *> listeners;

    Node(const Graph *graph, const NodeDefinition &def);

    void addListener(NodeListener *listener);

    void removeListener(NodeListener *listener);

    void flow(const Pin *source, const var &data) const;

    void publish(const var &data) const;
    
  };

  #pragma endregion Node

  #pragma region Wire
  
  struct Wire : IGraphObject {

    const Pin *source;
    
    const Pin *target;

    Wire(const Pin *source, const Pin *target) 
    : IGraphObject(reinterpret_cast<uint32>(this)),
      source(source), target(target) {}
  };

  #pragma endregion Wire

  #pragma endregion Graph Type Definitions
  
  Node *addNode(const NodeDefinition &def);

  void removeNode(const Node *node);

  const Wire *addWire(const Pin *source, const Pin *target);

  void removeWire(const Wire *wire);

  /** This method populates parameter, pins with a set of Pins
      that are connected to parameter, source with a Wire.
      @param source the Pin to find targets for.
      @param pins the collection of Pins that are connected to source.
   */
  void targets(const Pin *source, std::vector<const Pin *> &pins) const;

  /** This method populates parameter, nodes with a set of Nodes 
      that are connected to parameter, source with a Wire.
      @param source the Node to find targets for.
      @param nodes the collection of Nodes that are connected to source.
   */
  void targets(const Node *source, std::vector<const Node *> &nodes) const;

  void dfs(const Node *node, const std::function<void(const Node *)> &visit) const;

  void bfs(const Node *node, const std::function<void(const Node *)> &visit) const;

  /** Prints a simple report of the number of nodes and the number of wires in the graph.
   */
  void report() const;

};

typedef Graph::FlowDataType FlowDataType;
typedef Graph::PinType PinType;
typedef Graph::Pin Pin;
typedef Graph::Node Node;
typedef Graph::Wire Wire;

DISABLE_WARNING_POP
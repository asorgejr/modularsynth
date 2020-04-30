
#pragma once

#include <JuceHeader.h>
#include "Graph.h"

namespace nodesystem {

struct NodeDefinition;

/**
 * This abstract class is used to define the look of a Graph::Node.
 * It can be used to design a node with custom component behaviors,
 * such as switches, faders and labels.
 */
class GraphNodeView {
  
public:

  virtual ~GraphNodeView() {}

  virtual Component *guest() = 0;

  virtual void setModel(Graph::Node *model) {}

  virtual void onData(const Graph::Node *sourceNode, const Graph::Pin *sourcePin, const var &data) {}
  
  virtual NodeDefinition getNodeDefinition();

};

}
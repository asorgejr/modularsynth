
#pragma once

#include <JuceHeader.h>
#include "../Graph.h"

struct NodeDefinition;

/**
 * This abstract class is used to define the UI behavior of a Graph::Node.
 * It can be used to design a node with custom component behaviors,
 * such as switches, faders and labels.
 * You must implement this class for each unique node that will be used 
 * in a custom nodesystem.
 */
class NodeViewComponent {
  
public:

  virtual ~NodeViewComponent() = default;

  virtual Component * guest() = 0;

  virtual void setModel(Graph::Node *model) {}

  virtual void onData(const Graph::Node *sourceNode, const Graph::Pin *sourcePin, const var &data) {}
  
  virtual NodeDefinition getNodeDefinition();

};

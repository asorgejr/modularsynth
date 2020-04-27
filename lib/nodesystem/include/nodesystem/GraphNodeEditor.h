
#pragma once

#include <JuceHeader.h>
#include "Graph.h"

namespace nodesystem {

/**
 * This abstract class can be used to design a node with custom component behaviors,
 * such as switches, faders and labels.
 */
class GraphNodeEditor {
public:

  virtual ~GraphNodeEditor() {}

  virtual Component *guest() = 0;

  virtual void setModel(Graph::Node *model) {}

  virtual void onData(const Graph::Node *sourceNode, const Graph::Pin *sourcePin, const var &data) {}

};

}
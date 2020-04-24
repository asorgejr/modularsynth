
#pragma once

#include <JuceHeader.h>
#include "Graph.h"

namespace nodesystem {

class GraphNodeEditor {
public:

  virtual ~GraphNodeEditor() {}

  virtual Component *guest() = 0;

  virtual void setModel(Graph::Node *model) {}

  virtual void onData(const Graph::Node *sourceNode, const Graph::Pin *sourcePin, const var &data) {}

};

}
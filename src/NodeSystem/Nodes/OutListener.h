//
// Created by asorgejr on 4/22/2020.
//

#pragma once
#include <nodesystem/nodesystem.h>

namespace nodesystem {

struct OutListener : public Graph::NodeListener {
  Graph::Node *outsideWorld;

  void onData(const Graph::Node *sourceNode, const Graph::Pin *sourcePin, const var &data) override;
};

}
//
// Created by asorgejr on 4/22/2020.
//

#include "OutListener.h"

namespace nodesystem {

void OutListener::onData(const Graph::Node *sourceNode, const Graph::Pin *sourcePin, const var &data) {
  if (outsideWorld) outsideWorld->publish(data);
}

}
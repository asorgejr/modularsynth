//
// Created by asorgejr on 4/24/2020.
//

#include "../../include/nodesystem/NodeDefinition.h"

#include <utility>

using namespace std;

namespace nodesystem {

NodeDefinition::NodeDefinition(const String &name, int32 ins, int32 outs, NodeInstantiator instantiate)
: instantiate(move(instantiate)) {
  this->name = name;
  this->ins = ins;
  this->outs = outs;
}

NodeDefinition::NodeDefinition(const String &name, int32 ins, int32 outs)
: NodeDefinition(name, ins, outs, [=](
  const shared_ptr<GraphViewComponent> &host, const Point<float> &pos) -> NodeComponent* {
    return host->addNode(name.toStdString(), ins, outs, pos);
}) {}


HostNodeDefinition::HostNodeDefinition(const String &name, int32 ins, int32 outs, int32 width, int32 height, NodeInstantiator instantiate)
: NodeDefinition(name, ins, outs, move(instantiate)) {
  this->width = width;
  this->height = height;
}

HostNodeDefinition::HostNodeDefinition(const String &name, int32 ins, int32 outs, int32 width, int32 height)
: NodeDefinition(name, ins, outs) {
  this->width = width;
  this->height = height;
}

HostNodeDefinition::HostNodeDefinition(const String &name, int32 ins, int32 outs, NodeInstantiator instantiate)
: NodeDefinition(name, ins, outs, move(instantiate)) {}

HostNodeDefinition::HostNodeDefinition(const String &name, int32 ins, int32 outs)
: NodeDefinition(name, ins, outs) {}



} // namespace nodesystem
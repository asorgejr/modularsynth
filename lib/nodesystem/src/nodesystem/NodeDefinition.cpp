//
// Created by asorgejr on 4/24/2020.
//

#include "../../include/nodesystem/NodeDefinition.h"

using namespace std;

namespace nodesystem {

NodeDefinition::NodeDefinition(const String &name, int32 ins, int32 outs, NodeTypeFn instantiate)
  : instantiate(move(instantiate)) {
  this->name = name;
  this->ins = ins;
  this->outs = outs;
}

NodeDefinition::NodeDefinition(const String &name, int32 ins, int32 outs)
  : NodeDefinition(name, ins, outs, [=](
  const shared_ptr<GraphViewComponent> &host, const Point<float> &pos) -> void {
  host->addNode(name.toStdString(), ins, outs, pos);
}) {}

}
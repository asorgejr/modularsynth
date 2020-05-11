//
// Created by asorgejr on 4/24/2020.
//

#include <utility>
#include "NodeDefinition.h"
#include "../ModularGraphView.h"

DISABLE_WARNING_PUSH
DISABLE_WARNING_LOSS_OF_DATA

using namespace std;

NodeDefinition::NodeDefinition(std::string type_name, const std::string &name, FlowDataList inTypes, FlowDataList outTypes, NodeInstantiator instantiate)
: type_name(move(type_name)),
  inTypes(move(inTypes)),
  outTypes(move(outTypes)),
  instantiate(move(instantiate))
{
  this->name = name;
  this->ins = this->inTypes.size();
  this->outs = this->outTypes.size();
}

NodeDefinition::NodeDefinition(const std::string& type_name, FlowDataList inTypes, FlowDataList outTypes, NodeInstantiator instantiate) 
: NodeDefinition(type_name, type_name, move(inTypes), move(outTypes), move(instantiate)) {}

NodeDefinition::NodeDefinition(const std::string &type_name, FlowDataList inTypes, FlowDataList outTypes)
: NodeDefinition(type_name, type_name, move(inTypes), move(outTypes), [=](
  ModularGraphView *host, const Point<float> &pos) -> NodeComponent* {
    return nullptr; //! TODO: <- Very Bad. Consider removing overload.
}) {}

DISABLE_WARNING_POP

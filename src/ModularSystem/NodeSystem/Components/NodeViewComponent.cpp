//
// Created by asorgejr on 4/29/2020.
//

#include "NodeViewComponent.h"
#include "../NodeDefinition.h"

NodeDefinition NodeViewComponent::
getNodeDefinition() {
  return NodeDefinition("UNDEF", FlowDataList{}, FlowDataList{});
}

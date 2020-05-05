//
// Created by asorgejr on 4/29/2020.
//

#include "../../include/nodesystem/NodeViewComponent.h"
#include "../../include/nodesystem/NodeDefinition.h"


namespace nodesystem {


HostNodeDefinition NodeViewComponent::getNodeDefinition() {
  return HostNodeDefinition(String("UNDEF"), 0, 0);
}

}
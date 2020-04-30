//
// Created by asorgejr on 4/29/2020.
//

#include "../../include/nodesystem/GraphNodeView.h"
#include "../../include/nodesystem/NodeDefinition.h"


namespace nodesystem {


NodeDefinition GraphNodeView::getNodeDefinition() {
  return NodeDefinition(String("UNDEF"), 0, 0);
}

}
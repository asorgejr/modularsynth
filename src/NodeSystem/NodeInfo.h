//
// Created by asorgejr on 4/22/2020.
//

#pragma once

#include <functional>
#include <JuceHeader.h>
#include <nodesystem/nodesystem.h>
#include "../common.h"
#include "Nodes/nodes.h"
#include "Nodes/OscillatorNode.h"

struct NodeInfo {
  inline static std::vector<nodesystem::NodeDefinition> NodeDefinitions = {
    OscillatorNode().getNodeDefinition()
  };
};

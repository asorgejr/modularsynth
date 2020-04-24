//
// Created by asorgejr on 4/22/2020.
//

#pragma once
#include "GraphNodeEditor.h"
#include "GraphViewComponent.h"
#include "NodeComponent.h"
#include "NodeDefinition.h"

namespace nodesystem {
template<class T>
using Shareable = std::enable_shared_from_this<T>;

}

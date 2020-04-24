//
// Created by asorgejr on 4/22/2020.
//

#include "NodeInfo.h"

#include <utility>

using namespace std;
using namespace nodesystem;

vector<NodeDefinition> NodeInfo::NodeDefinitions = {
  NodeDefinition("1 x 1", 1, 1),

  NodeDefinition("2 x 2", 2, 2),

  NodeDefinition("4 x 4", 4, 4),

  NodeDefinition("Slider", 1, 1,
    [](const sptr<GraphViewComponent> &host, const Point<float> &pos) -> void {
      host->addHostNode(make_unique<SliderPanel>(), 1, 1, 150, 150, pos);
  }),

  NodeDefinition("Text", 1, 0,
    [](const sptr<GraphViewComponent> &host, const Point<float> &pos) -> void {
     host->addHostNode(make_unique<TextPanel>(), 1, 0, 200, 100, pos);
  })
};
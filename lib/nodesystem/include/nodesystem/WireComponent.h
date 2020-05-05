#pragma once

#include <JuceHeader.h>
#include "Graph.h"
#include "NodeComponent.h"

namespace nodesystem {

class WireComponent : public Component {

public:

  GraphViewTheme theme;
  NodeComponent::PinComponent *startPin;
  NodeComponent::PinComponent *endPin;
  const Graph::Wire *model;
  Path path;

  bool inverted = false;
  bool selected = false;

  WireComponent(const GraphViewTheme &theme, NodeComponent::PinComponent *startPin,
    NodeComponent::PinComponent *endPin, const Graph::Wire *model);

  ~WireComponent();

  bool hitTest(int x, int y) override;

  void paint(Graphics &g) override;

  bool isConnecting(NodeComponent::PinComponent *first, NodeComponent::PinComponent *second);


private:

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WireComponent)


};

}
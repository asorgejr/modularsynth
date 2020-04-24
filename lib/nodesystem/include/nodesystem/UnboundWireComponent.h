#pragma once

#include <JuceHeader.h>
#include "NodeComponent.h"

namespace nodesystem {

class UnboundWireComponent : public Component {

public:

  bool inverted = false;

  NodeComponent::PinComponent *startPin;
  NodeComponent::PinComponent *endPin;
  Point<int> currentEndPosition;


  UnboundWireComponent();

  void paint(Graphics &g) override;

  void calculateBounds(const Point<int> &start, const Point<int> &end);

private:

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(UnboundWireComponent)
};

}
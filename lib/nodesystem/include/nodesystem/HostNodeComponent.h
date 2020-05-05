#pragma once

#include "NodeComponent.h"
#include "NodeViewComponent.h"

namespace nodesystem {

class HostNodeComponent : public NodeComponent {

public:
  
  std::unique_ptr<NodeViewComponent> nodeView;

  std::unique_ptr<HostNodeDefinition> definition;

  HostNodeComponent(const GraphViewTheme &theme, Graph::Node *model, std::unique_ptr<NodeViewComponent> editor);

  ~HostNodeComponent();

  void paint(Graphics &g) override;

  void resized() override;

  void onData(const Graph::Node *sourceNode, const Graph::Pin *sourcePin, const var &data) override;

private:

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HostNodeComponent)

};

}
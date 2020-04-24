#pragma once

#include "NodeComponent.h"
#include "GraphNodeEditor.h"

namespace nodesystem {

class HostNodeComponent : public NodeComponent {

public:
  std::unique_ptr<GraphNodeEditor> editor;

  HostNodeComponent(const GraphViewTheme &theme, Graph::Node *model, std::unique_ptr<GraphNodeEditor> editor);

  ~HostNodeComponent();

  void paint(Graphics &g) override;

  void resized() override;

  void onData(const Graph::Node *sourceNode, const Graph::Pin *sourcePin, const var &data) override;

  NodeDefinition getNodeDefinition() override;

private:

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HostNodeComponent)

};

}
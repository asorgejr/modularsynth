#include "../../include/nodesystem/HostNodeComponent.h"
#include "../../include/nodesystem/NodeDefinition.h"
#include "../../include/nodesystem/NodeSystemAPI.h"

using namespace std;

namespace nodesystem {

HostNodeComponent::HostNodeComponent(const GraphViewTheme &theme, Graph::Node *model, unique_ptr<NodeViewComponent> editor) 
: NodeComponent(theme, model),
  nodeView(move(editor)) 
{
  this->nodeView->setModel(model);
  auto d = nodeView->getNodeDefinition();
  definition = unique_ptr<HostNodeDefinition>(new HostNodeDefinition(d.name, d.ins, d.outs, d.width, d.height, d.instantiate));
}

HostNodeComponent::~HostNodeComponent() {

}


void HostNodeComponent::paint(Graphics &g) {
  auto bounds = boxBounds();
  Path p = definition->getNodeShape(bounds);

  if (selected) g.setColour(Colour(theme.cNodeBackgroundSelected));
  else g.setColour(Colour(cNodeBackgroundCurrent));
  g.fillPath(p);
}


void HostNodeComponent::resized() {
  auto bounds = boxBounds();
  auto guest = nodeView->guest();
  bounds.removeFromTop(theme.hostComponentDragAreaHeight);
  guest->setBounds(0, theme.hostComponentDragAreaHeight, bounds.getWidth(), bounds.getHeight() + theme.pinHeight);
  addAndMakeVisible(guest);
  NodeComponent::resized();
}

void HostNodeComponent::onData(const Graph::Node *sourceNode, const Graph::Pin *sourcePin, const var &data) {
  nodeView->onData(sourceNode, sourcePin, data);
}

}
//
// Created by asorgejr on 4/25/2020.
//

#include "OscillatorNode.h"

using namespace std;

OscillatorNode::OscillatorNode() {
  nodeName = "Oscillator";
  this->setName("Osc");
}

Component *OscillatorNode::guest() {
  return this;
}

OscillatorNode::~OscillatorNode() {

}

void OscillatorNode::setModel(Graph::Node *model) {
  this->model = model;
}

void OscillatorNode::onData(const Graph::Node *sourceNode, const Graph::Pin *sourcePin, const var &data) {
  cout << getName() << " onData: " << endl 
  << "sourceNode: '" << sourceNode->name << "' address: '" << sourceNode << "'" << endl
  << "sourcePin: '" << sourcePin->order << "'" << endl
  << "data: '" << data.toString() << "'" << endl;
}

void OscillatorNode::paint(Graphics &g) {
  g.drawText("Osc", 
    getLocalBounds().reduced(5),
    Justification::centred, true);
}

NodeDefinition OscillatorNode::getNodeDefinition() {
  return NodeDefinition("Oscillator", 1, 1,
    [](const sptr<GraphViewComponent> &host, const Point<float> &pos) -> void {
      host->addHostNode(make_unique<OscillatorNode>(), 
        2, 1, nodeWidth, nodeHeight, pos);
  });
}

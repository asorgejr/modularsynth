//
// Created by asorgejr on 4/25/2020.
//

#include "OscillatorNode.h"

Component *OscillatorNode::guest() {
  return this;
}

OscillatorNode::~OscillatorNode() {

}

void OscillatorNode::setModel(Graph::Node *model) {
  GraphNodeEditor::setModel(model);
}

void OscillatorNode::onData(const Graph::Node *sourceNode, const Graph::Pin *sourcePin, const var &data) {
  GraphNodeEditor::onData(sourceNode, sourcePin, data);
}

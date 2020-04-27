//
// Created by asorgejr on 4/17/2020.
//

#include "WaveformNode.h"


using namespace std;

namespace nodesystem {

WaveformNode::WaveformNode() {

}

WaveformNode::~WaveformNode() {

}

Component *WaveformNode::guest() {
  return this;
}

void WaveformNode::setModel(Graph::Node *model) {
  this->model = model;
}

void WaveformNode::onData(Graph::Node *sourceNode, Graph::Pin *sourcePin, const var &data) {
  
}


}
//
// Created by asorgejr on 4/17/2020.
//

#pragma once
#include <JuceHeader.h>
#include <nodesystem/nodesystem.h>
namespace nodesystem {

struct WaveformNode : public GraphNodeView, public Component {
  
  AudioProcessorGraph::Node::Ptr audioNode;

  Graph::Node *model;
  
  WaveformNode();

  ~WaveformNode() override;

  Component *guest() override;

  void setModel(Graph::Node *model) override;

  void onData(Graph::Node *sourceNode, Graph::Pin *sourcePin, const var &data);

};


}
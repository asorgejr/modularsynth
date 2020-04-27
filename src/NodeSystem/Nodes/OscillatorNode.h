//
// Created by asorgejr on 4/25/2020.
//

#pragma once
#include <JuceHeader.h>
#include <nodesystem/nodesystem.h>

using namespace nodesystem;

class OscillatorNode : public Component, public GraphNodeEditor {
public:
  Component *guest() override;

  ~OscillatorNode() override;

  void setModel(Graph::Node *model) override;

  void
  onData(const Graph::Node *sourceNode, const Graph::Pin *sourcePin, const var &data) override;

};




//
// Created by asorgejr on 4/25/2020.
//

#pragma once
#include <JuceHeader.h>
#include <nodesystem/nodesystem.h>
#include "../../AudioProcessors/Oscillator.h"

using namespace nodesystem;

class OscillatorNode : public Component, public GraphNodeView {
  
private:
  
  Graph::Node *model;
  
protected:
  
  inline static int nodeHeight = 80;
  
  inline static int nodeWidth = 80;
  
public:
  
  String nodeName;
  
  OscillatorNode();
  
  Component *guest() override;

  ~OscillatorNode() override;

  void setModel(Graph::Node *model) override;

  void onData(const Graph::Node *sourceNode, const Graph::Pin *sourcePin, const var &data) override;

  void paint(Graphics &g) override;

  NodeDefinition getNodeDefinition() override;

};




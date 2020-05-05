//
// Created by asorgejr on 4/25/2020.
//

#pragma once
#include <JuceHeader.h>
#include <nodesystem/nodesystem.h>
#include "Module.h"
#include "../AudioProcessors/Oscillator.h"

using namespace nodesystem;

class OscillatorNode : public Module {
  
public:
  
  OscillatorNode()
  : Module() { this->setName("Osc"); }
  
  Component * guest() override { return this; }

  ~OscillatorNode() override { removeAllChildren(); }

  void setModel(Graph::Node *model) override { this->model = model; }

  void onData(const Graph::Node *sourceNode, const Graph::Pin *sourcePin, const var &data) override {
    std::cout << getName() << " onData: " << std::endl
         << "sourceNode: '" << sourceNode->name << "' address: '" << sourceNode << "'" << std::endl
         << "sourcePin: '" << sourcePin->order << "'" << std::endl
         << "data: '" << data.toString() << "'" << std::endl;
  }

  void paint(Graphics &g) override {
    g.drawText("Osc",
      getLocalBounds().reduced(5),
      Justification::centred, true);
  }

  HostNodeDefinition getNodeDefinition() override {
    return HostNodeDefinition("Oscillator", 2, 1, 80, 40,
      [](const sptr<GraphViewComponent> &host, const Point<float> &pos) -> NodeComponent* {
        return host->addHostNode(std::make_unique<OscillatorNode>(), pos);
      });
  }
  
  AudioNodeGenerator generateAudioNode() override {
    return [](ModularAudioProc *maProc) -> AudioNode::Ptr {
      auto node = maProc->mainProcessor->addNode(std::make_unique<Oscillator>());
      return node;
    };
  }

};




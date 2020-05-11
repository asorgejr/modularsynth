//
// Created by asorgejr on 4/25/2020.
//

#pragma once
#include <JuceHeader.h>
#include "Module.h"
#include "../AudioProcessors/Oscillator.h"

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

  NodeDefinition getNodeDefinition() override {
    const NodeDefinition def = NodeDefinition("Oscillator",
      FlowDataList {FlowDataType::CV, FlowDataType::Audio},
      FlowDataList {FlowDataType::Audio},
      [](ModularGraphView *host, const Point<float> &pos) -> NodeComponent* {
        return host->addNode(std::make_unique<OscillatorNode>(), pos);
      });
    return def;
  }
  
  AudioNodeGenerator generateAudioNode() override {
    return [](ModularAudioProcessor *parent) -> AudioNode::Ptr {
      auto node = parent->mainProcessor->addNode(std::make_unique<Oscillator>(parent));
      return node;
    };
  }

};




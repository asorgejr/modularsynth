//
// Created by asorgejr on 4/25/2020.
//

#pragma once
#include <JuceHeader.h>

#include <memory>
#include "Module.h"
#include "../AudioProcessors/Filter.h"

class FilterNode : public Module {
  
public:
  
  FilterNode()
  : Module() { this->setName("Filter"); }
  
  Component * guest() override { return this; }

  ~FilterNode() override { removeAllChildren(); }

  void setModel(Graph::Node *model) override { this->model = model; }

  void onData(const Graph::Node *sourceNode, const Graph::Pin *sourcePin, const var &data) override {
    std::cout << getName() << " onData: " << std::endl
         << "sourceNode: '" << sourceNode->name << "' address: '" << sourceNode << "'" << std::endl
         << "sourcePin: '" << sourcePin->order << "'" << std::endl
         << "data: '" << data.toString() << "'" << std::endl;
  }

  NodeDefinition getNodeDefinition() override {
    const NodeDefinition def = NodeDefinition("Filter",
      FlowDataList {FlowDataType::Audio, FlowDataType::CV},
      FlowDataList {FlowDataType::Audio},
      [](ModularGraphView *host, const Point<float> &pos) -> NodeComponent* {
        return host->addNode(std::make_unique<FilterNode>(), pos);
      });
    return def;
  }
  
  AudioNodeGenerator generateAudioNode() override {
    return [](ModularAudioProcessor *parent) -> AudioNode::Ptr {
      auto node = parent->mainProcessor->addNode(make_unique<Filter>(parent));
      return node;
    };
  }

};




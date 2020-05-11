//
// Created by asorgejr on 5/3/2020.
//

#pragma once
#include <algorithm>
#include <JuceHeader.h>
#include "Module.h"

class AudioInputNode : public Module {
  
private:
  
  const std::string kAudioInputName = "AudioInput"; 
  
public:
  
  AudioInputNode() : Module() { this->setName("Audio In"); }

  ~AudioInputNode() { removeAllChildren(); }
  
  Component * guest() override { return this; }
  
  void setModel(Graph::Node *model) override { this->model = model; }
  
  void onData(const Graph::Node* sourceNode, const Graph::Pin* sourcePin, const var& data) override {
    std::cout << getName() << " onData: " << std::endl
         << "sourceNode: '" << sourceNode->name << "' address: '" << sourceNode << "'" << std::endl
         << "sourcePin: '" << sourcePin->order << "'" << std::endl
         << "data: '" << data.toString() << "'" << std::endl;
  }
  
  void paint(Graphics &g) override {
    g.drawText(getName(),
      getLocalBounds().reduced(5),
      Justification::centred, true);
  }

  NodeDefinition getNodeDefinition() override {
    const NodeDefinition def = NodeDefinition(kAudioInputName,
      FlowDataList {},
      FlowDataList {FlowDataType::Audio},
      [=](ModularGraphView *host, const Point<float> &pos) -> NodeComponent* {
        for (auto &node : host->nodes) {
          if (node->model->name == kAudioInputName) {
            return node.get();
          }
        }
        return host->addNode(std::make_unique<AudioInputNode>(), pos);
      });
    return def;
  }

  AudioNodeGenerator generateAudioNode() override {
    return [](ModularAudioProcessor *maProc) -> AudioNode::Ptr {
      return maProc->getMasterAudioIn();
    };
  }
  
};

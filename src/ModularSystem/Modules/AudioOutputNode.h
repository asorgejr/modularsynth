//
// Created by asorgejr on 5/3/2020.
//

#pragma once
#include <JuceHeader.h>
#include "Module.h"

class AudioOutputNode : public Module {
  
private:
  const std::string kAudioOutputName = "AudioOutput";
  
public:

  AudioOutputNode() : Module() { this->setName("Audio Out"); }

  ~AudioOutputNode() { removeAllChildren(); }
  
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
    const NodeDefinition def = NodeDefinition(kAudioOutputName,
      FlowDataList {FlowDataType::Audio},
      FlowDataList {},
      [=](ModularGraphView *host, const Point<float> &pos) -> NodeComponent* {
        for (auto &node : host->nodes) {
          if (node->model->name == kAudioOutputName) {
            return node.get();
          }
        }
        return host->addNode(std::make_unique<AudioOutputNode>(), pos);
      });
    return def;
  }
  
  AudioNodeGenerator generateAudioNode() override {
    return [](ModularAudioProcessor *maProc) -> AudioNode::Ptr {
      return maProc->getMasterAudioOut();
    };
  }
  
};

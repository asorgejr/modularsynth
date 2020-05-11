//
// Created by asorgejr on 5/3/2020.
//

#pragma once
#include <JuceHeader.h>
#include "Module.h"

class MidiOutputNode : public Module {
  
private:
  
  const std::string kMidiOutputName = "MidiOutput";
  
public:
  
  MidiOutputNode() : Module() { this->setName("Midi Out"); }

  ~MidiOutputNode() { removeAllChildren(); }
  
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
    const NodeDefinition def = NodeDefinition(kMidiOutputName,
      FlowDataList {FlowDataType::CV},
      FlowDataList {},
      [=](ModularGraphView *host, const Point<float> &pos) -> NodeComponent* {
        return host->addNode(std::make_unique<MidiOutputNode>(), pos);
      });
    return def;
  }

  AudioNodeGenerator generateAudioNode() override {
    return [](ModularAudioProcessor *maProc) -> AudioNode::Ptr {
      return maProc->getMasterMidiOut();
    };
  }
  
};

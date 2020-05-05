//
// Created by asorgejr on 5/3/2020.
//

#pragma once
#include <JuceHeader.h>
#include <nodesystem/nodesystem.h>
#include "Module.h"

class MidiOutputNode : public Module {
  
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

  HostNodeDefinition getNodeDefinition() override {
    return HostNodeDefinition("MidiOutput", 1, 0, 80, 40,
      [](const sptr<GraphViewComponent> &host, const Point<float> &pos) -> NodeComponent* {
        return host->addHostNode(std::make_unique<MidiOutputNode>(), pos);
      });
  }

  AudioNodeGenerator generateAudioNode() override {
    return [](ModularAudioProc *maProc) -> AudioNode::Ptr {
      return maProc->masterAudioIn;
    };
  }
  
};

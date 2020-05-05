//
// Created by asorgejr on 4/25/2020.
//

#pragma once
#include <JuceHeader.h>
#include <nodesystem/nodesystem.h>
#include "../ModularAPI.h"
#include "../Components/ModularComponent/ModularComponent.h"

using namespace nodesystem;

typedef ModularComponent::ModularAudioProcessor ModularAudioProc;
using AudioNodeGenerator = std::function<AudioNode::Ptr(ModularAudioProc *)>;

class Module : public Component, public NodeViewComponent {
  
public:
  
  AudioProcessorGraph::Node::Ptr audioNode;
  
protected:
  
  Graph::Node *model;
  
//  public: std::unique_ptr<Oscillator> processor;
  
public:
  
  virtual Component *guest() override { return this; }

  virtual ~Module() override {};

  virtual void setModel(Graph::Node *model) override { this->model = model; }

  virtual void onData(const Graph::Node *sourceNode, const Graph::Pin *sourcePin, const var &data) override {
  #ifdef  NODESYSTEM_DEBUG
    std::cout << getName() << " onData: " << std::endl
    << "sourceNode: '" << sourceNode->name << "' address: '" << sourceNode << "'" << std::endl
    << "sourcePin: '" << sourcePin->order << "'" << std::endl
    << "data: '" << data.toString() << "'" << std::endl;
  #endif
  }

  virtual void paint(Graphics &g) override {};

  virtual HostNodeDefinition getNodeDefinition() override = 0;

  virtual AudioNodeGenerator generateAudioNode() = 0;

};

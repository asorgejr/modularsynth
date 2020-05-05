//
// Created by asorgejr on 4/22/2020.
//

#pragma once

#include <functional>
#include <type_traits>
#include <JuceHeader.h>
#include <nodesystem/nodesystem.h>
#include "../common.h"
#include "../Modules/Modules.h"
#include "../AudioProcessors/AudioProcessors.h"
#include "../Components/ModularComponent/ModularComponent.h"

template<typename T> concept NodeViewType = std::is_base_of<NodeViewComponent, T>::value;
template<typename T> concept ModuleType = std::is_base_of<Module, T>::value;
struct NodeInfo {
  
protected:
  
  inline static std::vector<nodesystem::NodeDefinition> *NodeDefinitions;
  inline static std::map<String, AudioNodeGenerator> *AudioNodeGenerators;

  template <NodeViewType T> inline static NodeDefinition GetND() {
    NodeDefinition def = T().getNodeDefinition();
    return def;
  }

  template <NodeViewType T> inline static HostNodeDefinition GetHND() {
    HostNodeDefinition def = T().getNodeDefinition();
    return def;
  }
  
  template <ModuleType T> inline static AudioNodeGenerator GetANG() {
    return T().generateAudioNode();
  }
  
public:
  
  inline static std::vector<nodesystem::NodeDefinition> getNodeDefinitions() {
    if (NodeDefinitions == nullptr) {
      NodeDefinitions = new std::vector<nodesystem::NodeDefinition> {
        GetHND<AudioInputNode>(),
        GetHND<AudioOutputNode>(),
        GetHND<MidiInputNode>(),
        GetHND<MidiOutputNode>(),
        GetHND<OscillatorNode>()
      };
    }
    auto ret = *NodeDefinitions;
    return ret;
  }

protected:
  
  static void initializeANGs() {
    AudioNodeGenerators = new std::map<String, AudioNodeGenerator> {
      {
        GetHND<AudioInputNode>().name,
        GetANG<AudioInputNode>()
      },
      {
        GetHND<AudioOutputNode>().name,
        GetANG<AudioOutputNode>()
      },
      {
        GetHND<MidiInputNode>().name,
        GetANG<MidiInputNode>()
      },
      {
        GetHND<MidiOutputNode>().name,
        GetANG<MidiOutputNode>()
      },
      {
        GetHND<OscillatorNode>().name,
        GetANG<OscillatorNode>()
      }
    };
  }
  
public:
  
  inline static bool tryGetAudioNodeGenerator(const String &key, AudioNodeGenerator *out) {
    if (AudioNodeGenerators == nullptr) {
      initializeANGs();
    }
    if (!AudioNodeGenerators->contains(key)) {
      return false;
    }
    *out = (*AudioNodeGenerators)[key];
    return true;
  }
};

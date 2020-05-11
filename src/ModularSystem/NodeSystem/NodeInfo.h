//
// Created by asorgejr on 4/22/2020.
//

#pragma once

#include <functional>
#include <type_traits>
#include <JuceHeader.h>
#include "nodesystem.h"
#include "../../common.h"
#include "../Modules/Modules.h"
#include "../AudioProcessors/AudioProcessors.h"
#include "../ModularComponent.h"

template<typename T> concept NodeViewType = std::is_base_of<NodeViewComponent, T>::value;
template<typename T> concept ModuleType = std::is_base_of<Module, T>::value;
struct NodeInfo {
  
protected:
  
  inline static std::vector<NodeDefinition> *NodeDefinitions;
  inline static std::map<std::string, AudioNodeGenerator> *AudioNodeGenerators;

  template <NodeViewType T> inline static NodeDefinition GetND() {
    NodeDefinition def = T().getNodeDefinition();
    return def;
  }

//  template <NodeViewType T> inline static HostNodeDefinition GetHND() {
//    HostNodeDefinition def = T().getNodeDefinition();
//    return def;
//  }
  
  template <ModuleType T> inline static AudioNodeGenerator GetANG() {
    return T().generateAudioNode();
  }
  
public:
  
  inline static std::vector<NodeDefinition> getNodeDefinitions() {
    if (NodeDefinitions == nullptr) {
      NodeDefinitions = new std::vector<NodeDefinition> {
        GetND<AudioInputNode>(),
        GetND<AudioOutputNode>(),
        GetND<MidiInputNode>(),
        GetND<MidiOutputNode>(),
        GetND<OscillatorNode>(),
        GetND<FilterNode>()
      };
    }
    auto ret = *NodeDefinitions;
    return ret;
  }

protected:
  
  static void initializeANGs() {
    AudioNodeGenerators = new std::map<std::string, AudioNodeGenerator> {
      {
        GetND<AudioInputNode>().type_name,
        GetANG<AudioInputNode>()
      },
      {
        GetND<AudioOutputNode>().type_name,
        GetANG<AudioOutputNode>()
      },
      {
        GetND<MidiInputNode>().type_name,
        GetANG<MidiInputNode>()
      },
      {
        GetND<MidiOutputNode>().type_name,
        GetANG<MidiOutputNode>()
      },
      {
        GetND<OscillatorNode>().type_name,
        GetANG<OscillatorNode>()
      },
      {
        GetND<FilterNode>().type_name,
        GetANG<FilterNode>()
      }
    };
  }
  
public:
  
  inline static bool tryGetAudioNodeGenerator(const std::string &type_name, AudioNodeGenerator *out) {
    if (AudioNodeGenerators == nullptr) {
      initializeANGs();
    }
    if (!AudioNodeGenerators->contains(type_name)) return false;
    *out = (*AudioNodeGenerators)[type_name];
    return true;
  }
};

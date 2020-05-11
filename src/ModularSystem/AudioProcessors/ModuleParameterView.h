//
// Created by asorgejr on 5/7/2020.
//

#pragma once

#include <type_traits>
#include <JuceHeader.h>
#include "../../common.h"
#include "../NodeSystem/Components/NodeComponent.h"

class ModuleProcessor;

template <typename T>
concept TModuleProcessor = requires { std::is_base_of<ModuleProcessor, T>::value; };

class ModuleParameterView : public GenericAudioProcessorEditor {
  
public:
  
  ModuleProcessor &moduleProcessor;
  
  const Rectangle<int> param_bounds = Rectangle<int> (0, 0, 100, 100);
  
  std::vector<uptr<Component>> parameters;
  
public:
  
  explicit ModuleParameterView(ModuleProcessor &processor);
  
  ~ModuleParameterView();

  void setControlHighlight(ParameterControlHighlightInfo info) override {}

  virtual int getControlParameterIndex(Component &component) override {
    return -1;
  }

  virtual bool supportsHostMIDIControllerPresence(bool hostMIDIControllerIsAvailable) override {
    return true;
  }

  virtual void hostMIDIControllerIsAvailable(bool controllerIsAvailable) override {}

};


#pragma region ModuleParameterWindow

class ModularGraphView;

class ModuleParameterWindow : public Component {
public:
  
  struct InactiveView : public Component {
    void paint(Graphics &g) override {
      g.fillAll(config::Palette::WindowBackground);
      g.drawText("Select a node to view parameters.",
        getLocalBounds().reduced(100), Justification::centred, true);
    }
  };

  ModularGraphView *view;
  uptr<InactiveView> inactiveView;
  ModuleParameterView *moduleParameters = nullptr;
  
public:

  explicit ModuleParameterWindow(ModularGraphView *view);
  
  void resized() override;

  void viewSelected(NodeComponent *node);
  
};

#pragma endregion ModuleParameterWindow

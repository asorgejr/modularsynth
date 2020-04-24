//
// Created by asorgejr on 4/17/2020.
//

#pragma once
#include <JuceHeader.h>
#include <modularsynth/IModularComponent.h>
#include <nodesystem/nodesystem.h>
#include "../common.h"
#include "../Utilities.h"

namespace modularsynth {
class ModularNodeGraph : nodesystem::Shareable<ModularNodeGraph>, public AudioProcessorGraph, public IModularComponent {
public:
  struct CustomGraphView : public nodesystem::Shareable<CustomGraphView>, public nodesystem::GraphViewComponent {
    void popupMenu(const MouseEvent &e) override;
  };
  
  sptr<CustomGraphView> view;
  sptr<Toolbar> toolbar;
  
public:
  ModularNodeGraph(sptr<AudioDeviceManager> audioDeviceManager, sptr<MouseListener> sharedMouseListener);
  ~ModularNodeGraph() override;

  void paint(Graphics &g) override;

  void resized() override;
  
  void layout();
  
  void childBoundsChanged(Component* child) override;
  
  void getView(const CustomGraphView *self, const std::function<void(sptr<CustomGraphView>)> &func);

};

}
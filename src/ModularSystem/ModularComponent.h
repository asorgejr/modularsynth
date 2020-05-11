//
// Created by asorgejr on 4/17/2020.
//

#pragma once
#include <JuceHeader.h>
#include "../common.h"

#include "../Models/IModularComponent.h"
#include "NodeSystem/nodesystem.h"
#include "ModularGraphView.h"

namespace modularsynth {

class ModularComponent : public Component, public IModularComponent {
  
  #pragma region Public Members
  
public:

  uptr<ModuleParameterWindow> parameterWindow;
  
  sptr<ModularAudioProcessor> modularAudioProcessor;
  
  sptr<ModularGraphView> view;
  
  sptr<Toolbar> toolbar;
  
  std::vector<uptr<AudioNode>> audioNodes;
  
  uptr<ResizableBorderComponent> resizer;
  
  uptr<ComponentBoundsConstrainer> constrainer;

  sptr<MidiKeyboardComponent> keyboardComponent;

  MidiKeyboardState keyboardState{ MidiKeyboardState() };

  #pragma endregion Public Members

  #pragma region NonPublic Members

protected:
  
  #pragma endregion NonPublic Members

  #pragma region Public Methods
  
public:
  
  ModularComponent(const sptr<AudioDeviceManager> &audioDeviceManager, const sptr<MouseListener> &sharedMouseListener);
  
  ~ModularComponent() override;

  void paint(Graphics &g) override;

  void resized() override;
  
  void layout();
  
  void childBoundsChanged(Component* child) override;
  
  #pragma endregion Public Methods
  
private:
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ModularComponent);

};

}
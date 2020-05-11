//
// Created by asorgejr on 5/7/2020.
//

#include "ModuleParameterView.h"
#include "ModuleProcessor.h"
#include "../ModularGraphView.h"

using namespace std;

ModuleParameterView::ModuleParameterView(ModuleProcessor &processor)
: GenericAudioProcessorEditor(processor),
  moduleProcessor(processor)
{}

ModuleParameterView::
~ModuleParameterView() {
  parameters.clear();
}

ModuleParameterWindow::
ModuleParameterWindow(ModularGraphView *view)
: view(view),
  inactiveView(std::make_unique<InactiveView>()) 
{
  addAndMakeVisible(inactiveView.get());
}

void ModuleParameterWindow::
resized() {
  if (moduleParameters == nullptr) {
    inactiveView->setBounds(getLocalBounds());
    addAndMakeVisible(inactiveView.get());
  } else {
    addAndMakeVisible(moduleParameters);
  }
}

void ModuleParameterWindow::
viewSelected(NodeComponent *node) {
  ModuleProcessor *processor = nullptr;
  if (node && view->audioNodeTether.contains(node->model))
    processor = dynamic_cast<ModuleProcessor *>(
      view->audioNodeTether[node->model]->getProcessor());

  if (processor == nullptr) {
    addAndMakeVisible(inactiveView.get());
    removeChildComponent(moduleParameters);
    return;
  }
  moduleParameters = processor->createEditor();
  addAndMakeVisible(moduleParameters);
  removeChildComponent(inactiveView.get());
}





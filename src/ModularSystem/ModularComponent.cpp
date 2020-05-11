//
// Created by asorgejr on 4/17/2020.
//
#include <memory>
#include "../app/config.h"
#include "ModularComponent.h"

DISABLE_WARNING_PUSH
DISABLE_WARNING_LOSS_OF_DATA

namespace modularsynth {
using namespace std;


#pragma region ModularComponent

ModularComponent::ModularComponent(const sptr<AudioDeviceManager> &audioDeviceManager, const sptr<MouseListener> &sharedMouseListener) 
: Component(),
  IModularComponent(audioDeviceManager, sharedMouseListener)
{
  keyboardComponent = make_shared<MidiKeyboardComponent>(keyboardState, MidiKeyboardComponent::horizontalKeyboard);
  keyboardComponent->setSize(800, 100);

  modularAudioProcessor = make_shared<ModularAudioProcessor>(this->audioDeviceManager);

  view = shared_ptr<ModularGraphView>(dynamic_cast<ModularGraphView*>(modularAudioProcessor->createEditor()));
  view->setSize(800, 400);
  view->theme = config::Palette::GraphViewTheme;
  toolbar = make_shared<Toolbar>();
  
  parameterWindow = make_unique<ModuleParameterWindow>(view.get());
  view->setParameterWindow(parameterWindow.get());
  
  constrainer = make_unique<ComponentBoundsConstrainer>();
  resizer = make_unique<ResizableBorderComponent>(parameterWindow.get(), constrainer.get());
  resizer->setBorderThickness(BorderSize<int>(20));
}

ModularComponent::~ModularComponent() = default;

void ModularComponent::paint(Graphics &g) {
  g.fillAll(getUIColourIfAvailable(LookAndFeel_V4::ColourScheme::UIColour::windowBackground));
}

void ModularComponent::resized() {
  constrainer->setSizeLimits(
    getBounds().getWidth() * 0.1,
    getBounds().getHeight() * 0.1,
    getBounds().getWidth() * 0.9,
    getBounds().getHeight() * 0.9);
  auto graphViewer = view.get();
  auto paramWindow = parameterWindow.get();
  
  auto rszr = resizer.get();
  auto cbounds = getLocalBounds();
  cbounds.setWidth(cbounds.getWidth() * 0.3f);
  constrainer->setBoundsForComponent(
    paramWindow,
    cbounds,
    false, false, false, true);

  addAndMakeVisible(graphViewer);
  addAndMakeVisible(paramWindow);
  addAndMakeVisible(rszr);
  
  auto keyboard = keyboardComponent.get();
  addAndMakeVisible(keyboard);
  layout();
}

void ModularComponent::layout() {
  auto graphViewer = view.get();
  auto paramWindow = parameterWindow.get();
  
  FlexBox flexBox;
  flexBox.flexDirection = FlexBox::Direction::column;
  flexBox.justifyContent = FlexBox::JustifyContent::flexStart;
  flexBox.alignItems = FlexBox::AlignItems::flexStart;
  flexBox.alignContent = FlexBox::AlignContent::stretch;

  FlexItem tbi(*toolbar);
  tbi.alignSelf = FlexItem::AlignSelf::stretch;
  tbi.minHeight = 40.0f;

  auto gviBounds = Rectangle<float>(0,0, getWidth() * 0.7f, getHeight() * 0.9f);
  FlexItem gvi(gviBounds.getWidth(), gviBounds.getHeight(), *graphViewer);
  gvi.alignSelf = FlexItem::AlignSelf::autoAlign;
  gvi.minHeight = gviBounds.getHeight();
  
  auto keysBounds = getRemainingHeight(gviBounds, getBounds());
  FlexItem keys(keysBounds.getWidth(), keysBounds.getHeight(), *keyboardComponent);
  keys.alignSelf = FlexItem::AlignSelf::autoAlign;
  keys.minHeight = 64;
  
//    flexBox.items.add(tbi);
  flexBox.items.add(gvi);
  flexBox.items.add(keys);


  FlexBox flexBox1;
  flexBox1.flexDirection = FlexBox::Direction::column;
  flexBox1.justifyContent = FlexBox::JustifyContent::flexStart;
  flexBox1.alignItems = FlexBox::AlignItems::flexStart;
  flexBox1.alignContent = FlexBox::AlignContent::stretch;
  
  auto paramsBounds = getRemainingRect(gviBounds, getBounds());
  paramsBounds.setHeight(getHeight());
  FlexItem params(paramsBounds.getWidth(), paramsBounds.getHeight(), *parameterWindow);
  gvi.alignSelf = FlexItem::AlignSelf::autoAlign;
  gvi.minHeight = paramsBounds.getHeight();
  flexBox1.items.add(params);
  
  FlexBox mainFlexBox;
  mainFlexBox.flexDirection = FlexBox::Direction::row;
  mainFlexBox.justifyContent = FlexBox::JustifyContent::flexStart;
  mainFlexBox.alignItems = FlexBox::AlignItems::flexStart;
  mainFlexBox.alignContent = FlexBox::AlignContent::spaceBetween;
  
  FlexItem fb0(flexBox);
  fb0.alignSelf = FlexItem::AlignSelf::autoAlign;
  
  FlexItem fb1(flexBox1);
  fb1.alignSelf = FlexItem::AlignSelf::autoAlign;
  
  mainFlexBox.items.add(FlexItem(gviBounds.getWidth(), getHeight(), flexBox));
  mainFlexBox.items.add(FlexItem(paramsBounds.getWidth(), getHeight(), flexBox1));
  mainFlexBox.performLayout(getLocalBounds());
}

void ModularComponent::childBoundsChanged(Component *child) {
  layout();
}

#pragma endregion ModularComponent

}

DISABLE_WARNING_POP
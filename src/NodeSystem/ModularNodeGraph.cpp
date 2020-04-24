//
// Created by asorgejr on 4/17/2020.
//
#include <memory>
#include "../../app/config.h"
#include "ModularNodeGraph.h"
#include "NodeInfo.h"
#include "Nodes/nodes.h"

namespace modularsynth {
using namespace std;
using namespace nodesystem;

ModularNodeGraph::ModularNodeGraph(sptr<AudioDeviceManager> audioDeviceManager, sptr<MouseListener> sharedMouseListener) 
  : AudioProcessorGraph(),
  IModularComponent(audioDeviceManager, sharedMouseListener) 
{
  view = make_shared<CustomGraphView>();
  view->setSize(800, 400);
  view->theme = config::Palette::GraphViewTheme;
  toolbar = make_shared<Toolbar>();
}

ModularNodeGraph::~ModularNodeGraph() = default;

void ModularNodeGraph::paint(Graphics &g) {
  g.fillAll(getUIColourIfAvailable(LookAndFeel_V4::ColourScheme::UIColour::windowBackground));
}

void ModularNodeGraph::resized() {
  auto r = getLocalBounds();
  auto graphViewer = view.get();
  addAndMakeVisible(graphViewer);
  layout();
}

void ModularNodeGraph::layout() {
    auto graphViewer = view.get();

    FlexBox flexBox;
    flexBox.flexDirection = FlexBox::Direction::column;
    flexBox.justifyContent = FlexBox::JustifyContent::flexStart;
    flexBox.alignItems = FlexBox::AlignItems::flexStart;
    flexBox.alignContent = FlexBox::AlignContent::stretch;

    FlexItem tbi(*toolbar);
    tbi.alignSelf = FlexItem::AlignSelf::stretch;
    tbi.minHeight = 40.0f;

    FlexItem gvi(*graphViewer);
    gvi.alignSelf = FlexItem::AlignSelf::stretch;
    gvi.minHeight = getHeight();
    
    //flexBox.items.add(tbi);
    flexBox.items.add(gvi);

    flexBox.performLayout(getBounds());
}

void ModularNodeGraph::childBoundsChanged(Component *child) {
  layout();
}

void ModularNodeGraph::CustomGraphView::popupMenu(const MouseEvent &e) {
  PopupMenu m;
  auto position = e.getMouseDownPosition().toFloat();
  for (auto i = 0; i < NodeInfo::NodeDefinitions.size(); i++) {
    auto mI = i+1;
    m.addItem(mI, NodeInfo::NodeDefinitions[i].name);
  }
//  m.addItem(1, "1 x 1");
//  m.addItem(2, "2 x 2");
//  m.addItem(3, "4 x 4");
//  m.addItem(4, "Slider");
//  m.addItem(5, "Text");
//  m.addItem(6, "Graph");
  auto selection = m.show();
  auto sliderPanel = [&]() {
    addHostNode(std::make_unique<SliderPanel>(), 1, 1, 150, 150, position);
  };
  auto textPanel = [&]() {
    addHostNode(std::make_unique<TextPanel>(), 1, 0, 200, 100, position);
  };
  auto embeddedGraphView = [&]() {
    addHostNode(std::make_unique<EmbeddedGraphView>(), 1, 1, 400, 400, position);
  };
  try {
    auto sI = selection >= 1 ? selection - 1 : 0;
    auto ShareSelfCopy = shared_from_this();
    NodeInfo::NodeDefinitions[sI].instantiate(ShareSelfCopy, position);
  } catch (exception &e) {
    cerr << "A shared_ptr could not be created." 
    << endl << e.what() << endl;
  }
//  switch (selection) {
//    case 1:
//      addNode("1 x 1", 1, 1, position);
//      break;
//    case 2:
//      addNode("2 x 2", 2, 2, position);
//      break;
//    case 3:
//      addNode("4 x 4", 4, 4, position);
//      break;
//    case 4:
//      sliderPanel();
//      break;
//    case 5:
//      textPanel();
//      break;
//    case 6:
//      embeddedGraphView();
//      break;
//    default:
//      break;
//  }
}

// HACK: this is probably not the right way to do this.
void ModularNodeGraph::getView(const CustomGraphView *self, const function<void(sptr<CustomGraphView>)> &func) {
  if (self != view.get()) throw exception("self is not a child of this.");
  auto shaptr = view->shared_from_this();
  func(shaptr);
}

}
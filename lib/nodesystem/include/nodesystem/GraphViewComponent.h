#pragma once

#include <JuceHeader.h>
#include "NodeComponent.h"
#include "HostNodeComponent.h"
#include "WireComponent.h"
#include "SelectionComponent.h"
#include "UnboundWireComponent.h"
#include "GraphNodeEditor.h"

namespace nodesystem {

class GraphViewComponent : public Component {
#pragma region Public Members
  
public:
  std::unique_ptr<Graph> graph;
  std::vector<std::unique_ptr<NodeComponent>> nodes;
  std::vector<std::unique_ptr<WireComponent>> wires;
  std::unique_ptr<SelectionComponent> selector;
  std::unique_ptr<UnboundWireComponent> wireDrawer;

  bool nodeMultiSelectionOn = false;
  bool wireMultiSelectionOn = false;

  GraphViewTheme theme;

  struct ChildrenMouseListener : public MouseListener {

    GraphViewComponent *view;

    explicit ChildrenMouseListener(GraphViewComponent *view);

    void mouseDown(const MouseEvent &e) override;

    void mouseUp(const MouseEvent &e) override;

    void mouseDrag(const MouseEvent &e) override;

    void mouseEnter(const MouseEvent &e) override;
  };

  std::unique_ptr<ChildrenMouseListener> mouseListener;
  
#pragma endregion Public Members
  
#pragma region Public Methods
  
public:
  GraphViewComponent();

  ~GraphViewComponent() override;

  void paint(Graphics &) override;

  void resized() override;
  
  void mouseDown(const MouseEvent &e) override;

  void mouseDrag(const MouseEvent &e) override;

  void mouseUp(const MouseEvent &e) override;

  bool keyPressed(const KeyPress &key) override;

  void assertions() const;

  NodeComponent *addNode(const std::string &name, 
    const int ins, const int outs,
    const Point<float> position = Point<float>(0, 0));

  HostNodeComponent *addHostNode(std::unique_ptr<GraphNodeEditor> editor, 
    const int ins, const int outs,
    const int width, const int height,
    const Point<float> position = Point<float>(0, 0));

  void removeNode(NodeComponent *n);

  WireComponent *addWire(NodeComponent::PinComponent *source, NodeComponent::PinComponent *target);

  void removeWire(WireComponent *w);

  void pinMouseDrag(NodeComponent::PinComponent *pin, const MouseEvent &e);

  void pinMouseUp(NodeComponent::PinComponent *pin, const MouseEvent &e);

  void pinMouseEnter(NodeComponent::PinComponent *pin, const MouseEvent &e);

  void drawConnector(NodeComponent::PinComponent *pin);

  bool isLegalWire(NodeComponent::PinComponent *w, NodeComponent::PinComponent *end);

  void nodeMouseDown(NodeComponent *node, const MouseEvent &e);

  void nodeMouseUp(NodeComponent *node, const MouseEvent &e);

  void nodeMouseDrag(NodeComponent *node, const MouseEvent &e);

  void calculateWireBounds(WireComponent *wire);

  void wireMouseDown(WireComponent *wire, const MouseEvent &e);

  void wireMouseUp(WireComponent *wire, const MouseEvent &e);

  virtual void popupMenu(const MouseEvent &e) {};

  void duplicate();

  void recordState();

  void removeSelected();

  void selectAll();

  void zoomIn();

  void zoomOut();

  void zoomToOriginalSize();

#pragma endregion Public Methods

private:
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GraphViewComponent)
};

}
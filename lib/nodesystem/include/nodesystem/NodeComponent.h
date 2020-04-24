#pragma once

#include <JuceHeader.h>
#include "Graph.h"
#include "GraphViewTheme.h"

namespace nodesystem {

class NodeDefinition;

class NodeComponent : public std::enable_shared_from_this<NodeComponent>,
  public Component, public Graph::NodeListener {
#pragma region Public Members

public:
  class PinComponent : public Component {

  public:

    const NodeComponent *node;
    const Graph::Pin *model;

    unsigned int cPinBackgroundCurrent;

    PinComponent(const NodeComponent *node, const Graph::Pin *model) : node(node), model(model) {
      cPinBackgroundCurrent = node->theme.cPinBackground;
    }

    void paint(Graphics &g) override {
      Path p;
      p.addRectangle(.0f, .0f, (float) node->theme.pinWidth, (float) node->theme.pinHeight);
      g.setColour(Colour(cPinBackgroundCurrent));
      g.fillPath(p);
    }


  private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PinComponent)
  };

  const GraphViewTheme theme;
  Graph::Node *model;


  std::vector<std::unique_ptr<PinComponent>> ins;
  std::vector<std::unique_ptr<PinComponent>> outs;

  bool selected{};

  unsigned int cNodeBackgroundCurrent;

  float scaleFactor;
  AffineTransform translation;
  AffineTransform scale;

#pragma endregion Public Members
  
#pragma region Public Abstract Methods
  
public:
  virtual NodeDefinition getNodeDefinition();

#pragma endregion Public Abstract Methods
  
#pragma region Public Methods
  
public:

  NodeComponent(const GraphViewTheme &theme, Graph::Node *model);

  ~NodeComponent() override;

  bool hasIns();

  bool hasOuts();

  int boxHeight();

  Rectangle<int> boxBounds();

  virtual void paint(Graphics &g) override;

  virtual void resized() override;

  virtual bool hitTest(int x, int y) override;

  void mouseDown(const MouseEvent &e) override;

  void mouseDrag(const MouseEvent &e) override;

  void mouseUp(const MouseEvent &e) override;

  void mouseMove(const MouseEvent &e) override;

  void mouseDoubleClick(const MouseEvent &e) override;

  void mouseExit(const MouseEvent &e) override;

#pragma endregion Public Methods

private:

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NodeComponent)

};

}
#pragma once

#include <JuceHeader.h>
#include "../../../common.h"
#include "../Graph.h"
#include "../GraphViewTheme.h"

struct NodeDefinition;

class NodeComponent : public Component, public Graph::NodeListener {

#pragma region Public Members
  
public:

#pragma region PinComponent
  
  class PinComponent : public Component {

  public:

    const NodeComponent *node;
    const Graph::Pin *model;

    unsigned int cPinBackgroundCurrent;

    PinComponent(const NodeComponent *node, const Graph::Pin *model) : node(node), model(model) {
      auto flowType = model->flowType;
      switch (flowType) {
        case Graph::FlowDataType::Audio:
          cPinBackgroundCurrent = (Colours::yellow).getARGB();
          break;
        case Graph::FlowDataType::CV:
          cPinBackgroundCurrent = (Colours::red).getARGB();
          break;
        case Graph::FlowDataType::Trigger:
          cPinBackgroundCurrent = (Colours::royalblue).getARGB();
          break;
      }
      // cPinBackgroundCurrent = node->theme.cPinBackground;
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
  
#pragma endregion PinComponent

  /**
   * The theme for this NodeComponent.
   */
  const GraphViewTheme theme;
  /**
   * The Graph::Node instance associated with this NodeComponent.
   */
  Graph::Node *model;

  uptr<NodeDefinition> definition;

  /**
   * Unique references to each of the Pins specified by the Node
   * passed into this NodeComponent via Node::ins.
   */
  std::vector<uptr<PinComponent>> ins;
  /**
   * Unique references to each of the Pins specified by the Node
   * passed into this NodeComponent via Node::outs.
   */
  std::vector<uptr<PinComponent>> outs;

  bool selected{};

  unsigned int cNodeBackgroundCurrent;

  float scaleFactor;
  AffineTransform translation;
  AffineTransform scale;

#pragma endregion Public Members
  
#pragma region Public Methods
  
public:

  NodeComponent(const GraphViewTheme &theme, Graph::Node *model, const NodeDefinition &def);

  ~NodeComponent() override;

  /**
   * Returns true if NodeComponent::ins is greater than zero.
   */
  bool hasIns();

  /**
   * Returns true if NodeComponent::outs is greater than zero. 
   */
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

//
// Created by asorgejr on 4/22/2020.
//

#pragma once

#include <JuceHeader.h>
#include <nodesystem/nodesystem.h>
#include "../NodeInfo.h"
#include "OutListener.h"
#include "NodePanel.h"

namespace nodesystem {

struct EmbeddedGraphView : public Shareable<EmbeddedGraphView>, public GraphNodeEditor {
  EmbeddedGraphView();

  ~EmbeddedGraphView() override;

  struct View : public Shareable<View>, public GraphViewComponent {
    View();

    ~View() override;

    void popupMenu(const MouseEvent &e) override;
  };

  std::unique_ptr<View> view;
  std::unique_ptr<OutListener> listener;
  NodeComponent *in;
  NodeComponent *out;

  Component *guest() override;

  void setModel(Graph::Node *model) override;

  void onData(const Graph::Node *sourceNode, const Graph::Pin *sourcePin, const var &data) override;

};

}
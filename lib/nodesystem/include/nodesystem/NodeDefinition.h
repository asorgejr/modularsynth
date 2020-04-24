//
// Created by asorgejr on 4/24/2020.
//

#pragma once
#include <functional>
#include <JuceHeader.h>

#include "GraphViewComponent.h"

namespace nodesystem {

/// This is used to define the procedure for instantiating a NodeComponent.
/// A typical usage would look like this:
/// \code['{'.cpp'}']
/// [](const std::shared_ptr&lt;nodesystem::GraphViewComponent&gt; &host, 
/// const juce::Point<float> &position) -> void {
/// host->addNode("Presentation Name", 1, 1, position);
/// }
/// \endcode
typedef std::function<
  void(std::shared_ptr<GraphViewComponent>, Point<float>)
> NodeTypeFn;

/**
 * This struct is used to configure the appearance of a nodesystem::NodeComponent.
 */
struct NodeDefinition {
  String name = "";
  int32 ins = 0;
  int32 outs = 0;
  NodeTypeFn instantiate;

  NodeDefinition(const String &name, int32 ins, int32 outs, NodeTypeFn instantiate);
  NodeDefinition(const String &name, int32 ins, int32 outs);
};

}
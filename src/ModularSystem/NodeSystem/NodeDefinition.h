//
// Created by asorgejr on 4/24/2020.
//

#pragma once
#include <functional>
#include <JuceHeader.h>
#include "Graph.h"

class ModularGraphView;
class NodeComponent;

/** This is used to define the procedure for instantiating a NodeComponent.
    A typical usage would look like this:
    \code['{'.cpp'}']
[](const std::shared_ptr&lt;nodesystem::ModularGraphView&gt; &host, const juce::Point<float> &position) -> void
{
  host->addNode("NodeName", 1, 1, position);
}
    \endcode
  */ 
typedef std::function<NodeComponent*(ModularGraphView *, Point<float>)> NodeInstantiator;

typedef Graph::FlowDataType FlowDataType;

typedef std::vector<FlowDataType> FlowDataList;


/**
 * This struct is used to configure a nodesystem::NodeComponent.
 */
struct NodeDefinition {
  
  /**
      The type of this node. This will be shared amongst all Graph::Node instances 
      created by this NodeDefinition.
   */
  const std::string type_name;
  /**
   * The name of this node. This name will be used for both the Graph::Node
   * model and the NodeComponent.
   */
  std::string name = "";
  /**
   * The total number of inputs this node will have.
   */
  int32 ins = 0;
  /**
   * The total number of outputs this node will have.
   */
  int32 outs = 0;
  /**
   * This is used to define the procedure for instantiating a NodeComponent.
   * A typical usage would look like this:
   * \code['{'.cpp'}']
   * [](const std::shared_ptr&lt;nodesystem::GraphViewComponent&gt; &host, 
   * const juce::Point<float> &position) -> void {
   * host->addNode("Presentation Name", 1, 1, position);
   * }
   * \endcode
   */
  NodeInstantiator instantiate;

  FlowDataList inTypes;
  FlowDataList outTypes;

  NodeDefinition(std::string type_name, const std::string &name, FlowDataList inTypes, FlowDataList outTypes, NodeInstantiator instantiate);

  NodeDefinition(const std::string& type_name, FlowDataList inTypes, FlowDataList outTypes, NodeInstantiator instantiate);
  
  NodeDefinition(const std::string &type_name, FlowDataList inTypes, FlowDataList outTypes);
  
};


///**
// * This struct is used to configure a nodesystem::NodeComponent.
// */
//struct HostNodeDefinition : NodeDefinition {
//  /**
//   * The width of this node.
//   */
//  int32 width = 80;
//  /**
//   * The height of this node.
//   */
//  int32 height = 40;
//  
//  HostNodeDefinition(const String &name, int32 ins, int32 outs, int32 width, int32 height, NodeInstantiator instantiate);
//  
//  HostNodeDefinition(const String &name, int32 ins, int32 outs, int32 width, int32 height);
//  
//  HostNodeDefinition(const String &name, int32 ins, int32 outs, NodeInstantiator instantiate);
//  
//  HostNodeDefinition(const String &name, int32 ins, int32 outs);
//  
//  /**
//   * For the sake of conformity, it is recommended that you do not override this method,
//   * however if you want a HostNode to have a customized appearance, then you can define it
//   * using this method.
//   */
//  virtual Path getNodeShape(const Rectangle<int> &bounds) {
//    Path p;
//    p.addRoundedRectangle(bounds, 3);
//    return p;
//  }
//  
//};

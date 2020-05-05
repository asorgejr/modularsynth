#pragma once
#include <JuceHeader.h>
#include "NodeSystemAPI.h"
#include "NodeComponent.h"
#include "HostNodeComponent.h"
#include "WireComponent.h"
#include "SelectionComponent.h"
#include "UnboundWireComponent.h"
#include "NodeViewComponent.h"

namespace nodesystem {

class GraphViewComponent : public Component {
  
#pragma region Public Members
  
public:
  
  uptr<Graph> graph;
  
  std::vector<uptr<NodeComponent>> nodes;
  
  std::vector<uptr<WireComponent>> wires;
  
  // TODO: Implement this!
  //  It will be used to recover from exceptions in removeNode()
  std::vector<std::tuple<sptr<NodeComponent>, Graph::Node *>> nodeTether;
  
  uptr<SelectionComponent> selector;
  
  uptr<UnboundWireComponent> wireDrawer;

  bool nodeMultiSelectionOn = false;
  
  bool wireMultiSelectionOn = false;

  GraphViewTheme theme;

  struct ChildrenMouseListener : public MouseListener {

    GraphViewComponent *view;

    explicit ChildrenMouseListener(GraphViewComponent *view) : view(view) {}

    #pragma region Mouse Events

    void mouseDown(const MouseEvent &e) override {
      if (auto node = dynamic_cast<NodeComponent *>(e.originalComponent)) {
        view->nodeMouseDown(node, e);
      } else if (auto wire = dynamic_cast<WireComponent *>(e.originalComponent)) {
        view->wireMouseDown(wire, e);
      }
    }

    void mouseUp(const MouseEvent &e) override {
      if (auto node = dynamic_cast<NodeComponent *>(e.originalComponent)) {
        view->nodeMouseUp(node, e);
      } else if (auto pin = dynamic_cast<NodeComponent::PinComponent *>(e.originalComponent)) {
        view->pinMouseUp(pin, e);
      } else if (auto wire = dynamic_cast<WireComponent *>(e.originalComponent)) {
        view->wireMouseUp(wire, e);
      }
    }

    void mouseDrag(const MouseEvent &e) override {
      if (auto node = dynamic_cast<NodeComponent *>(e.originalComponent)) {
        view->nodeMouseDrag(node, e);
      } else if (auto pin = dynamic_cast<NodeComponent::PinComponent *>(e.originalComponent)) {
        view->pinMouseDrag(pin, e);
      }
    }

    void mouseEnter(const MouseEvent &e) override {
      if (auto pin = dynamic_cast<NodeComponent::PinComponent *>(e.originalComponent)) {
        view->pinMouseEnter(pin, e);
      }
    }

    #pragma endregion Mouse Events
    
  };

  uptr<ChildrenMouseListener> mouseListener;
  
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

  /** Invokes various methods if the pressed keys match a set of conditions.
      The default invocations are as follows:
      
      cmd '0': zoomToOriginalSize() 
      
      cmd '+' or cmd '=': zoomIn()
      
      cmd '-': zoomOut()
      
      cmd 'a': selectAll()
      
      cmd 'd': duplicate()
      
      @param key the KeyPress event.
      @return 
   */
  virtual bool keyPressed(const KeyPress &key) override;

  void assertions() const;

  /** This method adds a standard NodeComponent to this graph.
      @note This method must be manually invoked in the implementation
      of a derived class. You should call this method in the 
      provided popupMenu() method, or within a lambda function, NodeInstantiator,
      set in NodeComponent::getNodeDefinition().
      @param name: the name of the node.
      @param ins: the number of inputs on the node.
      @param outs: the number of outputs on the node.
      @param position: the initial position of the node.
      @return a reference to the newly added node.
   */
  NodeComponent * addNode(
    const std::string &name, 
    int ins, int outs,
    Point<float> position = Point<float>(0, 0));
  
  /** This method adds a custom HostNodeComponent to this graph.
      @note This method must be manually invoked in the implementation
      of a derived class. You should call this method in the 
      provided popupMenu() method, or within a lambda function, NodeInstantiator,
      set in NodeComponent::getNodeDefinition().
      @param view: the NodeViewComponent used to define the 
      look and behavior of this node.
      @param ins: the number of inputs on the node.
      @param outs: the number of outputs on the node.
      @param width: the width of the node in pixels.
      @param height: the height of the node in pixels.
      @param position: the initial position of the node.
      @return a reference to the newly added node.
 */
  HostNodeComponent * addHostNode(
    std::unique_ptr<NodeViewComponent> view, 
    int ins, int outs,
    int width, int height,
    Point<float> position = Point<float>(0, 0));

  /** This method adds a custom HostNodeComponent to this graph.
      @note This method must be manually invoked in the implementation
      of a derived class. You should call this method in the 
      provided popupMenu() method, or within a lambda function, NodeInstantiator,
      set in HostNodeComponent::getNodeDefinition().
      @param view: the NodeViewComponent used to define the 
      look and behavior of this node.
      @param position: the initial position of the node.
      @return a reference to the newly added node.
   */
  HostNodeComponent * addHostNode(
    std::unique_ptr<NodeViewComponent> view,
    Point<float> position = Point<float>(0, 0));
  
  /** This method is called before any initialization logic occurs in addHostNode().
      @param view: the NodeViewComponent used to define the 
      look and behavior of the node to be created.
      @param position: the initial position of the node to be created.
   */
  virtual void onBeforeAddHostNode(const std::unique_ptr<NodeViewComponent> &view, const Point<float> &position) {}
  /** This method is called after initialization logic has occurred in addHostNode().
      @param node: a reference to the newly added node.
  */
  virtual void onAfterAddHostNode(const HostNodeComponent *node, const Graph::Node *model) {}
  /** This method is called when the user presses the delete or backspace key when a
      node(s) is selected.
      @note If you override this method, you must call the base method LAST. Do not
      handle cleanup logic for parameter 'n', it is handled by the base method. Instead
      of overriding this method, it is recommended that you override onBeforeRemoveNode(), 
      or onAfterRemoveNode().
      @param n: the node to be removed.
   */
  virtual void removeNode(NodeComponent *n);
  /** This method is called before any cleanup logic occurs in removeNode().
      @param n: the node to be removed.
   */
  virtual void onBeforeRemoveNode(const NodeComponent *n) {}
  /** This method is called after all cleanup logic has occurred in removeNode().
   */
  virtual void onAfterRemoveNode() {}

  /** This method adds a WireComponent to this graph.
      It is called in drawConnector() if the drawn connection is deemed a
      valid connection by the method, isLegalWire().
      @param source the origin NodeComponent::PinComponent of the drawn connection.
      @param target the destination NodeComponent::PinComponent of the drawn connection.
      @return the WireComponent which was added to the GraphViewComponent.
   */
  virtual WireComponent *addWire(NodeComponent::PinComponent *source, NodeComponent::PinComponent *target);
  /** This method is called before any initialization logic occurs in addWire().
   */
  virtual void onBeforeAddWire(const NodeComponent::PinComponent *source, const NodeComponent::PinComponent *target) {}
  /** This method is called after initialization logic has occurred in addWire().
    */
  virtual void onAfterAddWire(const NodeComponent::PinComponent *source, const NodeComponent::PinComponent *target) {}

  /** This method is called when the user presses the delete or backspace key when a
    wire(s) is selected.
    @note If you override this method, you must call the base method LAST. Do not
    handle cleanup logic for parameter 'w', it is handled by the base method. Instead
    of overriding this method, it is recommended that you override onBeforeRemoveWire(), 
    or onAfterRemoveNode().
    @param w: the wire to be removed.
   */
  virtual void removeWire(WireComponent *w);
  /** This method is called before any cleanup logic occurs in removeWire().
      @param w: the wire to be removed.
   */
  virtual void onBeforeRemoveWire(const WireComponent *w) {}
  /** This method is called after all cleanup logic has occurred in removeNode().
   */
  virtual void onAfterRemoveWire() {}
  /** Called when the mouse is moved while a button is held down.

      When a mouse button is pressed inside a component, that component
      receives mouseDrag callbacks each time the mouse moves, even if the
      mouse strays outside the component's bounds.
      @note If you override this method, you must call the base method first.
      @param pin the NodeComponent::PinComponent associated with this event.
      @param event  details about the position and status of the mouse event, including
                    the source component in which it occurred
      @see mouseDown, mouseUp, mouseMove, contains, setDragRepeatInterval
  */
  virtual void pinMouseDrag(NodeComponent::PinComponent *pin, const MouseEvent &e);
  /** Called when a mouse button is released.

      A mouseUp callback is sent to the component in which a button was pressed
      even if the mouse is actually over a different component when the
      button is released.

      The MouseEvent object passed in contains lots of methods for finding out
      which buttons were down just before they were released.
      @note If you override this method, you must call the base method first.
      @param pin the NodeComponent::PinComponent associated with this event.
      @param event  details about the position and status of the mouse event, including
                    the source component in which it occurred
      @see mouseDown, mouseDrag, mouseDoubleClick, contains
  */
  virtual void pinMouseUp(NodeComponent::PinComponent *pin, const MouseEvent &e);
  /** Called when the mouse first enters a component.

      If the mouse button isn't pressed and the mouse moves into a component,
      this will be called to let the component react to this.

      When the mouse button is pressed and held down while being moved in
      or out of a component, no mouseEnter or mouseExit callbacks are made - only
      mouseDrag messages are sent to the component that the mouse was originally
      clicked on, until the button is released.
      @note If you override this method, you must call the base method first.
      @param pin the NodeComponent::PinComponent associated with this event.
      @param event details about the position and status of the mouse event, including
                   the source component in which it occurred
      @see mouseExit, mouseDrag, mouseMove, contains
  */
  virtual void pinMouseEnter(NodeComponent::PinComponent *pin, const MouseEvent &e);
  /** Called when the mouse drags away from one NodeComponent::PinComponent to another.
      If the end pin is valid, a new WireComponent will be created.
      @param pin the NodeComponent::PinComponent from which the mouse gesture began.
   */
  virtual void drawConnector(NodeComponent::PinComponent *pin);
  /** This checks if a connection drawn between two pins is valid.
      First it checks if the most recently generated WireComponent is making 
      a connection that contains the start and end parameters. Then it checks if 
      \code start->model->pinType != end->model->pinType \endcode, and,
      \code start->node != end->node \endcode
      @param start: the pin from which the connection began.
      @param end: the pin from which the connection ended.
      @return true if the connection is valid.
   */
  bool isLegalWire(NodeComponent::PinComponent *start, NodeComponent::PinComponent *end);
  /** Called when a mouse button is pressed.

    The MouseEvent object passed in contains lots of methods for finding out
    which button was pressed, as well as which modifier keys (e.g. shift, ctrl)
    were held down at the time.

    Once a button is held down, the mouseDrag method will be called when the
    mouse moves, until the button is released.
    @note If you override this method, you must call the base method first.
    @param node the NodeComponent associated with this event.
    @param e  details about the position and status of the mouse event, including
                  the source component in which it occurred
    @see mouseUp, mouseDrag, mouseDoubleClick, contains
*/
  virtual void nodeMouseDown(NodeComponent *node, const MouseEvent &e);
  /** Called when a mouse button is released.

    A mouseUp callback is sent to the component in which a button was pressed
    even if the mouse is actually over a different component when the
    button is released.

    The MouseEvent object passed in contains lots of methods for finding out
    which buttons were down just before they were released.
    @note If you override this method, you must call the base method first.
    @param node the NodeComponent associated with this event.
    @param event  details about the position and status of the mouse event, including
                  the source component in which it occurred
    @see mouseDown, mouseDrag, mouseDoubleClick, contains
*/
  virtual void nodeMouseUp(NodeComponent *node, const MouseEvent &e);
  /** Called when the mouse is moved while a button is held down.

    When a mouse button is pressed inside a component, that component
    receives mouseDrag callbacks each time the mouse moves, even if the
    mouse strays outside the component's bounds.
    @note If you override this method, you must call the base method first.
    @param node the NodeComponent associated with this event.
    @param event  details about the position and status of the mouse event, including
                  the source component in which it occurred
    @see mouseDown, mouseUp, mouseMove, contains, setDragRepeatInterval
    */
  virtual void nodeMouseDrag(NodeComponent *node, const MouseEvent &e);

  void calculateWireBounds(WireComponent *wire);
  /** Called when a mouse button is pressed.

  The MouseEvent object passed in contains lots of methods for finding out
  which button was pressed, as well as which modifier keys (e.g. shift, ctrl)
  were held down at the time.

  Once a button is held down, the mouseDrag method will be called when the
  mouse moves, until the button is released.
  @note If you override this method, you must call the base method first.
  @param wire the WireComponent associated with this event.
  @param event  details about the position and status of the mouse event, including
                the source component in which it occurred
  @see mouseUp, mouseDrag, mouseDoubleClick, contains
*/
  virtual void wireMouseDown(WireComponent *wire, const MouseEvent &e);
  /** Called when a mouse button is released.

  A mouseUp callback is sent to the component in which a button was pressed
  even if the mouse is actually over a different component when the
  button is released.

  The MouseEvent object passed in contains lots of methods for finding out
  which buttons were down just before they were released.
  @note If you override this method, you must call the base method first.
  @param wire the WireComponent associated with this event.
  @param event  details about the position and status of the mouse event, including
                the source component in which it occurred
  @see mouseDown, mouseDrag, mouseDoubleClick, contains
*/
  virtual void wireMouseUp(WireComponent *wire, const MouseEvent &e);

  /** This method is called when the user right-clicks in this graph.
      The intended purpose of this method is to provide a menu list of 
      the available nodes a user can add to the graph by clicking on one.
      You should invoke addNode() and/or addHostNode() from this method.
      @param e the mouse event that fired.
   */
  virtual void popupMenu(const MouseEvent &e) {};

  void duplicate();

  void recordState();

  void removeSelected();

  void selectAll();

  void zoomIn();

  void zoomOut();

  void zoomToOriginalSize();
  
  int graphHasWire(const WireComponent *wire);
  
  int graphHasNode(const NodeComponent *node);

#pragma endregion Public Methods

private:
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GraphViewComponent)
};

}
#include "WireComponent.h"
#include "../../../common.h"

DISABLE_WARNING_PUSH
DISABLE_WARNING_LOSS_OF_DATA

typedef NodeComponent::PinComponent PinComp;

WireComponent::WireComponent(const GraphViewTheme &theme, PinComp *startPin, PinComp *endPin, const Graph::Wire *model)
: theme(theme),
  startPin(startPin),
  endPin(endPin),
  model(model)
{
  
}

WireComponent::~WireComponent() {}

bool WireComponent::hitTest(int x, int y) {
  auto position = Point<float>(x, y);
  Point<float> nearest;
  path.getNearestPoint(Point<float>(x, y), nearest);
  auto distance = position.getDistanceFrom(nearest);
  return distance < 5;
}


void WireComponent::paint(Graphics &g) {
  auto w = getWidth();
  auto h = getHeight();

  auto pstart = getLocalPoint(startPin, Point<float>(startPin->getWidth() / 2, 0));
  auto pend = getLocalPoint(endPin, Point<float>(endPin->getWidth() / 2, 0));

  path.clear();

  path.startNewSubPath(pstart.x, pstart.y);
  if (inverted) {
    path.cubicTo(w, h * 0.5f, 0.0f, h * 0.5f, pend.x, pend.y);
  } else {
    path.cubicTo(0.0f, h * 0.5f, w, h * 0.5f, pend.x, pend.y);
  }


  if (selected) {
    g.setColour(Colour(theme.cEdgeSelected));
  } else {
    auto color = startPin != nullptr ? Colour(startPin->cPinBackgroundCurrent) : Colour(theme.cEdge);
    g.setColour(color);
  }

  g.strokePath(path, PathStrokeType(theme.edgeStrokeWidth));

  /*
  Path boundry;
  boundry.addRectangle(getLocalBounds());
  g.strokePath(boundry, PathStrokeType(1.0f));
  */

}


bool WireComponent::isConnecting(PinComp *first, PinComp *second) {
  return (startPin == first && endPin == second) || (startPin == second && endPin == first);
}


DISABLE_WARNING_POP
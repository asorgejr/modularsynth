
#pragma once

namespace nodesystem {

struct GraphViewTheme {

  float edgeStrokeWidth = 3.0f;
  int hostComponentDragAreaHeight = 30;
  float initialScaleFactor = 1.0f;
  int nodeHeight = 40;
  int pinHeight = 7;
  int pinSpacing = 10;
  int pinWidth = 10;

  float scaleStep = 0.2f;
  float scaleMax = 2.0f;
  float scaleMin = 0.4f;

  unsigned int cBackground = 0xFF242424;
  unsigned int cEdge = 0xFF767676;
  unsigned int cEdgeSelected = 0xFFE27E2A;
  unsigned int cNodeBackground = 0xCF4F4F4F;
  unsigned int cNodeBackgroundHover = 0xCF656464;
  unsigned int cNodeBackgroundSelected = 0xCFDE4F18;
  unsigned int cPinBackground = 0xFF767676;
  unsigned int cPinBackgroundSelected = 0xFFE27E2A;
  unsigned int cSelectionBackground = 0x3D555555;

};

//inline static GraphViewTheme GraphViewTheme = {
//  .edgeStrokeWidth = 3.0f,
//  .hostComponentDragAreaHeight = 30,
//  .nodeHeight = 40,
//  .pinHeight = 7,
//  .pinSpacing = 10,
//  .pinWidth = 10,
//  .cBackground = 0xFF242424,
//  .cEdge = 0xFF767676,
//  .cEdgeSelected = 0xFFE27E2A,
//  .cNodeBackground = 0xCF4F4F4F,
//  .cNodeBackgroundHover = 0xCF656464,
//  .cNodeBackgroundSelected = 0xCFDE4F18,
//  .cPinBackground = 0xFF767676,
//  .cPinBackgroundSelected = 0xFFE27E2A,
//  .cSelectionBackground = 0x3D555555
//}

}
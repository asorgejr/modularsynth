//
// Created by asorgejr on 4/28/2020.
//
#pragma once
#include <JuceHeader.h>
#include "../common.h"


struct FunctionListener : public ChangeListener {
  
  std::function<void(ChangeBroadcaster*)> callbackFunction;
  
  FunctionListener(const std::function<void(ChangeBroadcaster*)> &callback) {
    callbackFunction = callback;
  }
  
  ~FunctionListener() {}
  
  void changeListenerCallback(ChangeBroadcaster *source) override {
    callbackFunction(source);
  }
  
};

// IModularComponent.h
// compile with: /LD /clr /doc

#pragma once
#include <memory>
#include <JuceHeader.h>

namespace modularsynth {

class IModularComponent {
protected:
  std::shared_ptr<AudioDeviceManager> audioDeviceManager;
  /** 
   * Never call addMouseListener() with this as the argument if the call 
   * is made from a class that derives from IModularComponent. Doing so
   * will cause a recursion.
   */
  std::shared_ptr<MouseListener> sharedMouseListener;
public:
  explicit IModularComponent(std::shared_ptr<AudioDeviceManager> audioDeviceManager, std::shared_ptr<MouseListener> sharedMouseListener)
  : audioDeviceManager(std::move(audioDeviceManager)),
    sharedMouseListener(std::move(sharedMouseListener)) {}
  virtual ~IModularComponent() {};
};

}
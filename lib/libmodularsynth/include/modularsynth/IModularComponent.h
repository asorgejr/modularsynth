// IModularComponent.h
// compile with: /LD /clr /doc

#pragma once
#include <JuceHeader.h>

namespace modularsynth {

class IModularComponent : public Component {
protected:
  std::shared_ptr<AudioDeviceManager> audioDeviceManager;
  /** 
   * Never call addMouseListener() with this as the argument if the call 
   * is made from a class that derives from IModularComponent. Doing so
   * will cause a recursion.
   */
  std::shared_ptr<MouseListener> sharedMouseListener;
public:
  explicit IModularComponent(std::shared_ptr<AudioDeviceManager> audioDeviceManager, std::shared_ptr<MouseListener> sharedMouseListener);
  virtual ~IModularComponent() {};
  virtual void mouseDown(const MouseEvent &event) override;
  virtual void mouseUp(const MouseEvent &event) override;
  virtual void mouseMove(const MouseEvent &event) override;
  virtual void mouseDrag(const MouseEvent &event) override;
  virtual void mouseDoubleClick(const MouseEvent &event) override;
  virtual void mouseEnter(const MouseEvent &event) override;
  virtual void mouseExit(const MouseEvent &event) override;
  virtual void mouseMagnify(const MouseEvent &event, float scaleFactor) override;
  virtual void mouseWheelMove(const MouseEvent &event, const MouseWheelDetails &wheel) override;
};

}
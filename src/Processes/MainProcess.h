#pragma once
#include "System/Context.h"
#include "System/Init.h"
#include "System/Process.h"
#include "System/Window.h"

#include <SDL3/SDL_events.h>

#include <memory>

class MainProcess : public Process
{
public:
  virtual void OnStart() override;
  virtual void OnUpdate(float dt) override;
  virtual void OnResize(int width, int height) override;

  void OnEvent(SDL_Event &event);

  std::unique_ptr<Window> &GetWindow() { return m_Window; }

private:
  int m_m = 0;
  bool m_FreeCursor = true;
  std::unique_ptr<Window> m_Window;
  std::unique_ptr<Context> m_Context;
};
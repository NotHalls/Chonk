#pragma once

#include "GUI/ImGui.h"
#include "System/Context.h"
#include "System/Init.h"
#include "System/Window.h"

#include <SDL3/SDL_events.h>

#include <memory>

class App
{
public:
  App();
  ~App();

  void Start();
  void Update(float dt);
  void OnResize(int width, int height);

  void OnEvent(const SDL_Event &event);

  static const App &Get() { return *m_App; }
  const std::unique_ptr<Window> &GetWindow() const { return m_Window; }
  const std::unique_ptr<Context> &GetContext() const { return m_Context; }

private:
  static App *m_App;
  std::unique_ptr<Window> m_Window;
  std::unique_ptr<Context> m_Context;
  GUI m_Gui;
};
#pragma once

#include "System/Context.h"
#include "System/Init.h"
#include "System/Window.h"

#include <SDL3/SDL_events.h>

#include <memory>

class App
{
public:
  static void Start();
  static void Update(float dt);
  static void OnResize(int width, int height);

  static void OnEvent(SDL_Event &event);

  static std::unique_ptr<Window> &GetWindow() { return m_Window; }

private:
  static bool m_FreeCursor;
  static std::unique_ptr<Window> m_Window;
  static std::unique_ptr<Context> m_Context;
};
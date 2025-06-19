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
  static void Start();
  static void Update(float dt);
  static void OnResize(int width, int height);
  static void Destroy();

  static void OnEvent(const SDL_Event &event);

  static std::unique_ptr<Window> &GetWindow() { return m_Window; }
  static std::unique_ptr<Context> &GetContext() { return m_Context; }

private:
  static bool m_FreeCursor;
  static std::unique_ptr<Window> m_Window;
  static std::unique_ptr<Context> m_Context;
  static GUI m_Gui;
};
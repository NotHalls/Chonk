#pragma once

#include "GUI/EngineGUI.h"
#include "System/Context.h"
#include "System/Init.h"
#include "System/Process.h"
#include "System/Window.h"


#include <SDL3/SDL_events.h>

#include <memory>
#include <vector>

class App
{
public:
  App();
  ~App();

  void Run();

  static const App &Get() { return *m_App; }
  const std::unique_ptr<Window> &GetWindow() const { return m_Window; }
  const std::unique_ptr<Context> &GetContext() const { return m_Context; }

private:
  void Init();
  void OnEvent(const SDL_Event &event);
  void Update(float dt);
  void OnGuiUpdate(float dt);
  void OnResize(int width, int height);

private:
  static App *m_App;
  std::unique_ptr<Window> m_Window;
  std::unique_ptr<Context> m_Context;

  std::vector<std::shared_ptr<Process>> m_Processes;
  std::shared_ptr<EngineGUI> m_EngineGui;

  bool m_IsRunning;
};
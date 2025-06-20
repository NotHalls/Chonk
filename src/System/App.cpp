#include "App.h"
#include "System/Scene.h"

#include <SDL3/SDL_events.h>
#include <glad/glad.h>
#include <imgui.h>

#include <iostream>
#include <string>

// file variables
static bool lockMouse = false;
static float avgFps = 0.0f;
static float avgFrameTime = 0.0f;
static float avgFrequency = 0.02f;

// file functions
static inline void DisplayStatsGUI(float dt)
{
#ifdef CHK_DEBUG
  ImGui::Begin("Stats");
  avgFps = avgFps * (1.0f - avgFrequency) + (1.0f / dt) * avgFrequency;
  avgFrameTime =
      avgFrameTime * (1.0f - avgFrequency) + (dt * 1000.0f) * avgFrequency;
  ImGui::Text("FPS: %.0f", 1.0f / dt);
  ImGui::Text("AVG FPS: %.0f", avgFps);
  ImGui::Separator();
  ImGui::Text("Frame Time: %.3f", dt * 1000.0f);
  ImGui::Text("AVG Frame Time: %.3f", avgFrameTime);
  ImGui::End();
#else
  ImGui::Begin("Stats");
  avgFps = avgFps * (1.0f - avgFrequency) + (1.0f / dt) * avgFrequency;
  ImGui::Text("AVG FPS: %.0f", avgFps);
  ImGui::End();
#endif
}

// class variables (static)
App *App::m_App = nullptr;

// class functions
App::App() { Start(); }
App::~App() {}

void App::Start()
{
  m_App = this;
  SDL_Init();
  m_Window = std::make_unique<Window>(1280, 720);
  m_Context = std::make_unique<Context>(*m_Window);
  GLAD_Init();
  m_Gui.OnStart();

  Scene::Init(45.0f, 0.01f, 100.0f, 1280, 720);
  OnResize(1280, 720);
}

void App::Update(float dt)
{
  GUI::Begin();

  Scene::GetCamera()->OnUpdate(dt);

  Scene::StartScene();
  Scene::StopScene();

  DisplayStatsGUI(dt);

  GUI::End();
  m_Window->Update();
}

void App::OnResize(int width, int height)
{
  glViewport(0, 0, width, height);
  Scene::GetCamera()->OnResize(width, height);
}

void App::OnEvent(const SDL_Event &event)
{
  switch(event.type)
  {
  case SDL_EVENT_KEY_DOWN: {
    if(event.key.scancode == SDL_SCANCODE_F)
    {
      lockMouse = !lockMouse;
      m_Window->ToggleCursorLock(lockMouse);
    }
#ifdef CHK_DEBUG
    else if(event.key.scancode == SDL_SCANCODE_X)
    {
      Scene::GetCamera()->LockInput = !Scene::GetCamera()->LockInput;
      m_Window->ToggleCursorLock(!Scene::GetCamera()->LockInput);
      lockMouse = !(Scene::GetCamera()->LockInput);
    }
#endif
    break;
  }
  case SDL_EVENT_WINDOW_RESIZED: {
    OnResize(event.window.data1, event.window.data2);
    break;
  }
  }
  m_Gui.OnEvent(event);
  Scene::GetCamera()->OnEvent(event);
}
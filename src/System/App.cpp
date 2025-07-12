#include "App.h"
#include "Renderer/GladFunctions.h"
#include "System/Scene.h"
#include "System/Settings.h"

#include <SDL3/SDL.h>
#include <glad/glad.h>
#include <imgui.h>

#include <chrono>
#include <iostream>
#include <print>
#include <string>

// file variables
static float avgFps = 0.0f;
static float avgFrameTime = 0.0f;
static float avgFrequency = 0.02f;

static glm::vec3 previousPlayerPosition = {0.0f, 0.0f, 0.0f};

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
  avgFrameTime =
      avgFrameTime * (1.0f - avgFrequency) + (dt * 1000.0f) * avgFrequency;
  ImGui::Text("AVG Frame Time: %.3f", avgFrameTime);
  ImGui::End();
#endif
}

// class variables (static)
App *App::m_App = nullptr;

// class functions
App::App() : m_IsRunning(true) { Init(); }
App::~App() {}

void App::Init()
{
  m_App = this;
  SDL_Init();
  m_Window = std::make_unique<Window>(1280, 720);
  m_Context = std::make_unique<Context>(*m_Window);
  GLAD_Init();
  m_Gui.OnStart();

  Scene::Init(45.0f, 0.01f, 1000.0f, 1280, 720);
  OnResize(1280, 720);

  Settings::Init();

  m_Window->ToggleCursor(true);
}

void App::Run()
{
  auto startTime = std::chrono::high_resolution_clock::now();
  SDL_Event event;

  while(m_IsRunning)
  {
    auto curtTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> delta = curtTime - startTime;
    startTime = curtTime;
    float dt = delta.count();

    while(SDL_PollEvent(&event))
    {
      OnEvent(event);
      if(event.type == SDL_EVENT_QUIT)
        m_IsRunning = false;
    }

    Update(dt);
  }

  SDL_Quit();
}

void App::Update(float dt)
{
  GUI::Begin();

  Scene::GetCamera()->OnUpdate(dt);

  Scene::StartScene();
  Scene::StopScene();

  OnGuiUpdate(dt);

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
    if(event.key.scancode == SDL_SCANCODE_ESCAPE)
    {
      Scene::GetCamera()->LockInput = !Scene::GetCamera()->LockInput;
      m_Window->ToggleCursor(!Scene::GetCamera()->LockInput);
    }
    if(event.key.scancode == SDL_SCANCODE_X)
    {
      if(!Scene::GetCamera()->Spectating)
        previousPlayerPosition = Scene::GetCamera()->GetPosition();
      else
        Scene::GetCamera()->SetPosition(previousPlayerPosition);

      Scene::GetCamera()->Spectating = !Scene::GetCamera()->Spectating;
    }
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

void App::OnGuiUpdate(float dt)
{
  if(ImGui::BeginMainMenuBar())
  {
    if(ImGui::BeginMenu("Game"))
    {
      if(ImGui::MenuItem("Settings"))
      {
        Settings::Visible = !Settings::Visible;
      }
      ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();
  }

  DisplayStatsGUI(dt);
  Settings::UpdateGUI();
}
#include "App.h"
#include "System/Scene.h"

#include <SDL3/SDL_events.h>
#include <glad/glad.h>

#include <iostream>

bool lockMouse = false;

bool App::m_FreeCursor = true;
std::unique_ptr<Window> App::m_Window = nullptr;
std::unique_ptr<Context> App::m_Context = nullptr;

void App::Start()
{
  SDL_Init();
  m_Window = std::make_unique<Window>(800, 600);
  m_Context = std::make_unique<Context>(*m_Window);
  GLAD_Init();

  Scene::Init(45.0f, 0.01f, 100.0f, 800, 600);
}

void App::Update(float dt)
{
  Scene::GetCamera()->OnUpdate(dt);

  Scene::StartScene();
  Scene::StopScene();
}

void App::OnResize(int width, int height)
{
  glViewport(0, 0, width, height);
  Scene::GetCamera()->OnResize(width, height);
}

void App::OnEvent(SDL_Event &event)
{
  Scene::GetCamera()->OnEvent(event);
  switch(event.type)
  {
  case SDL_EVENT_KEY_DOWN: {
    if(event.key.scancode == SDL_SCANCODE_F)
    {
      lockMouse = !lockMouse;
      m_Window->ToggleCursorLock(lockMouse);
    }
    break;
  }
  case SDL_EVENT_WINDOW_RESIZED: {
    OnResize(event.window.data1, event.window.data2);
    break;
  }
  }
}
#include "MainProcess.h"
#include "System/Scene.h"

#include <SDL3/SDL_events.h>
#include <glad/glad.h>

#include <iostream>

bool lockMouse = false;

void MainProcess::OnStart()
{
  SDL_Init();
  m_Window = std::make_unique<Window>(800, 600);
  m_Context = std::make_unique<Context>(*m_Window);
  GLAD_Init();
}

void MainProcess::OnUpdate(float dt) {}

void MainProcess::OnResize(int width, int height)
{
  glViewport(0, 0, width, height);
}

void MainProcess::OnEvent(SDL_Event &event)
{
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
    Scene::GetCamera()->OnResize(event.window.data1, event.window.data2);
    break;
  }
  }
}
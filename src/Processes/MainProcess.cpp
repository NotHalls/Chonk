#include "MainProcess.h"

#include <SDL3/SDL_events.h>
#include <glad/glad.h>

#include <iostream>

void MainProcess::OnStart()
{
  SDL_Init();
  m_Window = std::make_unique<Window>(800, 600);
  m_Context = std::make_unique<Context>(*m_Window);
  GLAD_Init();
}

void MainProcess::OnUpdate() {}

void MainProcess::OnResize(int width, int height)
{
  std::cout << "Resized To: " << width << height << "\n";
}

void MainProcess::OnEvent(SDL_Event &event)
{
  if(event.type == SDL_EVENT_KEY_DOWN)
  {
    if(event.key.scancode == SDL_SCANCODE_F)
    {
      m_Window->ToggleCursorLock(true);
    }
  }
}
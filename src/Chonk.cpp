#include "System/App.h"
#include "System/Init.h"

#include <SDL3/SDL.h>

#include <chrono>
#include <iostream>

int main()
{
  App::Start();

  bool IsRunning = true;
  SDL_Event Event;

  auto startTime = std::chrono::high_resolution_clock::now();

  while(IsRunning)
  {
    auto curtTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> delta = curtTime - startTime;
    startTime = curtTime;
    float dt = delta.count();
    float fps = 1 / dt;
    float frameTime = dt * 1000.0f;

    while(SDL_PollEvent(&Event))
    {
      App::OnEvent(Event);
      if(Event.type == SDL_EVENT_QUIT)
        IsRunning = false;
    }

    App::Update(dt);

    std::cout << "FPS: " << (int)fps << " (" << frameTime << " ms)"
              << std::endl;
  }

  SDL_Quit();

  return 0;
}

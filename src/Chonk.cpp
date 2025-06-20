#include "System/App.h"
#include "System/Init.h"

#include <SDL3/SDL.h>

#include <chrono>
#include <iostream>

int main()
{
  App app;

  bool IsRunning = true;
  SDL_Event Event;

  auto startTime = std::chrono::high_resolution_clock::now();

  while(IsRunning)
  {
    auto curtTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> delta = curtTime - startTime;
    startTime = curtTime;
    float dt = delta.count();

    while(SDL_PollEvent(&Event))
    {
      app.OnEvent(Event);
      if(Event.type == SDL_EVENT_QUIT)
        IsRunning = false;
    }

    app.Update(dt);
  }

  SDL_Quit();

  return 0;
}

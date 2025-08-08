#pragma once

#include <SDL3/SDL_events.h>

class Process
{
public:
  virtual void OnStart() {}
  virtual void OnUpdate(float dt) {}
  virtual void OnEvent(const SDL_Event &event) {}
};
#pragma once

#include "System/Process.h"

#include <SDL3/SDL_events.h>

class GUI : public Process
{
public:
  void Destroy();

  virtual void OnStart() override;
  virtual void OnUpdate(float dt) override;
  static void OnEvent(const SDL_Event &event);

  static void Begin();
  static void End();
};
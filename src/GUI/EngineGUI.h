#pragma once

#include "System/Process.h"

#include <SDL3/SDL_events.h>

class EngineGUI : public Process
{
public:
  ~EngineGUI();

  virtual void OnStart() override;
  virtual void OnUpdate(float dt) override;
  virtual void OnEvent(const SDL_Event &event) override;

  void Begin();
  void End();
};
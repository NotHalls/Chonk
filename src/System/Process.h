#pragma once

class Process
{
public:
  virtual void OnStart() {}
  virtual void OnUpdate(float dt) {}
};
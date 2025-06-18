#pragma once

class Process
{
public:
  virtual void OnStart() {}
  virtual void OnUpdate(float dt) {}
  virtual void OnResize(int width, int height) {}
};
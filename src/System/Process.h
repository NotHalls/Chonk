#pragma once

class Process
{
public:
  virtual void OnStart() {}
  virtual void OnUpdate() {}
  virtual void OnResize(int width, int height) {}
};
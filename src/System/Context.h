#pragma once
#include "System/Window.h"

class Context
{
public:
  Context(Window &window);
  ~Context();

  void Init(Window &window);
  void *GetRaw() const;

private:
  void *m_Context;
};
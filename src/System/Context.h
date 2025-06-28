#pragma once

#include "System/Window.h"

#include <memory>

class Context
{
public:
  Context(Window &window);
  ~Context();

  void Init(Window &window);
  void *GetRaw() const { return m_Context; }

private:
  void *m_Context;
};
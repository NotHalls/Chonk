#pragma once

#include <cstdint>

// class SDL_Window;

class Window
{
public:
  Window(int width, int height);
  ~Window();

  void Resize(int width, int height);
  void ToggleCursor(bool mode);

  void Update();

  void Init();
  void *GetRaw() const { return m_Window; }

private:
  void *m_Window;
  int m_Width, m_Height;
};
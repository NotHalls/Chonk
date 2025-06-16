#pragma once

class SDL_Window;

class Window
{
public:
  Window(int width, int height);
  ~Window();

  void Resize(int width, int height);

  void Init();
  SDL_Window *Get() const { return m_Window; }

private:
  SDL_Window *m_Window;
  int m_Width, m_Height;
};
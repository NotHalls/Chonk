#include "Debug/Assert.h"
#include "Window.h"

#include <SDL3/SDL.h>

#include <iostream>
#include <string>

Window::Window(int width, int height) : m_Width(width), m_Height(height)
{
  Init();
}
Window::~Window() { SDL_DestroyWindow(static_cast<SDL_Window *>(m_Window)); }

void Window::Init()
{
#ifdef CHK_DEBUG
  const char *windowName = "Chonk - Debug";
#elif CHK_RELEASE
  const char *windowName = "Chonk - Release";
#endif
  m_Window = SDL_CreateWindow(windowName, m_Width, m_Height,
                              SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
  CHK_ASSERT(m_Window,
             "Failed To Create A Window: " + std::string(SDL_GetError()));
}

void Window::Update()
{
  SDL_GL_SwapWindow(static_cast<SDL_Window *>(m_Window));
}

void Window::Resize(int width, int height)
{
  std::cout << "Resized To: " << width << height << "\n";
}

void Window::ToggleCursor(bool mode)
{
  SDL_SetWindowRelativeMouseMode(static_cast<SDL_Window *>(m_Window), mode);
}
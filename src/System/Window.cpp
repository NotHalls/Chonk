#include "Window.h"
#include "Debug/Assert.h"

#include <SDL3/SDL_video.h>

#include <iostream>
#include <string>

Window::Window(int width, int height) : m_Width(width), m_Height(height)
{
  Init();
}
Window::~Window() { SDL_DestroyWindow(m_Window); }

void Window::Init()
{
  m_Window = SDL_CreateWindow("Chonk", m_Width, m_Height,
                              SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
  CHK_ASSERT(m_Window,
             "Failed To Create A Window: " + std::string(SDL_GetError()));
}
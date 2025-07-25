#include "Context.h"
#include "Debug/Assert.h"

#include <SDL3/SDL_video.h>

#include <iostream>

Context::Context(Window &window) { Init(window); }
Context::~Context() { SDL_GL_DestroyContext((SDL_GLContext)m_Context); }

void Context::Init(Window &window)
{
  SDL_GLContext context =
      SDL_GL_CreateContext(static_cast<SDL_Window *>(window.GetRaw()));
  CHK_ASSERT(context,
             "Failed To Create SDL_GLContext: " + std::string(SDL_GetError()));

  if(!SDL_GL_MakeCurrent(static_cast<SDL_Window *>(window.GetRaw()), context))
  {
    SDL_GL_DestroyContext(context);
    SDL_DestroyWindow(static_cast<SDL_Window *>(window.GetRaw()));
    CHK_ASSERT(true, "Failed To Make GL Context Current: " +
                         std::string(SDL_GetError()));
  }
  m_Context = context;
}
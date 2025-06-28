#include "Debug/Assert.h"
#include "Init.h"

#include <SDL3/SDL_init.h>
#include <SDL3/SDL_video.h>
#include <glad/glad.h>

#include <string>

void SDL_Init()
{
  if(!SDL_Init(SDL_INIT_VIDEO))
  {
    CHK_ASSERT(true,
               "Failed To Init SDL_VIDEO: " + std::string(SDL_GetError()));
  }

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
}

void GLAD_Init()
{
  if(!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    CHK_ASSERT(true, "Failed To Init Glad");

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CCW);
  glViewport(0, 0, 800, 600);
}
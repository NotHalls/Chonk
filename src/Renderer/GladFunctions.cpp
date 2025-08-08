#include "GladFunctions.h"

#include <SDL3/SDL.h>
#include <glad/glad.h>

void ToggleWireframeMode(bool value)
{
  if(value == true)
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  else if(value == false)
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void ToggleVSync(bool value)
{
  if(value == true)
    SDL_GL_SetSwapInterval(1);
  else if(value == false)
    SDL_GL_SetSwapInterval(0);
}
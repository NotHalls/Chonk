#include "GladFunctions.h"

#include <glad/glad.h>

void ToggleWireframeMode(bool value)
{
  switch(value)
  {
  case true: {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    break;
  }
  case false: {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    break;
  }
  }
}
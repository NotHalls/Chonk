#include "GladFunctions.h"

#include <glad/glad.h>

void ToggleWireframeMode(bool value)
{
  if(value == true)
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  else if(value == false)
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
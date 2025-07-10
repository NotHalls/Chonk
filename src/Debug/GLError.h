#pragma once

#include "Debug/Assert.h"

#include <glad/glad.h>

#include <iostream>

#ifdef CHK_DEBUG
#define CheckGLErrors(x)                                                       \
  clearErrors();                                                               \
  x;                                                                           \
  CHK_ASSERT(checkGLErrors(__func__, __FILE__, __LINE__), "\n:(\n");

static void clearErrors()
{
  while(glGetError() != GL_NO_ERROR)
    ;
}

static bool checkGLErrors(const char *function, const char *file, int line)
{
  while(GLenum error = glGetError())
  {
    std::cerr << "FUCK! ERROR WITH OPENGL: " << error << "\tIn FIle: " << file
              << "\tIn Line: " << line << std::endl;

    return false;
  }
  return true;
}
#elif CHK_RELEASE
#define CheckGLErrors(x) x;
#endif
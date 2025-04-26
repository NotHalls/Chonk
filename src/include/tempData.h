#pragma once

#include <cstdint>

// clang-format off
float cubeVertices[] = {
  -0.5f, -0.5f, 0.0f,
   0.5f, -0.5f, 0.0f,
   0.5f,  0.5f, 0.0f,
  -0.5f,  0.5f, 0.0f
};

uint32_t cubeIndices[] = {
  0, 1, 2,
  2, 3, 0
};

// clang-format on

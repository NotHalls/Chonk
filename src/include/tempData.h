#pragma once

#include <cstdint>

// clang-format off
float cubeVertices[] = { // 6 faces × 4 vertices per face = 24 vertices
  // positions          // UVs
  // Front face
  -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
   0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
   0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
  -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,

  // Back face
  -0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
   0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
   0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
  -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,

  // Left face
  -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
  -0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
  -0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
  -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,

  // Right face
   0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
   0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
   0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
   0.5f,  0.5f, -0.5f,  1.0f, 1.0f,

  // Top face
  -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
   0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
   0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
  -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,

  // Bottom face
  -0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
   0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
   0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
  -0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
};


uint32_t cubeIndices[] = {
  // Front
  0, 1, 2, 2, 3, 0,

  // Back
  4, 5, 6, 6, 7, 4,

  // Left
  8, 9, 10, 10, 11, 8,

  // Right
  12, 13, 14, 14, 15, 12,

  // Top
  16, 17, 18, 18, 19, 16,

  // Bottom
  20, 21, 22, 22, 23, 20
};
// clang-format on

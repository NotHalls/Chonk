#pragma once

#include <cstdint>
#include <vector>

// clang-format off
std::vector<float> __CubeVertices__ = {// 6 faces × 4 vertices per face = 24 vertices
  // positions          // UVs       // FaceID
  // Front face
  -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f,
   0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  1.0f,
   0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  1.0f,
  -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  1.0f,

  // Back face
  -0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  1.0f,
   0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  1.0f,
   0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  1.0f,
  -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  1.0f,

  // Left face
  -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  1.0f,
  -0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  1.0f,
  -0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  1.0f,
  -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  1.0f,

  // Right face
   0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  1.0f,
   0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f,
   0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  1.0f,
   0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  1.0f,

  // Top face
  -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,  0.0f,
   0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,
   0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,
  -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,

  // Bottom face
  -0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  2.0f,
   0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  2.0f,
   0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  2.0f,
  -0.5f, -0.5f, -0.5f,  1.0f, 1.0f,  2.0f
};

std::vector<uint32_t> __CubeIndices__ = {
  // Front face (looking toward +Z)
  0, 1, 2,
  2, 3, 0,

  // Back face (looking toward -Z)
  4, 6, 5,
  6, 4, 7,

  // Left face (looking toward -X)
  8, 9, 10,
  10, 11, 8,

  // Right face (looking toward +X)
  12, 14, 13,
  14, 12, 15,

  // Top face (looking toward +Y)
  16, 17, 18,
  18, 19, 16,

  // Bottom face (looking toward -Y)
  20, 22, 21,
  22, 20, 23
};
// clang-format on

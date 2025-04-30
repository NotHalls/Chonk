#pragma once

#include <cstdint>

#include <glm/glm.hpp>

// clang-format off
enum class BlockDirection : uint8_t
{
  Top,      // +Y
  Bottom,   // -Y
  Left,     // -X
  Right,    // +X
  Front,    // +Z
  Back      // -Z
};

enum class BlockID : uint8_t
{
  None = 0,
  Grass,
  Moss,
  Dirt,
  Sand,
  Stone,
  Coal,
  Log,
  Leaf
};
// clang-format on

struct Vertice
{
  glm::vec3 Position;
  glm::vec2 UV;
  float FaceID;
};
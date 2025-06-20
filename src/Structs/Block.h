#pragma once

#include <glm/glm.hpp>

#include <array>
#include <cstdint>
#include <unordered_map>

// clang-format off
const glm::ivec3 FaceDirection[6] = {
  { 0,  0, -1},  // Front
  { 0,  0,  1},  // Back
  {-1,  0,  0},  // Left
  { 1,  0,  0},  // Right
  { 0,  1,  0},  // Top
  { 0, -1,  0}   // Bottom
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

struct Block
{
  BlockID ID;

  uint8_t TopTexID;
  uint8_t SideTexID;
  uint8_t BottomTexID;
};
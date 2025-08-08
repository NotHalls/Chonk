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
  Air,
  Grass,
  Moss,
  Dirt,
  Sand,
  Stone,
  Coal,
  Log,
  Leaf
};

struct Block
{
  BlockID ID = BlockID::None;

  uint8_t TopTexID = 0;
  uint8_t SideTexID = 0;
  uint8_t BottomTexID = 0;
} typedef Block;

// this is a set of values that tell us what side to add or sub according to
// the index. (used for greedy meshing)
constexpr const glm::ivec3 NextBlockFromFaceIndex[6] = {
    { 0, 0, -1},  // Front
    { 0, 0,  1},  // Back
    {-1, 0,  0},  // Left
    { 1, 0,  0},  // Right
    { 0, 1,  0},  // Top
    { 0, -1, 0}   // Bottom
};
// clang-format on

struct BaseFaceTemplate
{
  glm::vec3 Positions[4];
  glm::vec2 UVs[4];
  uint8_t FaceID;
};

// this is what we push into vertices for each face index
constexpr const std::array<BaseFaceTemplate, 6> FaceTemplates = {
    // Front Face
    BaseFaceTemplate{{glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f),
                      glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)},
                     {glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 0.0f),
                      glm::vec2(1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
                     1},

    // Back Face
    BaseFaceTemplate{{glm::vec3(1.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f),
                      glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f)},
                     {glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 0.0f),
                      glm::vec2(1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
                     1},
    // Left Face
    BaseFaceTemplate{{glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f),
                      glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 1.0f)},
                     {glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 0.0f),
                      glm::vec2(1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
                     1},
    // Right Face
    BaseFaceTemplate{{glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 1.0f),
                      glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 0.0f)},
                     {glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 0.0f),
                      glm::vec2(1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
                     1},
    // Top Face
    BaseFaceTemplate{{glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f),
                      glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 1.0f)},
                     {glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 0.0f),
                      glm::vec2(1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
                     0},
    // Bottom Face
    BaseFaceTemplate{{glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f),
                      glm::vec3(1.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f)},
                     {glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 0.0f),
                      glm::vec2(1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
                     2}};

// this piece of code gives us the uvs for each vertice.
constexpr const glm::vec2 CalculateBlockTextureUVs(int texID,
                                                   const glm::vec2 &uvs)
{
  const int numRows = 5;
  const int numCols = 5;

  int col = texID % numCols;
  int row = texID / numCols;

  glm::vec2 tileOffset =
      glm::vec2(float(col) / float(numCols), float(row) / float(numRows));
  glm::vec2 tileScale = glm::vec2(1 / float(numCols), 1 / float(numRows));

  return glm::vec2(tileOffset + uvs * tileScale);
}

// clang-format off
constexpr const glm::ivec3 GetBlockTextureFromID(BlockID id)
{
  switch(id)
  {
    case BlockID::None:   return {0, 0, 0};
    case BlockID::Air:   return {0, 0, 0};
    case BlockID::Grass:  return {1, 2, 3};
    case BlockID::Moss:   return {1, 1, 1};
    case BlockID::Dirt:   return {3, 3, 3};
    case BlockID::Stone:  return {4, 4, 4};
    default: return {0, 0, 0};
  };
}
// clang-format on
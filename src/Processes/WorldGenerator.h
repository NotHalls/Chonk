#pragma once

#include "Renderer/Chunk.h"
#include "System/Process.h"

#include <glm/vec3.hpp>

#include <functional>
#include <memory>
#include <unordered_map>

/// @todo: the operators related to glm should be in a seperate file
struct IVec3Hasher
{
  std::size_t operator()(const glm::ivec3 &v) const
  {
    std::size_t h1 = std::hash<int>{}(v.x);
    std::size_t h2 = std::hash<int>{}(v.y);
    std::size_t h3 = std::hash<int>{}(v.z);
    return h1 ^ (h2 << 1) ^ (h3 << 2);
  }
};
// struct IVec3Hasher
// {
//   std::size_t operator()(const glm::ivec3 &v) const
//   {
//     std::size_t seed = 0;
//     std::hash<int> hasher;

//     // This is a standard "hash_combine" pattern. It mixes the bits of
//     // the incoming hashes much more thoroughly, making collisions far less
//     // likely.
//     seed ^= hasher(v.x) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
//     seed ^= hasher(v.y) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
//     seed ^= hasher(v.z) + 0x9e3779b9 + (seed << 6) + (seed >> 2);

//     return seed;
//   }
// };
// namespace glm
// {
// inline bool operator==(const glm::ivec3 &a, const glm::ivec3 &b)
// {
//   return a.x == b.x && a.y == b.y && a.z == b.z;
// }
// } // namespace glm

class World
{
public:
  static void GenerateWorld();

  static const std::unordered_map<glm::ivec3, std::shared_ptr<Chunk>,
                                  IVec3Hasher> &
  GetChunks()
  {
    return m_Chunks;
  }
  static const std::shared_ptr<Chunk> &GetChunkAtPos(const glm::ivec3 &pos);
  static const Block &GetChunkBlockAtPos(const glm::ivec3 &chunkPos,
                                         const glm::ivec3 &blockPos);
  static bool CheckChunkAtPos(const glm::ivec3 &pos);

  static void UpdateGUI();

private:
  static std::unordered_map<glm::ivec3, std::shared_ptr<Chunk>, IVec3Hasher>
      m_Chunks;
};
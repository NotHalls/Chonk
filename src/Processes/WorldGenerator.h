#pragma once

#include "Renderer/Chunk.h"
#include "Structs/Util.h"
#include "System/Process.h"

#include <glm/vec3.hpp>

#include <functional>
#include <memory>
#include <unordered_map>

class World
{
public:
  static void GenerateWorld();
  static void LoadChunk(const glm::ivec3 &pos);
  /// @brief Unloads a chunk at specified position
  static void UnloadChunk(const glm::ivec3 &pos);
  /// @brief Unloads chunks outside the Settings::RenderDistance
  static void UnloadUnseenChunks();
  /// @brief Unloads and Loads the chunk at a given position (IF IT EXISTS)
  static void ReloadChunk(const glm::ivec3 &pos);

  /// @brief Removes all the chunks form the World
  static void ClearChunks();

  /// @brief Checks if given Chunk has neighbours and updates them
  static void UpdateChunkNeighbours(const glm::ivec3 &pos);

  static const std::unordered_map<glm::ivec3, std::shared_ptr<Chunk>,
                                  Util::IVec3Hasher> &
  GetChunks()
  {
    return m_Chunks;
  }
  static const std::shared_ptr<Chunk> &GetChunkAtPos(const glm::ivec3 &pos);
  static const Block &GetChunkBlockAtPos(const glm::ivec3 &blockPos);
  static inline bool CheckChunkAtPos(const glm::ivec3 &pos);

  static void UpdateGUI();

private:
  static std::unordered_map<glm::ivec3, std::shared_ptr<Chunk>,
                            Util::IVec3Hasher>
      m_Chunks;
};
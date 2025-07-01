#pragma once

#include "Structs/Block.h"

#include <glm/glm.hpp>

#include <array>
#include <cstdint>
#include <vector>

/// @todo: after optimization tests make Y: 256
constexpr size_t CHUNK_SIZE_X = 16;
constexpr size_t CHUNK_SIZE_Y = 16;
constexpr size_t CHUNK_SIZE_Z = 16;
constexpr size_t CHUNK_VOLUME = CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z;

class Chunk
{
public:
  Chunk(const glm::ivec3 &pos);
  ~Chunk();

  void Bind();
  void Unbind();

  const glm::vec3 &GetPosition() const { return m_Position; }
  void SetPosition(const glm::vec3 &pos) { m_Position = pos; }
  int GetBlockIndexFromPos(const glm::ivec3 &pos) const;
  glm::ivec3 GetBlockPosFromIndex(int index) const;
  const Block &GetBlockAtPos(const glm::ivec3 &pos);

  /// @brief returns the fixed chunk size for all the chunks
  /// @todo chunk sizes should be a global variable
  static glm::ivec3 GetFixedChunkSize()
  {
    return {CHUNK_SIZE_X, CHUNK_SIZE_Y, CHUNK_SIZE_Z};
  }

  void Init();
  /// @brief Generates and sets all the block types in each chunk
  void GenerateChunkBlocks();
  /// @brief Generates all the faces inside a chunk
  void GenerateChunkFaces();
  void Draw();

  bool IsBlockOuterChunk(const glm::ivec3 &pos) const;

private:
  void AddVertices(int x, int y, int z, int faceIndex, BlockID id);
  void GenerateMesh();

private:
  uint32_t m_VAO;
  uint32_t m_VBO;
  uint32_t m_IBO;

  glm::vec3 m_Position;

  // std::array<Block, CHUNK_VOLUME> m_Blocks;
  std::vector<Block> m_Blocks = std::vector<Block>(CHUNK_VOLUME);
  std::vector<float> m_Vertices;
  std::vector<uint32_t> m_Indices;
  uint32_t m_CurrentVerticeCount = 0;
};
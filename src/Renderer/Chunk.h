#pragma once

#include "FixedGlobals.h"
#include "Structs/Block.h"

#include <glm/glm.hpp>

#include <array>
#include <cstdint>
#include <vector>

class Chunk
{
public:
  Chunk(const glm::ivec3 &pos);
  ~Chunk();

  void Bind();
  void Unbind();

  const glm::ivec3 &GetPosition() const { return m_Position; }
  int GetBlockIndexFromPos(const glm::ivec3 &pos) const;
  glm::ivec3 GetBlockPosFromIndex(int index) const;
  const Block &GetBlockAtPos(const glm::ivec3 &pos);

  void Init();
  /// @brief Generates all the faces inside a chunk
  void GenerateChunkFaces();
  void Draw();

  bool IsBlockOuterChunk(const glm::ivec3 &pos) const;

private:
  void GenerateChunkBlocks();
  void AddVertices(int x, int y, int z, int faceIndex, BlockID id);
  void GenerateMesh();

public:
  bool Dirty;

private:
  uint32_t m_VAO;
  uint32_t m_VBO;
  uint32_t m_IBO;

  glm::ivec3 m_Position;

  // std::array<Block, CHUNK_VOLUME> m_Blocks;
  std::vector<Block> m_Blocks;
  std::vector<float> m_Vertices;
  std::vector<uint32_t> m_Indices;
  uint32_t m_CurrentVerticeCount;
};
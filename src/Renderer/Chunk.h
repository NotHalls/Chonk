#pragma once

#include <array>
#include <cstdint>
#include <vector>

#include <glm/glm.hpp>

#include "Structs/Block.h"

/// @todo: after optimization tests make Y: 256
constexpr size_t CHUNK_SIZE_X = 16;
constexpr size_t CHUNK_SIZE_Y = 16;
constexpr size_t CHUNK_SIZE_Z = 16;
constexpr size_t CHUNK_VOLUME = CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z;

class Chunk
{
public:
  Chunk();
  ~Chunk() {}

  void Bind();
  void Unbind();

  const glm::vec3 &GetPosition() const { return m_Position; }
  void SetPosition(const glm::vec3 &pos) { m_Position = pos; }
  const int GetBlockIndex(int x, int y, int z) const;

  void Init();
  void RegenerateChunk();
  void GenerateMesh();
  void Draw();

private:
  void addVertices(int x, int y, int z, int faceIndex);

private:
  uint32_t m_VAO;
  uint32_t m_VBO;
  uint32_t m_IBO;

  glm::vec3 m_Position;

  std::array<Block, CHUNK_VOLUME> m_Blocks;
  std::vector<float> m_Vertices;
  std::vector<uint32_t> m_Indices;
  uint32_t m_CurrentVerticeCount = 0;
};
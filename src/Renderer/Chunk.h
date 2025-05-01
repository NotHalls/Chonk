// #pragma once

// #include <cstdint>
// #include <vector>

// #include <glm/glm.hpp>

// #include "Structs/Components.h"

// class Chunk
// {
// public:
//   Chunk(const glm::ivec3 &pos = {0.0f, 0.0f, 0.0f});
//   ~Chunk() {}

//   void Bind();
//   void Unbind();

//   const glm::ivec3 &GetPosition() const { return m_Position; }
//   const uint32_t GetIndiceCount() const { return m_IndiceCount; }
//   void GenerateMesh();
//   void RecalculateData();

// private:
//   void Init();
//   void GenerateChunk();

// private:
//   uint32_t m_VBO;
//   uint32_t m_VAO;
//   uint32_t m_IBO;

//   glm::ivec3 m_Position;

//   const int m_ChunkWidth = 8;
//   const int m_ChunkHeight = 8;
//   const int m_ChunkDepth = 8;

//   std::vector<Block> m_Blocks;
//   std::vector<float> m_Vertices;
//   std::vector<uint32_t> m_Indices;
//   uint32_t m_IndiceCount;
// };
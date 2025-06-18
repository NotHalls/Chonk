#include "Chunk.h"

#include <iostream>

#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

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
                     {1}},

    // Back Face
    BaseFaceTemplate{{glm::vec3(1.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f),
                      glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f)},
                     {glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 0.0f),
                      glm::vec2(1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
                     {1}},
    // Left Face
    BaseFaceTemplate{{glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f),
                      glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 1.0f)},
                     {glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 0.0f),
                      glm::vec2(1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
                     {1}},
    // Right Face
    BaseFaceTemplate{{glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 1.0f),
                      glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 0.0f)},
                     {glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 0.0f),
                      glm::vec2(1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
                     {1}},
    // Top Face
    BaseFaceTemplate{{glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f),
                      glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 1.0f)},
                     {glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 0.0f),
                      glm::vec2(1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
                     {0}},
    // Bottom Face
    BaseFaceTemplate{{glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f),
                      glm::vec3(1.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f)},
                     {glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 0.0f),
                      glm::vec2(1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
                     {2}}};
constexpr const glm::vec3 GetBlockTextureFromID(BlockID id)
{
  // clang-format off
  switch(id)
  {
    case BlockID::None:   return {0, 0, 0};
    case BlockID::Grass:  return {1, 2, 3};
    case BlockID::Moss:   return {1, 1, 1};
    case BlockID::Dirt:   return {3, 3, 3};
    case BlockID::Stone:  return {4, 4, 4};
    default: return {0, 0, 0};
  };
  // clang-format on
}
// this piece of code gives us the uvs for each vertice.
constexpr const glm::vec2 CalculateTextureUVs(int texID, const glm::vec2 &uvs)
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

Chunk::Chunk()
{
  Init();
  RegenerateChunk();
}

void Chunk::Init()
{
  glCreateVertexArrays(1, &m_VAO);
  glCreateBuffers(1, &m_VBO);
  glCreateBuffers(1, &m_IBO);
}

void Chunk::RegenerateChunk()
{
  for(int y = 0; y < CHUNK_SIZE_Y; y++)
  {
    for(int x = 0; x < CHUNK_SIZE_X; x++)
    {
      for(int z = 0; z < CHUNK_SIZE_Z; z++)
      {
        Block &block = m_Blocks[GetBlockIndex(x, y, z)];
        block.ID = BlockID::Grass;

        for(int faceIndex = 0; faceIndex < 6; faceIndex++)
        {
          addVertices(x, y, z, faceIndex, block.ID);
        }
      }
    }
  }
  GenerateMesh();
}

void Chunk::addVertices(int x, int y, int z, int faceIndex, BlockID id)
{
  glm::vec3 worldPos = m_Position + glm::vec3((float)x, (float)y, (float)z);
  // adding vertices
  for(int v = 0; v < 4; v++)
  {
    glm::vec3 pos = FaceTemplates[faceIndex].Positions[v] + worldPos;
    glm::vec2 verticeUV = CalculateTextureUVs(
        GetBlockTextureFromID(id)[FaceTemplates[faceIndex].FaceID],
        FaceTemplates[faceIndex].UVs[v]);
    m_Vertices.push_back(pos.x);
    m_Vertices.push_back(pos.y);
    m_Vertices.push_back(pos.z);
    m_Vertices.push_back(verticeUV.x);
    m_Vertices.push_back(verticeUV.y);
  }

  // adding indices
  m_Indices.push_back(m_CurrentVerticeCount + 3);
  m_Indices.push_back(m_CurrentVerticeCount + 2);
  m_Indices.push_back(m_CurrentVerticeCount + 1);
  m_Indices.push_back(m_CurrentVerticeCount + 1);
  m_Indices.push_back(m_CurrentVerticeCount + 0);
  m_Indices.push_back(m_CurrentVerticeCount + 3);
  m_CurrentVerticeCount += 4;
}

void Chunk::GenerateMesh()
{
  glBindVertexArray(m_VAO);

  glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(float),
               m_Vertices.data(), GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(uint32_t),
               m_Indices.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glBindVertexArray(0);
}

void Chunk::Draw()
{
#ifdef CHK_DEBUG
  // std::cout << "VAO: " << m_VAO << ", VBO: " << m_VBO << ", IBO: " <<
  // m_IBO
  //           << "\n";
  // std::cout << "Indices size: " << m_Indices.size() << "\n";
#endif

  glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, nullptr);
}

void Chunk::Bind() { glBindVertexArray(m_VAO); }
void Chunk::Unbind() { glBindVertexArray(0); }

const int Chunk::GetBlockIndex(int x, int y, int z) const
{
  return x + CHUNK_SIZE_Z * (z + CHUNK_SIZE_X * y);
}
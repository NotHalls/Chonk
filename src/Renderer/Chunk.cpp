#include <glad/glad.h>

#include "Chunk.h"
#include "include/tempData.h"

Chunk::Chunk(const glm::ivec3 &pos)
    : m_Position(pos), m_Blocks(m_ChunkWidth * m_ChunkHeight * m_ChunkDepth,
                                Block{BlockID::None, 0, 0, 0})
{
  Init();
  RecalculateData();
  GenerateMesh();
}

void Chunk::Init()
{
  glCreateVertexArrays(1, &m_VAO);
  glCreateBuffers(1, &m_VBO);
  glCreateBuffers(1, &m_IBO);
}

void Chunk::RecalculateData()
{
  // this generates the terrain of the chunk
  GenerateChunk();

  // this fills out the vertices and indices
  for(auto &block : m_Blocks)
  {
    m_Vertices.insert(m_Vertices.end(), CubeVertices.begin(),
                      CubeVertices.end());
    for(int i = 0; i < CubeIndices.size(); i++)
    {
      m_Indices.push_back(CubeIndices[i] + m_IndiceCount);
    }
    m_IndiceCount += 24;
  }
}

void Chunk::GenerateChunk()
{
  for(int x = 0; x < m_ChunkWidth; x++)
  {
    for(int z = 0; z < m_ChunkDepth; z++)
    {
      for(int y = 0; y < m_ChunkHeight; y++)
      {
        int index = x + m_ChunkWidth * (z + m_ChunkDepth * y);
        Block &block = m_Blocks[index];

        if(y == 0)
        {
          block.ID = BlockID::Moss;
          block.TopTextureID = 1;
          block.SideTextureID = 1;
          block.BottomTextureID = 1;
        }
        else if(y > 0 && y <= 31)
        {
          block.ID = BlockID::Dirt;
          block.TopTextureID = 3;
          block.SideTextureID = 3;
          block.BottomTextureID = 3;
        }
        else
        {
          block.ID = BlockID::Grass;
          block.TopTextureID = 1;
          block.SideTextureID = 2;
          block.BottomTextureID = 3;
        }
      }
    }
  }
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

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (void *)(5 * sizeof(float)));
  glEnableVertexAttribArray(2);

  glBindVertexArray(0);
}

void Chunk::Bind() { glBindVertexArray(m_VAO); }
void Chunk::Unbind() { glBindVertexArray(0); }
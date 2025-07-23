#include "Chunk.h"
#include "Debug/GLError.h"
#include "Processes/WorldGenerator.h"
#include "Structs/Math.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <print>

// file variables
constexpr int VERTICES_PER_FACE = 4;
constexpr int INDICES_PER_FACE = 6;
constexpr int ATTRIBS_PER_VERTICE = 5; // 3 Pos; 2 UV

// class functions
Chunk::Chunk(const glm::ivec3 &pos)
    : Dirty(true), m_Position(pos), m_CurrentVerticeCount(0)
{
  m_Blocks.resize(Global::CHUNK_VOLUME);
  Init();
  GenerateChunkBlocks();
}
Chunk::~Chunk()
{
  CheckGLErrors(glDeleteBuffers(1, &m_VBO));
  CheckGLErrors(glDeleteBuffers(1, &m_IBO));
  CheckGLErrors(glDeleteVertexArrays(1, &m_VAO));
}

void Chunk::Init()
{
  CheckGLErrors(glCreateVertexArrays(1, &m_VAO));
  CheckGLErrors(glCreateBuffers(1, &m_VBO));
  CheckGLErrors(glCreateBuffers(1, &m_IBO));
}

void Chunk::GenerateChunkBlocks()
{
  for(int i = 0; i < int(Global::CHUNK_VOLUME); i++)
  {
    m_Blocks[i].ID = BlockID::Grass;
  }
}

void Chunk::GenerateChunkFaces()
{
  if(!Dirty)
    return;

  int maxFaces = (Global::CHUNK_VOLUME / 2) * 3;
  int maxVertices = maxFaces * VERTICES_PER_FACE;
  int maxIndices = maxFaces * INDICES_PER_FACE;

  m_Vertices.clear();
  m_Indices.clear();
  m_Vertices.reserve(maxVertices * ATTRIBS_PER_VERTICE);
  m_Indices.reserve(maxIndices);
  m_CurrentVerticeCount = 0;

  // pushing the block vertices into a buffer
  for(int i = 0; i < int(Global::CHUNK_VOLUME); i++)
  {
    Block &block = m_Blocks[i];
    if(block.ID == BlockID::Air)
      continue;

    glm::ivec3 localPos = GetBlockPosFromIndex(i);
    for(int faceIndex = 0; faceIndex < 6; faceIndex++)
    {
      // the faceIndex's index being:
      // 0 - Front
      // 1 - Back
      // 2 - Left
      // 3 - Right
      // 4 - Top
      // 5 - Bottom

      Math::IVec3 checkPos = {
          localPos.x + NextBlockFromFaceIndex[faceIndex].x,
          localPos.y + NextBlockFromFaceIndex[faceIndex].y,
          localPos.z + NextBlockFromFaceIndex[faceIndex].z,
      };
      if(IsBlockOuterChunk({checkPos.x, checkPos.y, checkPos.z}))
      {
        glm::ivec3 outerBlockWorldPos =
            glm::ivec3(checkPos.x, checkPos.y, checkPos.z) + m_Position;

        const Block &checkBlock = World::GetChunkBlockAtPos(outerBlockWorldPos);
        if(checkBlock.ID != BlockID::None && checkBlock.ID != BlockID::Air)
          continue;
      }
      else if(m_Blocks[GetBlockIndexFromPos(
                           glm::ivec3(checkPos.x, checkPos.y, checkPos.z))]
                  .ID != BlockID::Air)
        continue;
      AddVertices(localPos.x, localPos.y, localPos.z, faceIndex, block.ID);
    }
  }
  Dirty = false;
}

void Chunk::AddVertices(int x, int y, int z, int faceIndex, BlockID id)
{
  glm::vec3 worldPos = glm::vec3((float)x, (float)y, (float)z);
  // adding vertices
  for(int v = 0; v < 4; v++)
  {
    glm::vec3 pos = FaceTemplates[faceIndex].Positions[v] + worldPos;
    glm::vec2 verticeUV = CalculateBlockTextureUVs(
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
  // std::println("Vertice At X:{}, Y:{}, Z:{} Added", x, y, z);
}

void Chunk::GenerateMesh()
{
  CheckGLErrors(glBindVertexArray(m_VAO));

  CheckGLErrors(glBindBuffer(GL_ARRAY_BUFFER, m_VBO));
  CheckGLErrors(glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(float),
                             m_Vertices.data(), GL_STATIC_DRAW));
  CheckGLErrors(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO));
  CheckGLErrors(glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                             m_Indices.size() * sizeof(uint32_t),
                             m_Indices.data(), GL_STATIC_DRAW));

  CheckGLErrors(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                                      5 * sizeof(float), (void *)0));
  CheckGLErrors(glEnableVertexAttribArray(0));
  CheckGLErrors(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
                                      5 * sizeof(float),
                                      (void *)(3 * sizeof(float))));
  CheckGLErrors(glEnableVertexAttribArray(1));

  CheckGLErrors(glBindVertexArray(0));
}

void Chunk::Draw()
{
  // std::cout << "VAO: " << m_VAO << ", VBO: " << m_VBO << ", IBO: " << m_IBO
  //           << "\n";
  // std::cout << "Indices size: " << m_Indices.size() << "\n";

  CheckGLErrors(
      glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, nullptr));
}

void Chunk::Bind() { glBindVertexArray(m_VAO); }
void Chunk::Unbind() { glBindVertexArray(0); }

int Chunk::GetBlockIndexFromPos(const glm::ivec3 &pos) const
{
  // we push blocks into the chunk in this order: x -> z -> y
  // so the above formula is basicly
  // x + (CHUNK_SIZE_X * z) + (CHUNK_SIZE_X *
  // CHUNK_SIZE_Z * y)

  return pos.x + (pos.z * Global::CHUNK_SIZE_X) +
         (pos.y * Global::CHUNK_SIZE_X * Global::CHUNK_SIZE_Z);
}

glm::ivec3 Chunk::GetBlockPosFromIndex(int index) const
{
  int y = index / (Global::CHUNK_SIZE_Z * Global::CHUNK_SIZE_X);
  int remainder = index % (Global::CHUNK_SIZE_Z * Global::CHUNK_SIZE_X);
  /// @test try having different values for X and Z
  /// see if the 2 lines below work then (just curious).
  int z = remainder / Global::CHUNK_SIZE_X;
  int x = remainder % Global::CHUNK_SIZE_X;
  return {x, y, z};
}

const Block &Chunk::GetBlockAtPos(const glm::ivec3 &pos)
{
  int index = GetBlockIndexFromPos(pos);
  return m_Blocks[index];
}

// clang-format off
bool Chunk::IsBlockOuterChunk(const glm::ivec3 &pos) const
{
  return pos.x < 0 || pos.x >= int(Global::CHUNK_SIZE_X) ||
         pos.y < 0 || pos.y >= int(Global::CHUNK_SIZE_Y) ||
         pos.z < 0 || pos.z >= int(Global::CHUNK_SIZE_Z);
}
// clang-format on
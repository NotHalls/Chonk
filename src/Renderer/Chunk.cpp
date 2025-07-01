#include "Chunk.h"
#include "Debug/GLError.h"
#include "Processes/WorldGenerator.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

struct BaseFaceTemplate
{
  glm::vec3 Positions[4];
  glm::vec2 UVs[4];
  uint8_t FaceID;
};

// clang-format off
// this is a set of values that tell us what side to add or sub according to
// the index. (used for greedy meshing)
constexpr const glm::ivec3 NextBlockFromFaceIndex[6] = {
  { 0,  0, -1},  // Front
  { 0,  0,  1},  // Back
  {-1,  0,  0},  // Left
  { 1,  0,  0},  // Right
  { 0,  1,  0},  // Top
  { 0, -1,  0}   // Bottom
};
// clang-format on

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

Chunk::Chunk(const glm::ivec3 &pos) : m_Position(pos)
{
  m_Blocks.resize(CHUNK_VOLUME);
  GenerateChunkBlocks();
  Init();
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
  for(int i = 0; i < int(CHUNK_VOLUME); i++)
  {
    m_Blocks[i].ID = BlockID::Grass;
  }
}

void Chunk::GenerateChunkFaces()
{
  // pushing the block vertices into a buffer
  for(int i = 0; i < int(CHUNK_VOLUME); i++)
  {
    Block &block = m_Blocks[i];
    glm::ivec3 pos = GetBlockPosFromIndex(i);
    for(int faceIndex = 0; faceIndex < 6; faceIndex++)
    {
      // the faceIndex's index being:
      // 0 - Front
      // 1 - Back
      // 2 - Left
      // 3 - Right
      // 4 - Top
      // 5 - Bottom

      glm::ivec3 checkPos = pos + NextBlockFromFaceIndex[faceIndex];
      if(IsBlockOuterChunk(checkPos))
      {
        glm::ivec3 outerBlockWorldPos = checkPos + glm::ivec3(m_Position);

        glm::ivec3 outerChunkPos = {
            (outerBlockWorldPos.x / CHUNK_SIZE_X) * CHUNK_SIZE_X,
            (outerBlockWorldPos.y / CHUNK_SIZE_Y) * CHUNK_SIZE_Y,
            (outerBlockWorldPos.z / CHUNK_SIZE_Z) * CHUNK_SIZE_Z,
        };

        if(World::CheckChunkAtPos(outerChunkPos) &&
           World::GetChunkBlockAtPos(outerChunkPos, outerBlockWorldPos).ID !=
               BlockID::None)
          continue;
      }
      else if(m_Blocks[GetBlockIndexFromPos(checkPos)].ID != BlockID::None)
        continue;
      AddVertices(pos.x, pos.y, pos.z, faceIndex, block.ID);
    }
  }
  GenerateMesh();
}

void Chunk::AddVertices(int x, int y, int z, int faceIndex, BlockID id)
{
  glm::vec3 worldPos = glm::vec3((float)x, (float)y, (float)z);
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
  // #ifdef CHK_DEBUG
  //   std::cout << "VAO: " << m_VAO << ", VBO: " << m_VBO << ", IBO: " << m_IBO
  //             << "\n";
  //   std::cout << "Indices size: " << m_Indices.size() << "\n";
  // #endif

  CheckGLErrors(
      glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, nullptr));
}

void Chunk::Bind() { glBindVertexArray(m_VAO); }
void Chunk::Unbind() { glBindVertexArray(0); }

int Chunk::GetBlockIndexFromPos(const glm::ivec3 &pos) const
{
  // we push blocks into the chunk in this order: x -> z -> y
  // so the above formula is basicly
  // x + (CHUNK_SIZE_X * z) + (CHUNK_SIZE_X * CHUNK_SIZE_Z * y)

  return pos.x + (pos.z * CHUNK_SIZE_X) + (pos.y * CHUNK_SIZE_X * CHUNK_SIZE_Z);
}

glm::ivec3 Chunk::GetBlockPosFromIndex(int index) const
{
  int y = index / (CHUNK_SIZE_Z * CHUNK_SIZE_X);
  int remainder = index % (CHUNK_SIZE_Z * CHUNK_SIZE_X);
  /// @test try having different values for X and Z
  /// see if the 2 lines below work then (just curious).
  int z = remainder / CHUNK_SIZE_Z;
  int x = remainder % CHUNK_SIZE_Z;
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
  return pos.x < 0 || pos.x >= int(CHUNK_SIZE_X) ||
         pos.y < 0 || pos.y >= int(CHUNK_SIZE_Y) ||
         pos.z < 0 || pos.z >= int(CHUNK_SIZE_Z);
}
// clang-format on
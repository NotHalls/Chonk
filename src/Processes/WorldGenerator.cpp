#include "Debug/Assert.h"
#include "FixedGlobals.h"
#include "WorldGenerator.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#ifdef CHK_DEBUG
#include <imgui.h>
#endif

#include <format>
#include <iostream>
#include <print>
#include <string>

std::unordered_map<glm::ivec3, std::shared_ptr<Chunk>, Util::IVec3Hasher>
    World::m_Chunks;

void World::GenerateWorld()
{
  for(int z = 0; z < Global::CHUNK_SIZE_Z * 20; z += 16)
  {
    for(int x = 0; x < Global::CHUNK_SIZE_X * 20; x += 16)
    {
      std::shared_ptr<Chunk> chunk =
          std::make_shared<Chunk>(glm::vec3(float(x), 0.0f, float(z)));
      m_Chunks.insert({{x, 0, z}, chunk});
    }
  }

  // std::shared_ptr<Chunk> chunk =
  //     std::make_shared<Chunk>(glm::vec3(0.0f, 0.0f, 0.0f));
  // m_Chunks.insert({{0, 0, 0}, chunk});
  // std::shared_ptr<Chunk> chunk1 =
  //     std::make_shared<Chunk>(glm::vec3(16.0f, 0.0f, 0.0f));
  // m_Chunks.insert({{16, 0, 0}, chunk1});
  // std::shared_ptr<Chunk> chunk2 = std::make_shared<Chunk>();
  // chunk2->SetPosition(glm::vec3(32.0f, 0.0f, 0.0f));
  // m_Chunks.insert({{32, 0, 0}, chunk2});
  // std::shared_ptr<Chunk> chunk3 = std::make_shared<Chunk>();
  // chunk3->SetPosition(glm::vec3(0.0f, 0.0f, 16.0f));
  // m_Chunks.insert({{0, 0, 16}, chunk3});

  for(auto [pos, chunk] : m_Chunks)
  {
    chunk->GenerateChunkFaces();
  }
}

const std::shared_ptr<Chunk> &World::GetChunkAtPos(const glm::ivec3 &pos)
{
  auto itr = m_Chunks.find(pos);
  if(itr != m_Chunks.end())
    return itr->second;

  std::string msg =
      std::format("No Chunk At Pos: X:{}, Y:{}, Z:{}", pos.x, pos.y, pos.z);
  CHK_ASSERT(false, std::string(msg));
}

const Block &World::GetChunkBlockAtPos(const glm::ivec3 &chunkPos,
                                       const glm::ivec3 &blockPos)
{
  const std::shared_ptr<Chunk> &chunk = GetChunkAtPos(chunkPos);
  glm::ivec3 checkPos =
      blockPos % glm::ivec3(Global::CHUNK_SIZE_X, Global::CHUNK_SIZE_Y,
                            Global::CHUNK_SIZE_Z);
  return chunk->GetBlockAtPos(checkPos);
}

bool World::CheckChunkAtPos(const glm::ivec3 &pos)
{
  return m_Chunks.contains(pos);
}

void World::UpdateGUI()
{
#ifdef CHK_DEBUG
#endif
}

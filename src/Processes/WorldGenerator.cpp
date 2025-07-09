#include "Debug/Assert.h"
#include "FixedGlobals.h"
#include "Structs/Util.h"
#include "System/Scene.h"
#include "System/Settings.h"
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

// file functions
// clang-format off
static constexpr glm::ivec3 GetChunkNeighbour[4] = {
  // the index represents the side:
  // 0 - Front
  // 1 - Back
  // 2 - Left
  // 3 - Right
  
  { 0,  0,  Global::CHUNK_SIZE_Z},  // Front
  { 0,  0, -Global::CHUNK_SIZE_Z},  // Back
  {-Global::CHUNK_SIZE_X,  0,  0},  // Left
  { Global::CHUNK_SIZE_X,  0,  0},  // Right
};
// clang-format on

// class variables
std::unordered_map<glm::ivec3, std::shared_ptr<Chunk>, Util::IVec3Hasher>
    World::m_Chunks;

void World::GenerateWorld()
{
  const int renderDistance =
      Settings::GetVideoSettings(VideoSettingsOptions::RenderDistance);
  const glm::ivec3 playerChunkPos = Scene::GetCamera()->GetChunkPosition();

  for(int z = -renderDistance; z <= renderDistance; z++)
  {
    for(int x = -renderDistance; x <= renderDistance; x++)
    {
      LoadChunk(glm::ivec3(playerChunkPos.x + (x * Global::CHUNK_SIZE_X), 0,
                           playerChunkPos.z + (z * Global::CHUNK_SIZE_Z)));
    }
  }

  for(auto &[pos, chunk] : m_Chunks)
  {
    chunk->GenerateChunkFaces();
  }
}

void World::LoadChunk(const glm::ivec3 &pos)
{
  if(CheckChunkAtPos(pos))
    return;
  std::shared_ptr<Chunk> chunk = std::make_shared<Chunk>(
      glm::vec3(float(pos.x), float(pos.y), float(pos.z)));
  m_Chunks.insert({{pos.x, pos.y, pos.z}, chunk});

  UpdateChunkNeighbours(pos);
}

void World::UnloadChunk(const glm::ivec3 &pos)
{
  CHK_ASSERT(CheckChunkAtPos(pos),
             "No Chunk Exists At Position: " +
                 std::format("X: {}, Y: {}, Z: {}", pos.x, pos.y, pos.z));

  m_Chunks.erase(pos);

  UpdateChunkNeighbours(pos);
}

void World::ReloadChunk(const glm::ivec3 &pos)
{
  UnloadChunk(pos);
  LoadChunk(pos);
  m_Chunks.at(pos)->GenerateChunkFaces();
  UpdateChunkNeighbours(pos);
}

void World::UnloadUnseenChunks()
{
  int renderDistance =
      Settings::GetVideoSettings(VideoSettingsOptions::RenderDistance);
  glm::ivec3 playerChunkPos = Scene::GetCamera()->GetChunkPosition();

  std::vector<glm::ivec3> chunksToUnload;

  for(const auto &[pos, chunk] : m_Chunks)
  {
    glm::ivec3 checkPos = glm::abs(pos - playerChunkPos);

    if(checkPos.x > renderDistance * Global::CHUNK_SIZE_X ||
       checkPos.z > renderDistance * Global::CHUNK_SIZE_Z)
    {
      chunksToUnload.push_back(pos);
    }
  }

  for(const auto &pos : chunksToUnload)
  {
    UnloadChunk(pos);
  }
}

void World::UpdateChunkNeighbours(const glm::ivec3 &pos)
{
  for(int i = 0; i < 4; i++)
  {
    glm::ivec3 neighborPos = pos + GetChunkNeighbour[i];
    if(m_Chunks.contains(neighborPos))
    {
      m_Chunks.at(neighborPos)->Dirty = true;
    }
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
  glm::ivec3 checkPos = Util::NegetiveModule(
      blockPos, glm::ivec3(Global::CHUNK_SIZE_X, Global::CHUNK_SIZE_Y,
                           Global::CHUNK_SIZE_Z));
  return chunk->GetBlockAtPos(checkPos);
}

bool World::CheckChunkAtPos(const glm::ivec3 &pos)
{
  return m_Chunks.contains(pos);
}

void World::ClearChunks() { m_Chunks.clear(); }

void World::UpdateGUI()
{
#ifdef CHK_DEBUG
#endif
}

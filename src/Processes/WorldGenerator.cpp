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

static void GenerateChunkFaceOf(const std::shared_ptr<Chunk> &chunk)
{
  chunk->GenerateChunkFaces();
}

// class variables
std::unordered_map<glm::ivec3, std::shared_ptr<Chunk>, Util::IVec3Hasher>
    World::m_Chunks;
std::mutex World::m_ChunksMutex;
std::unordered_map<glm::ivec3, std::future<void>, Util::IVec3Hasher>
    World::m_ChunkGenFutures;

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
    if(chunk->Dirty && !m_ChunkGenFutures.contains(pos))
      m_ChunkGenFutures[pos] =
          std::async(std::launch::async, GenerateChunkFaceOf, chunk);
  }
}

void World::UpdateWorld()
{
  for(auto itr = m_ChunkGenFutures.begin(); itr != m_ChunkGenFutures.end();)
  {
    if(itr->second.wait_for(std::chrono::seconds(0)) ==
       std::future_status::ready)
    {
      try
      {
        itr->second.get();
      }
      catch(const std::exception &e)
      {
        std::cout << "Error While Generating Chunk: " << e.what() << std::endl;
      }
      itr = m_ChunkGenFutures.erase(itr);
    }
    else
      itr++;
  }
}

void World::GenerateChunkMeshes()
{
  for(auto &[pos, chunk] : m_Chunks)
  {
    if(!chunk->Dirty || World::GetChunkGenFutures().contains(pos))
      chunk->GenerateMesh();
  }
}

void World::LoadChunk(const glm::ivec3 &pos)
{
  std::lock_guard<std::mutex> lock(m_ChunksMutex);
  if(CheckChunkAtPos(pos))
    return;
  std::shared_ptr<Chunk> chunk = std::make_shared<Chunk>(
      glm::vec3(float(pos.x), float(pos.y), float(pos.z)));

  if(chunk->Dirty || m_ChunkGenFutures.contains(pos))
  {
    m_ChunkGenFutures.erase(pos);
  }

  m_Chunks.insert({{pos.x, pos.y, pos.z}, chunk});
  UpdateChunkNeighbours(pos);
}

void World::UnloadChunk(const glm::ivec3 &pos)
{
  std::lock_guard<std::mutex> lock(m_ChunksMutex);
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

  std::lock_guard<std::mutex> lock(m_ChunksMutex);
  m_Chunks.at(pos)->GenerateChunkFaces();
  UpdateChunkNeighbours(pos);
}

void World::UnloadUnseenChunks()
{
  std::vector<glm::ivec3> chunksToUnload;
  {
    std::lock_guard<std::mutex> lock(m_ChunksMutex);
    int renderDistance =
        Settings::GetVideoSettings(VideoSettingsOptions::RenderDistance);
    glm::ivec3 playerChunkPos = Scene::GetCamera()->GetChunkPosition();

    for(const auto &[pos, chunk] : m_Chunks)
    {
      glm::ivec3 checkPos = glm::abs(pos - playerChunkPos);

      if(checkPos.x > renderDistance * Global::CHUNK_SIZE_X ||
         checkPos.z > renderDistance * Global::CHUNK_SIZE_Z)
      {
        if(chunk->Dirty || m_ChunkGenFutures.contains(pos))
          continue;
        chunksToUnload.push_back(pos);
      }
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

const Block &World::GetChunkBlockAtPos(const glm::ivec3 &blockPos)
{
  std::lock_guard<std::mutex> lock(m_ChunksMutex);
  glm::ivec3 chunkPos = {
      static_cast<int>(
          std::floor(static_cast<float>(blockPos.x) / Global::CHUNK_SIZE_X) *
          Global::CHUNK_SIZE_X),
      static_cast<int>(
          std::floor(static_cast<float>(blockPos.y) / Global::CHUNK_SIZE_Y) *
          Global::CHUNK_SIZE_Y),
      static_cast<int>(
          std::floor(static_cast<float>(blockPos.z) / Global::CHUNK_SIZE_Z) *
          Global::CHUNK_SIZE_Z),
  };

  if(!CheckChunkAtPos(chunkPos))
  {
    static Block noneBlock{};
    return noneBlock;
  }

  const std::shared_ptr<Chunk> &chunk = GetChunkAtPos(chunkPos);
  glm::ivec3 checkPos = Util::NegetiveModule(
      blockPos, glm::ivec3(Global::CHUNK_SIZE_X, Global::CHUNK_SIZE_Y,
                           Global::CHUNK_SIZE_Z));
  return chunk->GetBlockAtPos(checkPos);
}

inline bool World::CheckChunkAtPos(const glm::ivec3 &pos)
{
  return m_Chunks.contains(pos);
}

void World::ClearChunks() { m_Chunks.clear(); }

void World::UpdateGUI()
{
#ifdef CHK_DEBUG
#endif
}

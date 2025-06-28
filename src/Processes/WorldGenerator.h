#pragma once

#include "Renderer/Chunk.h"
#include "System/Process.h"

#include <memory>
#include <vector>

class World
{
public:
  static void GenerateWorld();

  static const std::vector<std::shared_ptr<Chunk>> &GetChunks()
  {
    return m_Chunks;
  }

private:
  static std::vector<std::shared_ptr<Chunk>> m_Chunks;
};
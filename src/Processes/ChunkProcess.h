#pragma once
#include "Renderer/Chunk.h"
#include "System/Process.h"

class ChunkProcess : public Process
{
public:
  virtual void OnStart() override;
  virtual void OnUpdate() override;

private:
  Chunk m_Chunk;
};
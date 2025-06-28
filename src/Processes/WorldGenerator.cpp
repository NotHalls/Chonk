#include "WorldGenerator.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

std::vector<std::shared_ptr<Chunk>> World::m_Chunks;

void World::GenerateWorld()
{
  for(int i = 0; i < 16 * 10; i += 16)
  {
    for(int j = 0; j < 16 * 10; j += 16)
    {
      std::shared_ptr<Chunk> chunk = std::make_shared<Chunk>();
      chunk->SetPosition(glm::vec3(float(i), 0.0f, float(j)));
      m_Chunks.push_back(chunk);
    }
  }
}
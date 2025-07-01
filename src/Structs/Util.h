#pragma once

#include <glm/vec3.hpp>

#include <functional>

namespace Util
{
struct IVec3Hasher
{
  size_t operator()(const glm::ivec3 &v) const
  {
    size_t h1 = std::hash<int>{}(v.x);
    size_t h2 = std::hash<int>{}(v.y);
    size_t h3 = std::hash<int>{}(v.z);
    return h1 ^ (h2 << 1) ^ (h3 << 2);
  }
};
}; // namespace Util
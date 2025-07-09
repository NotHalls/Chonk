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

// clang-format off
inline int NegetiveModule(int a, int b) { return (a % b + b) % b; }
inline glm::ivec3 NegetiveModule(const glm::ivec3 &a, const glm::ivec3 &b)
{
    return glm::ivec3(
      (a.x % b.x + b.x) % b.x,
      (a.y % b.y + b.y) % b.y,
      (a.z % b.z + b.z) % b.z);
}
// clang-format on

}; // namespace Util
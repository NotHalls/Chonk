#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Structs/Block.h"

struct Transform
{
  glm::vec3 Position;
  glm::vec3 Rotation;
  glm::vec3 Scale;

  const glm::mat4 Get() const
  {
    glm::mat4 rot =
        glm::rotate(glm::mat4(1.0f), Rotation.x, glm::vec3(1.0f, 0.0f, 0.0f)) *
        glm::rotate(glm::mat4(1.0f), Rotation.y, glm::vec3(0.0f, 1.0f, 0.0f)) *
        glm::rotate(glm::mat4(1.0f), Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
    return glm::translate(glm::mat4(1.0f), Position) * rot *
           glm::scale(glm::mat4(1.0f), Scale);
  }
};
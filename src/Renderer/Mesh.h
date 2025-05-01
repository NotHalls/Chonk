#pragma once

#include <cstdint>
#include <vector>

#include <glm/glm.hpp>

class Mesh
{
private:
  struct TextureID
  {
    uint8_t Top;
    uint8_t Side;
    uint8_t Bottom;
  };

public:
  Mesh(const std::vector<float> &vertices, const std::vector<uint32_t> &indices,
       const glm::vec3 &pos = {0.0f, 0.0f, 0.0f});

  void Bind();
  void Unbind();

  glm::vec3 &GetPosition() { return m_Position; }
  TextureID &GetTextureID() { return m_TextureID; }

private:
  void Init();

private:
  glm::vec3 m_Position;
  TextureID m_TextureID;

  uint32_t m_VAO;
  uint32_t m_VBO;
  uint32_t m_IBO;

  std::vector<float> m_Vertices;
  std::vector<uint32_t> m_Indices;
};
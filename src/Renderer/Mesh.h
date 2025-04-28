#pragma once

#include <iostream>
#include <vector>

#include "Structs/Block.h"
#include "Structs/Components.h"

class Mesh
{
public:
  Mesh();
  Mesh(std::vector<float> vertices, std::vector<uint32_t> indices);
  ~Mesh() {}

  void Bind();
  void Unbind();

  Transform &GetTransform() { return m_Transform; }
  BlockInfo &GetBlockInfo() { return m_BlockInfo; }

private:
  void ProcessMesh();

private:
  uint32_t m_VBO;
  uint32_t m_VAO;
  uint32_t m_IBO;

  BlockInfo m_BlockInfo;
  Transform m_Transform;

  std::vector<float> m_Vertices;
  std::vector<uint32_t> m_Indices;
};
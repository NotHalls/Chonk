#include <glad/glad.h>

#include "Mesh.h"

Mesh::Mesh()
    : m_BlockInfo({0, 0, 0}),
      m_Transform({{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}})
{
  ProcessMesh();
}
Mesh::Mesh(std::vector<float> vertices, std::vector<uint32_t> indices)
    : m_Vertices(vertices), m_Indices(indices), m_BlockInfo({0, 0, 0}),
      m_Transform({{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}})
{
  ProcessMesh();
}

void Mesh::ProcessMesh()
{
  glCreateVertexArrays(1, &m_VAO);
  glCreateBuffers(1, &m_VBO);
  glCreateBuffers(1, &m_IBO);

  glBindVertexArray(m_VAO);

  glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(float),
               m_Vertices.data(), GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(uint32_t),
               m_Indices.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (void *)(5 * sizeof(float)));
  glEnableVertexAttribArray(2);

  glBindVertexArray(0);
}

void Mesh::Bind() { glBindVertexArray(m_VAO); }
void Mesh::Unbind() { glBindVertexArray(0); }
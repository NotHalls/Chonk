#include "Scene.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>

std::unique_ptr<Camera> Scene::m_Camera = nullptr;
std::unique_ptr<Shader> Scene::m_Shader = nullptr;
std::vector<Chunk> Scene::m_Chunks;

void Scene::Init(float fov, float nPlane, float fPlane, int width, int height)
{
  m_Camera = std::make_unique<Camera>(fov, nPlane, fPlane, width, height);
  std::unordered_map<std::string, ShaderType> shaders = {
      {"assets/shaders/Default.vert.glsl", ShaderType::Vertex},
      {"assets/shaders/Default.frag.glsl", ShaderType::Fragment}};
  m_Shader = std::make_unique<Shader>(shaders);
}

void Scene::StartScene()
{
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CCW);
  glClearColor(0.1f, 0.1f, 0.1f, 0.1f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void Scene::StopScene()
{
  for(auto chunk : m_Chunks)
  {
    glm::mat4 model = glm::translate(glm::mat4(1.0f), chunk.GetPosition());
    glm::mat4 mvp = m_Camera->GetVPMatrix() * model;
    int uMVPLoc = glGetUniformLocation(m_Shader->Get(), "u_MVP");
    glUniformMatrix4fv(uMVPLoc, 1, GL_FALSE, glm::value_ptr(mvp));
    chunk.Bind();
    chunk.Draw();
  }
}

void Scene::PushChunk(Chunk chunk) { m_Chunks.push_back(chunk); }
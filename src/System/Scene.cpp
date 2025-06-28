#include "Debug/Assert.h"
#include "Scene.h"
#include "System/App.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>

#include <print>

std::unique_ptr<Camera> Scene::m_Camera = nullptr;
std::unique_ptr<Shader> Scene::m_Shader = nullptr;
std::unique_ptr<Texture> Scene::m_TextureAtlas = nullptr;
std::vector<std::shared_ptr<Chunk>> Scene::m_Chunks;

void Scene::Init(float fov, float nPlane, float fPlane, int width, int height)
{
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CCW);

  m_Camera = std::make_unique<Camera>(fov, nPlane, fPlane, width, height);
  std::unordered_map<std::string, ShaderType> shaders = {
      {"assets/shaders/Default.vert.glsl", ShaderType::Vertex},
      {"assets/shaders/Default.frag.glsl", ShaderType::Fragment}};
  m_Shader = std::make_unique<Shader>(shaders);
  m_TextureAtlas =
      std::make_unique<Texture>("assets/textures/TextureAtlas.png");

  /// @temp: pushing chunks into the vector is what the random generator
  /// should do later on
  for(int i = 0; i < 16 * 10; i += 16)
  {
    for(int j = 0; j < 16 * 10; j += 16)
    {
      std::shared_ptr<Chunk> chunk = std::make_shared<Chunk>();
      chunk->SetPosition(glm::vec3(float(i), 0.0f, float(j)));
      Scene::PushChunk(chunk);
    }
  }
}

void Scene::StartScene()
{
  glClearColor(0.1f, 0.1f, 0.1f, 0.1f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  m_Shader->Bind();
  m_TextureAtlas->Bind();
  m_Shader->SetUniformInt("u_Texture0", 0);
}
void Scene::StopScene()
{
  // CHK_ASSERT(!m_Chunks.empty(), "No Chunks To Render!");
  for(auto &chunk : m_Chunks)
  {
    glm::mat4 model = glm::translate(glm::mat4(1.0f), chunk->GetPosition());
    glm::mat4 mvp = m_Camera->GetVPMatrix() * model;
    m_Shader->SetUniformMat4("u_MVP", mvp);
    chunk->Bind();
    chunk->Draw();
  }
}

void Scene::PushChunk(const std::shared_ptr<Chunk> &chunk)
{
  m_Chunks.push_back(chunk);
}
#include "Debug/Assert.h"
#include "GUI/EngineGUI.h"
#include "Processes/WorldGenerator.h"
#include "Scene.h"
#include "System/App.h"
#include "System/Settings.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

#include <glad/glad.h>

#include <print>

// class variables
std::unique_ptr<Camera> Scene::m_Camera = nullptr;
std::unique_ptr<Shader> Scene::m_Shader = nullptr;
std::unique_ptr<Texture> Scene::m_TextureAtlas = nullptr;

// file variables
static glm::ivec2 lastCamChunkPos = {0, 0};

// file functions
static bool ChunkGenerationCheck()
{
  glm::ivec2 camChunkPos = {Scene::GetCamera()->GetChunkPosition().x,
                            Scene::GetCamera()->GetChunkPosition().z};

  if(lastCamChunkPos != camChunkPos)
  {
    lastCamChunkPos = camChunkPos;
    return true;
  }
  else
    return false;
}

// class functions
void Scene::Init(float fov, float nPlane, float fPlane, int width, int height)
{
  m_Camera = std::make_unique<Camera>(fov, nPlane, fPlane, width, height);
  std::unordered_map<std::string, ShaderType> shaders = {
      {"assets/shaders/Default.vert.glsl", ShaderType::Vertex},
      {"assets/shaders/Default.frag.glsl", ShaderType::Fragment}};
  m_Shader = std::make_unique<Shader>(shaders);
  m_TextureAtlas =
      std::make_unique<Texture>("assets/textures/TextureAtlas.png");
  World::GenerateWorld();
}

void Scene::StartScene()
{
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  m_Shader->Bind();
  m_TextureAtlas->Bind();
  m_Shader->SetUniformInt("u_Texture0", 0);

  // DisplayGUI();
  World::UpdateGUI();
}
void Scene::StopScene()
{
  if(!Settings::GetGameSettings(GameSettingsOptions::Spectating))
  {
    if(ChunkGenerationCheck())
    {
      World::UnloadUnseenChunks();
      World::GenerateWorld();
    }
  }

  for(auto &[pos, chunk] : World::GetChunks())
  {
    glm::mat4 model =
        glm::translate(glm::mat4(1.0f), glm::vec3(chunk->GetPosition()));
    glm::mat4 mvp = m_Camera->GetVPMatrix() * model;
    m_Shader->SetUniformMat4("u_MVP", mvp);
    chunk->Bind();
    chunk->Draw();
  }
}
#include "Scene.h"

std::unique_ptr<Camera> Scene::m_Camera = nullptr;
std::unique_ptr<Shader> Scene::m_Shader = nullptr;

void Scene::Init(float fov, float nPlane, float fPlane, int width, int height)
{
  m_Camera = std::make_unique<Camera>(fov, nPlane, fPlane, width, height);
  std::unordered_map<std::string, ShaderType> shaders = {
      {"assets/shaders/Default.vert.glsl", ShaderType::Vertex},
      {"assets/shaders/Default.frag.glsl", ShaderType::Fragment}};
  m_Shader = std::make_unique<Shader>(shaders);
}

void Scene::PushScene() {}
#pragma once

#include "Renderer/Camera.h"
#include "Renderer/Chunk.h"
#include "Renderer/Shader.h"
#include "Renderer/Texture.h"

#include <memory>
#include <vector>

class Scene
{
public:
  static void Init(float fov, float nPlane, float fPlane, int width,
                   int height);
  static const std::unique_ptr<Camera> &GetCamera() { return m_Camera; }
  static const std::unique_ptr<Shader> &GetShader() { return m_Shader; }

  static void StartScene();
  static void StopScene();

private:
  static std::unique_ptr<Camera> m_Camera;
  static std::unique_ptr<Shader> m_Shader;

  static std::unique_ptr<Texture> m_TextureAtlas;
};
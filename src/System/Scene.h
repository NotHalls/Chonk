#pragma once

#include "Renderer/Camera.h"
#include "Renderer/Shader.h"

#include <memory>

class Scene
{
public:
  static void Init(float fov, float nPlane, float fPlane, int width,
                   int height);
  static const std::unique_ptr<Camera> &GetCamera() { return m_Camera; }
  static const std::unique_ptr<Shader> &GetShader() { return m_Shader; }

  static void PushScene();

private:
  static std::unique_ptr<Camera> m_Camera;
  static std::unique_ptr<Shader> m_Shader;
};
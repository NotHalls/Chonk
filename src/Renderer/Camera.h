#pragma once

#include <SDL3/SDL_events.h>
#include <glm/glm.hpp>

class Camera
{
public:
  Camera(float fov, float nearP, float farP, uint32_t width, uint32_t height);
  ~Camera() {}

  const glm::mat4 &GetVPMatrix() const { return m_ProjectionView; }
  const glm::vec3 &GetTransform() const { return m_Position; }
  const float GetFOV() const { return m_FOV; }
  const float GetNearPlane() const { return m_NearPlane; }
  const float GetFatPlane() const { return m_FarPlane; }

  const glm::vec3 &GetForward() const { return m_Forward; }
  const glm::vec3 &GetUp() const { return m_Up; }

  void SetPosition(const glm::vec3 &pos);
  void SetForward(const glm::vec3 &forward);
  void SetFOV(float fov);
  void SetNearPlane(float nearP);
  void SetFarPlane(float farP);

  // Camera Changes
  void OnResize(uint32_t width, uint32_t height);
  void OnUpdate(float dt);
  void OnEvent(const SDL_Event &event);

private:
  void RecalculateMatrix();

public:
#ifdef CHK_DEBUG
  bool LockInput = false;
#endif

private:
  glm::mat4 m_Projection;
  glm::mat4 m_View;
  glm::mat4 m_ProjectionView;

  uint32_t m_Width, m_Height;

  glm::vec3 m_Position;
  glm::vec3 m_Up;
  glm::vec3 m_Forward;

  float m_FOV;
  float m_AspectRatio;
  float m_NearPlane, m_FarPlane;

  // Camera Controller
  float m_Speed = 5.0f;
  float m_Sensitivity = 0.3f;
  float m_Yaw = 90.0f, m_Pitch = 0.0f;
  bool m_FirstMouse = true;
};
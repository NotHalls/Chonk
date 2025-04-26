#pragma once

#include <SDL3/SDL_events.h>
#include <glm/glm.hpp>

class Camera
{
public:
  Camera(float fov, float nearP, float farP);
  ~Camera() {}

  const glm::mat4 &GetVPMatrix() const { return m_ProjectionView; }
  const glm::vec3 &GetTransform() const { return m_Position; }
  const float GetFOV() const { return m_FOV; }
  const float GetNearPlane() const { return m_NearPlane; }
  const float GetFatPlane() const { return m_FarPlane; }

  void SetPosition(const glm::vec3 &pos);
  void SetForward(const glm::vec3 &forward);
  void SetFOV(float fov);
  void SetNearPlane(float nearP);
  void SetFarPlane(float farP);

  // Camera Changes
  void OnResize(int width, int height);
  void OnUpdate(float dt);
  void OnEvent(SDL_Event &event);

private:
  void RecalculateMatrix();

private:
  glm::mat4 m_Projection;
  glm::mat4 m_View;
  glm::mat4 m_ProjectionView;

  glm::vec3 m_Position;
  glm::vec3 m_Up;
  glm::vec3 m_Forward;

  float m_FOV;
  float m_AspectRatio;
  float m_NearPlane, m_FarPlane;
};
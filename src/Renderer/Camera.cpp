#include <glm/gtc/matrix_transform.hpp>

#include "Camera.h"

Camera::Camera(float fov, float nearP, float farP)
    : m_Forward(0.0f, 0.0f, 1.0f), m_Up(0.0f, 1.0f, 0.0f),
      m_Position(0.0f, 0.0f, -5.0f), m_NearPlane(nearP), m_FarPlane(farP),
      m_FOV(fov)
{
  OnResize(800, 600);
}

void Camera::RecalculateMatrix()
{
  m_View = glm::lookAt(m_Position, m_Position + m_Forward, m_Up);
  m_Projection =
      glm::perspective(m_FOV, m_AspectRatio, m_NearPlane, m_FarPlane);
  m_ProjectionView = m_Projection * m_View;
}

// Camera Changes
void Camera::OnResize(int width, int height)
{
  m_AspectRatio = (float)width / (float)height;
  RecalculateMatrix();
}
void Camera::OnUpdate(float dt) {}
void Camera::OnEvent(SDL_Event &event) {}

// SetGet ers
void Camera::SetPosition(const glm::vec3 &pos)
{
  m_Position = pos;
  RecalculateMatrix();
}
void Camera::SetForward(const glm::vec3 &forward)
{
  m_Forward = forward;
  RecalculateMatrix();
}
void Camera::SetFOV(float fov)
{
  m_FOV = fov;
  RecalculateMatrix();
}
void Camera::SetNearPlane(float nearP)
{
  m_NearPlane = nearP;
  RecalculateMatrix();
}
void Camera::SetFarPlane(float farP)
{
  m_FarPlane = farP;
  RecalculateMatrix();
}
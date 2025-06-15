#include <glm/gtc/matrix_transform.hpp>

#include "Camera.h"

Camera::Camera(float fov, float nearP, float farP, uint32_t width,
               uint32_t height)
    : m_Forward(0.0f, 0.0f, 1.0f), m_Up(0.0f, 1.0f, 0.0f),
      m_Position(0.0f, 0.0f, -5.0f), m_NearPlane(nearP), m_FarPlane(farP),
      m_FOV(fov), m_Width(width), m_Height(height)
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
void Camera::OnResize(uint32_t width, uint32_t height)
{
  m_Width = width;
  m_Height = height;
  m_AspectRatio = (float)m_Width / (float)m_Height;
  RecalculateMatrix();
}
void Camera::OnUpdate(float dt)
{
  const bool *keyboard = SDL_GetKeyboardState(NULL);

  // Camera Movement
  if(keyboard[SDL_SCANCODE_W])
    SetPosition((m_Position += m_Forward * m_Speed * dt));
  if(keyboard[SDL_SCANCODE_S])
    SetPosition((m_Position -= m_Forward * m_Speed * dt));
  if(keyboard[SDL_SCANCODE_A])
    SetPosition((m_Position -=
                 (glm::normalize(glm::cross(m_Forward, m_Up)) * m_Speed * dt)));
  if(keyboard[SDL_SCANCODE_D])
    SetPosition((m_Position +=
                 (glm::normalize(glm::cross(m_Forward, m_Up)) * m_Speed * dt)));
  if(keyboard[SDL_SCANCODE_E])
    SetPosition((m_Position += m_Up * m_Speed * dt));
  if(keyboard[SDL_SCANCODE_Q])
    SetPosition((m_Position -= m_Up * m_Speed * dt));
}
void Camera::OnEvent(SDL_Event &event)
{
  if(event.type == SDL_EVENT_MOUSE_MOTION)
  {
    if(m_FirstMouse)
    {
      m_FirstMouse = false;
      return;
    }
    m_Yaw += event.motion.xrel * m_Sensitivity;
    m_Pitch -= event.motion.yrel * m_Sensitivity;

    m_Pitch = glm::clamp(m_Pitch, -89.9f, 89.9f);

    glm::vec3 rotDir = m_Forward;
    rotDir.x = glm::cos(glm::radians(m_Yaw)) * glm::cos(glm::radians(m_Pitch));
    rotDir.y = glm::sin(glm::radians(m_Pitch));
    rotDir.z = glm::sin(glm::radians(m_Yaw)) * glm::cos(glm::radians(m_Pitch));
    m_Forward = rotDir;
    RecalculateMatrix();
  }
}

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
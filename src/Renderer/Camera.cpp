#include "Camera.h"
#include "FixedGlobals.h"
#include "Processes/WorldGenerator.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

void Camera::GUIUpdate()
{
  ImGui::Begin("Camera");
  if(ImGui::DragFloat3("Position", glm::value_ptr(m_Position)))
    SetPosition(m_Position);
  ImGui::DragFloat("FOV", &m_FOV);
  ImGui::DragFloat("Near Plane", &m_NearPlane);
  ImGui::DragFloat("Far Plane", &m_FarPlane);
  ImGui::DragFloat("Speed", &m_Speed);
  ImGui::DragFloat("Sensitivity", &m_Sensitivity);
  ImGui::End();
  RecalculateMatrix();
}

Camera::Camera(float fov, float nearP, float farP, uint32_t width,
               uint32_t height)
    : m_Width(width), m_Height(height), m_Position(8.0f, 25.0f, 8.0f),
      m_Up(0.0f, 1.0f, 0.0f), m_Forward(0.0f, 0.0f, -1.0f), m_FOV(fov),
      m_NearPlane(nearP), m_FarPlane(farP)
{
  m_Speed = 5.0f;
  m_Sensitivity = 0.3f;
  m_Yaw = -90.0f;
  m_Pitch = 0.0f;
  m_FirstMouse = true;
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
  if(!LockInput)
  {
    if(keyboard[SDL_SCANCODE_W])
      SetPosition((m_Position += m_Forward * m_Speed * dt));
    if(keyboard[SDL_SCANCODE_S])
      SetPosition((m_Position -= m_Forward * m_Speed * dt));
    if(keyboard[SDL_SCANCODE_A])
      SetPosition((m_Position -= (glm::normalize(glm::cross(m_Forward, m_Up)) *
                                  m_Speed * dt)));
    if(keyboard[SDL_SCANCODE_D])
      SetPosition((m_Position += (glm::normalize(glm::cross(m_Forward, m_Up)) *
                                  m_Speed * dt)));
    if(keyboard[SDL_SCANCODE_E])
      SetPosition((m_Position += m_Up * m_Speed * dt));
    if(keyboard[SDL_SCANCODE_Q])
      SetPosition((m_Position -= m_Up * m_Speed * dt));
  }
  GUIUpdate();
}
void Camera::OnEvent(const SDL_Event &event)
{
  if(!LockInput)
  {
    switch(event.type)
    {
    case SDL_EVENT_MOUSE_MOTION: {
      if(m_FirstMouse)
      {
        m_FirstMouse = false;
        return;
      }
      m_Yaw += event.motion.xrel * m_Sensitivity;
      m_Pitch -= event.motion.yrel * m_Sensitivity;

      m_Pitch = glm::clamp(m_Pitch, -89.9f, 89.9f);

      glm::vec3 rotDir = m_Forward;
      rotDir.x =
          glm::cos(glm::radians(m_Yaw)) * glm::cos(glm::radians(m_Pitch));
      rotDir.y = glm::sin(glm::radians(m_Pitch));
      rotDir.z =
          glm::sin(glm::radians(m_Yaw)) * glm::cos(glm::radians(m_Pitch));
      m_Forward = rotDir;
      RecalculateMatrix();
      break;
    }
    case SDL_EVENT_MOUSE_WHEEL: {
      const SDL_MouseWheelEvent &wheelEvent = event.wheel;
      m_Speed += wheelEvent.y;
      m_Speed = glm::clamp(m_Speed, 0.0f, 1000.0f);
      break;
    }
    }
  }
}

// SetGet ers
void Camera::SetPosition(const glm::vec3 &pos)
{
  m_Position = pos;
  RecalculateMatrix();

  if(!Spectating)
  {
    World::UnloadUnseenChunks();
    World::GenerateWorld();
  }
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

const glm::ivec3 Camera::GetChunkPosition() const
{
  return {static_cast<int>(std::floor(m_Position.x / Global::CHUNK_SIZE_X) *
                           Global::CHUNK_SIZE_X),
          static_cast<int>(std::floor(m_Position.y / Global::CHUNK_SIZE_Y) *
                           Global::CHUNK_SIZE_Y),
          static_cast<int>(std::floor(m_Position.z / Global::CHUNK_SIZE_Z) *
                           Global::CHUNK_SIZE_Z)};
}
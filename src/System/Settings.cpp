#include "Processes/WorldGenerator.h"
#include "Renderer/GladFunctions.h"
#include "Settings.h"

#include <imgui.h>

#include <print>

int Settings::m_RenderDistance = 1;
bool Settings::m_VSync = false;
bool Settings::m_WireframeMode = false;
bool Settings::Visible = true;

void Settings::Init()
{
  ToggleVSync(m_VSync);
  ToggleWireframeMode(m_WireframeMode);
}

void Settings::SetVideoSettings(VideoSettingsOptions option, int value)
{
  switch(option)
  {
  case VideoSettingsOptions::NoOption: {
    std::println("You Didn't Select A Valid Option");
    break;
  }
  case VideoSettingsOptions::RenderDistance: {
    m_RenderDistance = value;
    break;
  }
  case VideoSettingsOptions::VSync: {
    m_VSync = bool(value);
    break;
  }
  case VideoSettingsOptions::WireframeMode: {
    m_WireframeMode = bool(value);
    break;
  }
  default: {
    std::println("You Didn't Select A Valid Option");
  }
  };
}

// clang-format off
int Settings::GetVideoSettings(VideoSettingsOptions option)
{
  switch(option)
  {
  case VideoSettingsOptions::NoOption: {
    std::println("Select A Valid Option");
    break;
  }
  case VideoSettingsOptions::RenderDistance:  return m_RenderDistance;
  case VideoSettingsOptions::VSync:           return int(m_VSync);
  case VideoSettingsOptions::WireframeMode:   return int(m_WireframeMode);
  default: {
    std::println("Select A Valid Option");
  }
  }
}
// clang-format on

void Settings::UpdateGUI()
{
  if(Visible)
  {
    if(ImGui::Begin("Settings", &Visible))
    {
      if(ImGui::Button("Toggle Wireframe"))
      {
        m_WireframeMode = !m_WireframeMode;
        ToggleWireframeMode(m_WireframeMode);
      }
      ImGui::Separator();
      if(ImGui::DragInt("Render Distance", &m_RenderDistance))
      {
        World::UnloadUnseenChunks();
        World::GenerateWorld();
      }
      if(ImGui::Checkbox("VSync", &m_VSync))
      {
        ToggleVSync(m_VSync);
      }
      ImGui::End();
    }
  }
}
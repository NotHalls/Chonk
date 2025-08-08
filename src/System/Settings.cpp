#include "Processes/WorldGenerator.h"
#include "Renderer/GladFunctions.h"
#include "Scene.h"
#include "Settings.h"

#include <imgui.h>

#include <print>

int Settings::m_RenderDistance = 3;
bool Settings::m_Spectating = false;
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
    ToggleVSync(value);
    break;
  }
  case VideoSettingsOptions::WireframeMode: {
    m_WireframeMode = bool(value);
    ToggleWireframeMode(value);
    break;
  }
  default:
    std::println("You Didn't Select A Valid Option");
  };
}

void Settings::SetGameSettings(GameSettingsOptions option, int value)
{
  switch(option)
  {
  case GameSettingsOptions::NoOption: {
    std::println("You Didn't Select A Valid Option");
    break;
  }
  case GameSettingsOptions::Spectating: {
    m_Spectating = bool(value);
    Scene::GetCamera()->OnSpectateChange(m_Spectating);
    break;
  }
  default:
    std::println("You Didn't Select A Valid Option");
  }
}

// clang-format off
int Settings::GetVideoSettings(VideoSettingsOptions option)
{
  switch(option)
  {
  case VideoSettingsOptions::NoOption: {
    std::println("You Didn't Select A Valid Option");
    break;
  }
  case VideoSettingsOptions::RenderDistance:  return m_RenderDistance;
  case VideoSettingsOptions::VSync:           return int(m_VSync);
  case VideoSettingsOptions::WireframeMode:   return int(m_WireframeMode);
  default: {
    std::println("You Didn't Select A Valid Option");
  }
  }
}

int Settings::GetGameSettings(GameSettingsOptions option)
{
  switch(option)
  {
    case GameSettingsOptions::NoOption: {
      std::println("You Didn't Select A Valid Option");
      break;
    }
    case GameSettingsOptions::Spectating: return m_Spectating;
  }
}
// clang-format on

void Settings::UpdateGUI()
{
  if(Visible)
  {
    if(ImGui::Begin("Settings", &Visible))
    {
      ImGui::SeparatorText("Game Settings");
      if(ImGui::Checkbox("Spectate", &m_Spectating))
      {
        Scene::GetCamera()->OnSpectateChange(m_Spectating);
      }

      ImGui::Spacing();
      ImGui::SeparatorText("Video Settings");

      if(ImGui::DragInt("Render Distance", &m_RenderDistance, 1, 2, 32))
      {
        World::UnloadUnseenChunks();
        World::GenerateWorld();
      }
      ImGui::Spacing();
      if(ImGui::Checkbox("VSync", &m_VSync))
      {
        ToggleVSync(m_VSync);
      }
      ImGui::SameLine();
      if(ImGui::Checkbox("Wireframe", &m_WireframeMode))
      {
        ToggleWireframeMode(m_WireframeMode);
      }

      ImGui::End();
    }
  }
}
#pragma once

#include "Include/Defines.h"

// clang-format off
enum class VideoSettingsOptions : short
{
  NoOption        = 0,
  RenderDistance  = 1,
  VSync           = 2,
  WireframeMode   = 3
};

enum class VideoSettingsToggles : short
{
  Unknown       = 0,
  VSync         = 1,
  WireframeMode = 2
};
// clang-format on

class Settings
{
public:
  static void SetVideoSettings(VideoSettingsOptions option, int value);
  static const int GetVideoSettings(VideoSettingsOptions option);

  static void UpdateGUI();

public:
  static bool Visible;

private:
  // Video Settings
  static int m_RenderDistance;
  static bool m_VSync;
  static bool m_WireframeMode;
};
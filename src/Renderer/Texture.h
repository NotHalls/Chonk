#pragma once

#include <cstdint>
#include <string>

class Texture
{
public:
  Texture(const std::string &file, uint32_t mipmapLvl = 1);

  uint32_t Get() const { return m_ID; }
  const std::string &GetFile() const { return m_File; }

  void Bind(uint32_t slot = 0);
  void Unbind(uint32_t slot = 0);

private:
  void ProcessTexture();

private:
  std::string m_File;
  uint32_t m_ID;

  // texture properties
  uint32_t m_Width;
  uint32_t m_Height;
  uint32_t m_Mipmaps;
};
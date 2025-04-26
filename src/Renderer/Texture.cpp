#include <iostream>

#include <SDL3/SDL_surface.h>
#include <glad/glad.h>
#include <stb_image.h>

#include "Texture.h"

Texture::Texture(const std::string &file, uint32_t mmLVL)
    : m_File(file), m_Mipmaps(mmLVL)
{
  ProcessTexture();
}

void Texture::ProcessTexture()
{
  // fetching the img data into memory from the img file
  int width, height, channels;
  stbi_set_flip_vertically_on_load(1);
  uint8_t *data = stbi_load(m_File.c_str(), &width, &height, &channels, 0);
  if(!data)
  {
    std::cerr << "Failed To Load Texture: " << m_File << std::endl;
    return;
  }
  m_Width = width;
  m_Height = height;

  int internalFormat, imageFormat;
  if(channels == 4)
  {
    internalFormat = GL_RGBA8;
    imageFormat = GL_RGBA;
  }
  else if(channels == 3)
  {
    internalFormat = GL_RGB8;
    imageFormat = GL_RGB;
  }

  // creating the texture from data in memory
  glCreateTextures(GL_TEXTURE_2D, 1, &m_ID);
  glTextureStorage2D(m_ID, m_Mipmaps, internalFormat, width, height);

  // texture settings
  glTextureParameteri(m_ID, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTextureParameteri(m_ID, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTextureParameteri(m_ID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glTextureSubImage2D(m_ID, 0, 0, 0, m_Width, m_Height, imageFormat,
                      GL_UNSIGNED_BYTE, data);
  // generating mipmaps
  glGenerateTextureMipmap(m_ID);

  stbi_image_free(data);
}

void Texture::Bind(uint32_t slot)
{
  glBindTextureUnit(slot, m_ID);
  glActiveTexture(GL_TEXTURE0 + slot);
}
void Texture::Unbind(uint32_t slot) { glBindTextureUnit(slot, 0); }
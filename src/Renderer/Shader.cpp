#include <fstream>
#include <iostream>
#include <sstream>

#include <glad/glad.h>

#include "Shader.h"

GLenum ToGlShaderType(ShaderType type)
{
  // clang-format off
  switch(type)
  {
    case ShaderType::Vertex:    return GL_VERTEX_SHADER;
    case ShaderType::Fragment:  return GL_FRAGMENT_SHADER;
    case ShaderType::Geometry:  return GL_GEOMETRY_SHADER;
    case ShaderType::Unknown:   return GL_INVALID_ENUM;
  }
  // clang-format on
}

Shader::Shader(const std::unordered_map<std::string, ShaderType> &files)
{
  m_ID = glCreateProgram();
  for(const auto &[path, type] : files)
  {
    std::string source = ReadFile(path);
    uint32_t shader = CompileShader(source, type);
    glAttachShader(m_ID, shader);
    glDeleteShader(shader);
  }

  glLinkProgram(m_ID);

  int status;
  glGetProgramiv(m_ID, GL_LINK_STATUS, &status);
  if(!status)
  {
    char errMsg[512];
    glGetProgramInfoLog(m_ID, 512, nullptr, errMsg);
    std::cerr << "Failed To Link Shader!\n" << errMsg << std::endl;
  }
}
Shader::~Shader() { glDeleteProgram(m_ID); }

std::string Shader::ReadFile(const std::string &path)
{
  std::ifstream file(path);
  if(!file.is_open())
  {
    std::cerr << "Failed To Load Shader: " << path << std::endl;
    return "";
  }

  std::stringstream shaderContent;
  shaderContent << file.rdbuf();
  return shaderContent.str();
}

uint32_t Shader::CompileShader(const std::string &source, ShaderType type)
{
  uint32_t shader = glCreateShader(ToGlShaderType(type));
  const char *shaderSrc = source.c_str();
  glShaderSource(shader, 1, &shaderSrc, nullptr);
  glCompileShader(shader);

  int status;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
  if(!status)
  {
    char errorMsg[512];
    glGetShaderInfoLog(shader, 512, nullptr, errorMsg);
    std::cerr << "Failed To Compile Shader!\n" << errorMsg << std::endl;
  }

  return shader;
}

void Shader::Bind() { glUseProgram(m_ID); }
void Shader::Unbind() { glUseProgram(0); }
#include "Debug/Assert.h"
#include "Shader.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <iostream>
#include <sstream>

GLenum ToGlShaderType(ShaderType type)
{
  // clang-format off
  switch(type)
  {
    case ShaderType::Vertex:    return GL_VERTEX_SHADER;
    case ShaderType::Fragment:  return GL_FRAGMENT_SHADER;
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
    CHK_ASSERT(false, "Failed To Link Shader!\n" + std::string(errMsg));
  }
}
Shader::~Shader() { glDeleteProgram(m_ID); }

std::string Shader::ReadFile(const std::string &path)
{
  std::ifstream file(path);
  CHK_ASSERT(file.is_open(),
             "Failed To Read The Shader File: " + std::string(path));

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
    CHK_ASSERT(false, "Failed To Compile " + ShaderTypeToName(type) +
                          std::string(" Shader!\n") + std::string(errorMsg));
  }

  return shader;
}

// clang-format off
std::string Shader::ShaderTypeToName(ShaderType type)
{
  switch(type)
  {
  case ShaderType::Unknown: return "Unknown";
  case ShaderType::Vertex: return "Vertex";
  case ShaderType::Fragment: return "Fragment";
  default: "Failed To Get ShaderType String From The Given Type";
  }
}
// clang-format on

void Shader::Bind() { glUseProgram(m_ID); }
void Shader::Unbind() { glUseProgram(0); }

// Uniform Functions
void Shader::SetUniformInt(const std::string &name, int value)
{
  int uniformLoc = glGetUniformLocation(m_ID, name.c_str());
  CHK_ASSERT(uniformLoc != -1, "Failed To Get Uniform With Name: " + name);
  glUniform1i(uniformLoc, value);
}
void Shader::SetUniformMat4(const std::string &name, const glm::mat4 value)
{
  int uniformLoc = glGetUniformLocation(m_ID, name.c_str());
  CHK_ASSERT(uniformLoc != -1, "Failed To Get Uniform With Name: " + name);
  glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, glm::value_ptr(value));
}
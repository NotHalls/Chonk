#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>

enum class ShaderType : uint8_t
{
  Unknown = 0,
  Vertex = 1,
  Fragment = 2,
  Geometry = 3
};

class Shader
{
public:
  Shader(const std::unordered_map<std::string, ShaderType> &files);
  ~Shader();

  void Bind();
  void Unbind();
  const uint32_t Get() const { return m_ID; }

private:
  std::string ReadFile(const std::string &path);
  uint32_t CompileShader(const std::string &source, ShaderType type);
  void CreateShaderProgram();

private:
  uint32_t m_ID;
};
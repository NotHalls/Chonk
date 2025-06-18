#version 460

layout(location = 0) in vec3 a_Pos;
layout(location = 1) in vec2 a_UV;

uniform mat4 u_MVP;

out vec2 o_UVAtlas;

void main()
{
  gl_Position = u_MVP * vec4(a_Pos, 1.0f);
  o_UVAtlas = a_UV;
}
#version 460

uniform sampler2D u_Texture0;

out vec4 FragColor;

in vec2 o_UV;

void main()
{
  // FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
  FragColor = texture(u_Texture0, o_UV);
}
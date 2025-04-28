#version 460

// out block textures
uniform sampler2D u_Texture0;

out vec4 FragColor;

in vec2 o_UVAtlas;

void main() { FragColor = texture(u_Texture0, o_UVAtlas); }
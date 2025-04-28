#version 460

layout(location = 0) in vec3 a_Pos;
layout(location = 1) in vec2 a_UV;
layout(location = 2) in float a_FaceID;

uniform mat4 u_MVP;
uniform int u_Textures[3];

out vec2 o_UVAtlas;

vec2 CalculateTextureForFace(int textureID, vec2 uvs)
{
  const int numRows = 5;
  const int numCols = 5;

  int col = textureID % numCols;
  int row = textureID / numCols;

  vec2 tileOffset =
      vec2(float(col) / float(numCols), float(row) / float(numRows));
  vec2 tileScale = vec2(1 / float(numCols), 1 / float(numRows));

  return vec2(tileOffset + uvs * tileScale);
}

void main()
{
  gl_Position = u_MVP * vec4(a_Pos, 1.0f);
  int selectedTexture = u_Textures[int(a_FaceID)];
  o_UVAtlas = CalculateTextureForFace(selectedTexture, a_UV);
}
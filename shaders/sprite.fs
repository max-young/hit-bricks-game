#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D sprite;
uniform vec3 spriteColor;

void main()
{
  vec4 texColor = texture(sprite, TexCoords);
  if (texColor.a < 0.1)
    discard;
  color = vec4(spriteColor, 1.0) * texture(sprite, TexCoords);
}
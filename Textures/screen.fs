#version 330 core
out vec4 FragColor;

in vec2 texCoord;

uniform sampler2D screenTexture;

void main()
{

   vec3 col = texture(screenTexture, texCoord).rgb;
   FragColor = vec4(col, 1.0);
}
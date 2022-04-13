#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 texCoord;

uniform sampler2D ourTexture;
uniform sampler2D ourTexture2;

uniform float mixValue;

void main()
{
   FragColor = mix(texture(ourTexture, texCoord), texture(ourTexture2, texCoord), mixValue);
}
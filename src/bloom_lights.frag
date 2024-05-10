#version 330 core

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in vec3 fragColor;

void main()
{
	FragColor = vec4(fragColor, 1.0f);
	BrightColor = FragColor; //No need to check for brightness, lights should always be sent.
}
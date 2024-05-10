#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec3 normal;

out vec3 vertexColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0f);

	vec3 direction = vec3(-1.0f, 1.0f, 1.0f);
	
	//Fixed directional light.
	float diffuse = max(dot(normal, direction), 0.1f);
	
	vertexColor = color * diffuse / 1.7f;
}


#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec2 texCoord;
layout (location = 4) in vec3 tangent;
layout (location = 5) in vec3 bitangent;

out vec3 fragColor;
out vec3 fragNormal;
out vec2 fragTexCoord;
out vec3 lightPos;
out vec3 viewPos;
out vec3 fragPos;
out vec3 tangentLightPos;
out vec3 tangentViewPos;
out vec3 tangentFragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 viewPosition;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0f);

	vec3 fragPosition = vec3(model * vec4(position, 1.0f));
	fragPos = fragPosition;
	fragColor = color;

	//Transform the normal into world space so that scale and
	//rotation are respected.
	mat3 normalMatrix = transpose(inverse(mat3(model)));
	fragNormal = normalMatrix * normal;

	fragTexCoord = texCoord;

	//Positional light.
	lightPos = vec3(-2.0f, 4.0f, 4.0f);

	//Compute inverse TBN matrix to transform world space light vectors to tangent space.
	//This allows the tangent-space normals to have proper light computation in the fragment shader.
	vec3 T = normalize(normalMatrix * tangent);
	vec3 B = normalize(normalMatrix * bitangent);
	mat3 TBN = transpose(mat3(T, B, fragNormal));
	tangentLightPos = TBN * lightPos;
	tangentViewPos = TBN * viewPosition;
	tangentFragPos = TBN * fragPosition;
}


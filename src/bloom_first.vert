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

out vec3 tangentViewPos;
out vec3 tangentFragPos;

out vec3 tangentLightPositions[8];
out vec3 lightColors[8];

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 viewPosition;
uniform bool normalMappingEnabled;

void setupLights();

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0f);

	vec3 fragPosition = vec3(model * vec4(position, 1.0f));
	fragColor = color;
	fragTexCoord = texCoord;

	//Transform the normal into world space so that scale and
	//rotation are respected.
	mat3 normalMatrix = transpose(inverse(mat3(model)));

	setupLights();

	//Compute inverse TBN matrix to transform world space light vectors to tangent space.
	//This allows the tangent-space normals to have proper light computation in the fragment shader.
	vec3 T = normalize(normalMatrix * tangent);
	vec3 B = normalize(normalMatrix * bitangent);
	fragNormal = normalMatrix * normal;
	mat3 TBN = transpose(mat3(T, B, fragNormal));
	tangentViewPos = viewPosition;
	tangentFragPos = fragPosition;

	if (normalMappingEnabled)
	{
		tangentViewPos = TBN * viewPosition;
		tangentFragPos = TBN * fragPosition;
		
		//Transform lights into tangent space.
		for (int i = 0; i < 8; i++)
		{
			tangentLightPositions[i] = TBN * tangentLightPositions[i];
		}
	}
}

void setupLights()
{
	//Orange-ish light.
	tangentLightPositions[0] = vec3(-2.0f, 4.0f, 4.0f);
	lightColors[0] = vec3(2.0f, 1.14f, 0.78f);

	//Red light.
	tangentLightPositions[1] = vec3(5.0f, 3.0f, 5.0f);
	lightColors[1] = vec3(3.0f, 0.0f, 0.0f);

	//Green light.
	tangentLightPositions[2] = vec3(-5.0f, 3.0f, -5.0f);
	lightColors[2] = vec3(0.0f, 2.0f, 0.0f);

	//Blue light.
	tangentLightPositions[3] = vec3(5.0f, 3.0f, -5.0f);
	lightColors[3] = vec3(0.0f, 0.0f, 4.0f);

	//Yellowish/White lights.
	tangentLightPositions[4] = vec3(10.0f, 5.0f, 10.0f);
	lightColors[4] = vec3(1.6f, 1.7f, 0.6f);

	tangentLightPositions[5] = vec3(-10.0f, 5.0f, -10.0f);
	lightColors[5] = vec3(1.6f, 1.7f, 0.6f);

	tangentLightPositions[6] = vec3(10.0f, 5.0f, -10.0f);
	lightColors[6] = vec3(1.6f, 1.7f, 0.6f);

	tangentLightPositions[7] = vec3(-10.0f, 5.0f, 10.0f);
	lightColors[7] = vec3(1.6f, 1.7f, 0.6f);
}
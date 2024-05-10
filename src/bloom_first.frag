#version 330 core

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in vec3 fragColor;
in vec3 fragNormal;
in vec2 fragTexCoord;

in vec3 tangentViewPos;
in vec3 tangentFragPos;

in vec3 tangentLightPositions[8];
in vec3 lightColors[8];

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;
uniform sampler2D depthMap;
uniform bool normalMappingEnabled;

void main()
{
	vec3 normal = normalize(fragNormal); //Use default normal for ligthing.
	if (normalMappingEnabled)
	{
		//If normal mapping is enabled, use the normal on a per-fragment basis.
		//Get normal from normal map.
		normal = texture(normalMap, fragTexCoord).rgb;
		//Convert from range [0, 1] to range [-1, -1]
		normal = normalize(normal * 2.0 - 1.0);
	}

	//From diffuse map.
	vec3 diffuseColor = texture(diffuseMap, fragTexCoord).rgb;
	vec3 ambient = 0.1 * diffuseColor;

	//Specular light setup.
	float specularStrength = 0.6f;
	float shininess = 16;
	vec3 viewDir = normalize(tangentViewPos - tangentFragPos);

	//From light arrays...
	vec3 diffuse;
	vec3 specular;
	
	vec3 lightDir[8];
	float dist[8];
	float diff;
	vec3 diffuseResult;
	vec3 specularResult;
	
	for (int i = 0; i < 8; i++)
	{
		lightDir[i] = normalize(tangentLightPositions[i] - tangentFragPos);
		dist[i] = abs(length(tangentLightPositions[i] - tangentFragPos));
		diff = max(dot(lightDir[i], normal), 0.0f);
		diffuseResult = lightColors[i] * diff * diffuseColor;

		//Specular	
		vec3 reflectDir = reflect(-lightDir[i], normal);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0f), shininess);
		vec3 specularResult = specularStrength * spec * lightColors[i];
		
		//Attenuation:
		//Diffuse/Specular is completely black at distance >= 10.
		float range = 10.0f;
		float darkness = max((range - (dist[i] - 1.0f)) / range, 0.0f);
		diffuseResult = diffuseResult * max((range - (dist[i] - 1.0f)) / range, 0.0f);
		specularResult = specularResult * max((range - (dist[i] - 1.0f)) / range, 0.0f);

		diffuse += diffuseResult;
		specular += specularResult;
	}

	FragColor = vec4(ambient + diffuse + specular, 1.0f);

	float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
	if (brightness > 0.7)
		BrightColor = vec4(FragColor.rgb, 1.0);
}
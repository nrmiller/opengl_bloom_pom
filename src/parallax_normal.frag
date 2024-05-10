#version 330 core

in vec3 fragColor;
in vec3 fragNormal;
in vec2 fragTexCoord;
in vec3 lightPos;
in vec3 viewPos;
in vec3 fragPos;
in vec3 tangentLightPos;
in vec3 tangentViewPos;
in vec3 tangentFragPos;
in mat3 TBN;

layout (location = 0) out vec4 color;
layout (location = 1) out vec4 brightColor;

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;
uniform sampler2D depthMap;
uniform bool normalMappingEnabled;
uniform bool parallaxMappingEnabled;

vec2 ParallaxOcclusionMapping(vec2 texCoordA, vec3 viewDir);

void main()
{
	//For parallax mapping, we shift the tex coords to cause
	//the illusion of a depth map.
	vec2 texCoord = fragTexCoord;
	if (parallaxMappingEnabled)
	{
		vec3 viewDir = normalize(tangentViewPos - tangentFragPos);
		texCoord = ParallaxOcclusionMapping(texCoord, viewDir);
		if (texCoord.x < 0.0 || texCoord.x > 1.0 ||
			texCoord.y < 0.0 || texCoord.y > 1.0)
		{
			discard;
		}
	}

	vec3 normal = normalize(fragNormal); //Use default normal for ligthing.
	if (normalMappingEnabled)
	{
		//If normal mapping is enabled, use the normal on a per-fragment basis.
		//Get normal from normal map.
		normal = texture(normalMap, texCoord).rgb;
		//Convert from range [0, 1] to range [-1, -1]
		normal = normalize(normal * 2.0 - 1.0);
	}

	//Positional light.
	vec3 lightPos = vec3(-2.0f, 4.0f, 4.0f);

	//From diffuse map.
	vec3 diffuseColor = texture(diffuseMap, texCoord).rgb;
	vec3 ambient = 0.1 * diffuseColor;

	vec3 lightDir = normalize(tangentLightPos - tangentFragPos);
	float dist = abs(length(tangentLightPos - tangentFragPos));
	if (!normalMappingEnabled)
	{
		//If normal mapping not enabled, then use world space lighting.
		lightDir = normalize(lightPos - fragPos);
		dist = abs(length(lightPos - fragPos));
	}
	float diff = max(dot(lightDir, normal), 0.0f);
	vec3 diffuse = diff * diffuseColor;
	
	//Specular light.
	float specularStrength = 0.2f;
	float shininess = 32;
	vec3 viewDir = normalize(tangentViewPos - tangentFragPos);
	if (!normalMappingEnabled)
	{
		//If normal mapping not enabled, then use world space lighting.
		viewDir = normalize(viewPos - fragPos);
	}

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), shininess);
	vec3 specular = specularStrength * spec * vec3(1.0f, 0.76f, 0.39f);
	
	//Suppose distance > 1 starts to fade to black.
	//Diffuse/Specular is completely black at distance >= 5.
	float range = 5.0f;
	float darkness = max((range - (dist - 1.0f)) / range, 0.0f);
	if (dist > 1)
	{
		diffuse = diffuse * max((range - (dist - 1.0f)) / range, 0.0f);
		specular = specular * max((range - (dist - 1.0f)) / range, 0.0f);
	}

	color = vec4(ambient + diffuse + specular, 1.0f);
	brightColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
}

vec2 ParallaxOcclusionMapping(vec2 texCoord, vec3 viewDir)
{
	const float minLayers = 10;
    const float maxLayers = 100;
	//If viewing from orthogonal, use maximum layers,
	//If viewing from parallel, use minimum layers.
    float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));  


	float layerDepth = 1.0f / numLayers; //Size of each layer.
	float currentLayerDepth = 0.0f;
	vec2 P = viewDir.xy / viewDir.z * 0.1f;
	vec2 deltaTexCoord = P / numLayers;

	vec2 currentTexCoord = texCoord;
	float currentDepthMapValue  = texture(depthMap, texCoord).r;

	while (currentLayerDepth < currentDepthMapValue)
	{
		currentTexCoord -= deltaTexCoord;
		currentDepthMapValue = texture(depthMap, currentTexCoord).r;
		currentLayerDepth += layerDepth;
	}

	//Lerp betweent the last to points.
	vec2 previousTexCoord = currentTexCoord + deltaTexCoord;
	float afterDepthDifference = currentDepthMapValue - currentLayerDepth;
	float beforeDepthDifference = texture(depthMap, previousTexCoord).r - currentLayerDepth + layerDepth;
	
	//When weight = 1, then fully beforeDepth.
	//When weight = 0, then fully afterDepth.
	float weight = afterDepthDifference / (afterDepthDifference - beforeDepthDifference);
	return mix(currentTexCoord, previousTexCoord, weight);
}
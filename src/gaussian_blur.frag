#version 330 core

out vec4 color;

in vec3 fragColor;
in vec2 fragTexCoord;

uniform sampler2D image;

uniform bool horizontal;

uniform float weight[5] = float[]  (0.227027f, 0.1945946f, 0.1216216f, 0.054054f, 0.016216f);

void main()
{
	float dist = 2.0f;

	//Get texel size (width & height).
	vec2 texelSize = dist / textureSize(image, 0);
	
	//Contribution of the blur from the current fragment.
	vec3 result = texture(image, fragTexCoord).rgb * weight[0];

	//Allow nearby fragments to affect the current fragment's output as well.
	if (horizontal)
	{
		//Horizontal Pass
		for (int i = 1; i < 5; i++)
		{
			result += texture(image, fragTexCoord + vec2(texelSize.x * i, 0.0f)).rgb * weight[i];
			result += texture(image, fragTexCoord - vec2(texelSize.x * i, 0.0f)).rgb * weight[i];
		}
	}
	else
	{
		//Vertical Pass
		for (int i = 1; i < 5; i++)
		{
			result += texture(image, fragTexCoord + vec2(0.0f, texelSize.y * i)).rgb * weight[i];
			result += texture(image, fragTexCoord - vec2(0.0f, texelSize.y * i)).rgb * weight[i];
		}
	}
	//Send guassian-blurred result to colorbuffer.
	color = vec4(result, 1.0f);
}
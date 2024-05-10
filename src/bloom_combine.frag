#version 330 core

in vec3 fragColor;
in vec2 fragTexCoord;

out vec4 color;

uniform sampler2D scene;
uniform sampler2D bloomBlur;

uniform int bloomEnabled;

void main()
{
	vec3 sceneColor = texture(scene, fragTexCoord).rgb;
	vec3 bloomColor = texture(bloomBlur, fragTexCoord).rgb;
	color = vec4(sceneColor + bloomColor * bloomEnabled, 1.0f);
}
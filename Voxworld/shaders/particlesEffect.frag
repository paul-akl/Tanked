#version 330 core

layout(location = 0) out vec4 emissiveBuffer;
layout(location = 1) out vec4 finalBuffer;

uniform vec4 colour;
uniform float maxLifeTime;

in float timeToLive;

void main()
{
	vec4 particleColor = vec4(colour.xyz, (timeToLive / maxLifeTime)*0.5);

	emissiveBuffer = particleColor;
	finalBuffer = particleColor;
}
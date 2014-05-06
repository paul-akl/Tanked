#version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 3) in float TTL;

uniform mat4 MVP;

out float timeToLive;

void main(void)
{
	timeToLive = TTL;
	gl_Position =  MVP * vec4(vertexPosition, 1.0);
}
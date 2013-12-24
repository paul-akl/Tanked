#version 330 core

layout(location = 0) in vec3 vertexPosition;

uniform mat4 pointLightMVP;

void main()
{
	gl_Position =  pointLightMVP * vec4(vertexPosition, 1.0);
}
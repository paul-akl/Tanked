#version 330 core

layout(location = 0) in vec3 vertexPosition;

uniform mat4 dirLightMat;

void main()
{
	gl_Position =  dirLightMat * vec4(vertexPosition, 1.0);
}
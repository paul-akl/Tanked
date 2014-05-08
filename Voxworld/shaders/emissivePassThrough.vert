#version 330

layout(location = 0) in vec3 VertexPosition;
layout(location = 3) in vec2 vertexUV;

out vec2 texCoord;

void main(void)
{
    texCoord = vertexUV;
	gl_Position = vec4(VertexPosition, 1.0);
}
#version 330

uniform mat4 modelview;
uniform mat4 projection;

layout(location = 0) in vec3 VertexPosition;
layout(location = 3) in vec2 TextureCoordinates;

out vec2 texCoord;

void main(void)
{
	texCoord = TextureCoordinates;
	gl_Position = projection * modelview * vec4(VertexPosition,1.0);
}
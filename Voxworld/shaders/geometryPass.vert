#version 330

uniform mat4 modelview;
uniform mat4 modelMatrix;
uniform mat3 normalMatrix;
uniform mat4 projection;

layout(location = 0) in vec3 VertexPosition;
layout(location = 3) in vec2 TextureCoordinates;

out vec3 worldPos;
out vec2 texCoord;
out vec3 normal;

void main(void)
{
    worldPos = (modelMatrix * vec4(VertexPosition, 1.0)).xyz;
	normal = (modelMatrix * vec4(VertexNormal, 0.0)).xyz;
	texCoord = TextureCoordinates;

	gl_Position = projection * modelview * vec4(VertexPosition,1.0);
}
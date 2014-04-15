#version 330

uniform mat4 modelview;
uniform mat4 modelMatrix;
uniform mat3 normalMatrix;
uniform mat4 projection;

in layout(location = 0) vec3 VertexPosition;
in layout(location = 2) vec3 VertexNormal;
in layout(location = 3) vec2 TextureCoordinates;

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
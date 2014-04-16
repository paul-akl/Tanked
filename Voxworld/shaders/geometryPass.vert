#version 330

uniform mat4 modelview;
uniform mat4 modelMatrix;
uniform mat3 normalMatrix;
uniform mat4 projection;

layout(location = 0) in vec3 VertexPosition;
layout(location = 2) in vec3 VertexNormal;
layout(location = 3) in vec2 TextureCoordinates;
layout(location = 4) in vec3 VertexTangent;
layout(location = 5) in vec3 TextureBiTangent;

out vec3 worldPos;
out vec2 texCoord;
out vec3 normal;
out mat3 TBN;
out vec3 eyeDir;

void main(void)
{
    worldPos = (modelMatrix * vec4(VertexPosition, 1.0)).xyz;
	vec4 viewPos = projection * modelview * vec4(VertexPosition,1.0);
	texCoord = TextureCoordinates;
	eyeDir = normalize(-viewPos).xyz;
	mat3 normalMat = transpose(inverse(mat3(modelMatrix)));
	TBN = mat3(normalMat * VertexTangent, normalMat * TextureBiTangent, normalMat * VertexNormal);
	normal = (modelMatrix * vec4(VertexNormal, 0.0)).xyz;

	gl_Position = viewPos;
}
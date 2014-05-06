#version 330
// Some drivers require the following
precision highp float;

// Variables pass from vertex shader
in vec3 worldPos;
in vec2 texCoord;
in vec3 normal;
in vec3 eyeDir;
in mat3 TBN;

// Buffers assigned by GBuffer class
layout(location = 0) out vec4 positionBuffer;
layout(location = 1) out vec4 diffuseBuffer;
layout(location = 2) out vec4 normalBuffer;
layout(location = 3) out vec4 texCoordBuffer;
layout(location = 4) out vec4 emissiveBuffer;

uniform sampler2D diffuseMap;	// Model's texture
uniform sampler2D normalMap;
uniform sampler2D emissiveMap;
uniform sampler2D heightMap;
uniform sampler2D specularMap;

void main(void)
{
	float height = texture(heightMap, texCoord.st).r;
	float v = height*0.04-0.02;
	vec2 newCoords = texCoord+(eyeDir.xy*v);
	
	float specularPower = texture(specularMap, newCoords).r;
	vec4 emissiveTexture = texture(emissiveMap, newCoords).rgba;
	if(emissiveTexture.a<0.3)
	{
		emissiveTexture = vec4(0.0);
	}
	emissiveBuffer	= emissiveTexture * emissiveTexture.a;

	diffuseBuffer	= texture(diffuseMap, newCoords).rgba;	// Write the color from model's texture
	texCoordBuffer	= vec4(texCoord, 0.0, 0.0);				// Write texture coordinates (and an aditional values for specular)
	positionBuffer	= vec4(worldPos, 0.0);					// Write fragment's position in world space
	normalBuffer = vec4(TBN * normalize((2.0 * (texture(normalMap, newCoords).rgb) - 1.0)), specularPower);
}
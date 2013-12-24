#version 330
// Some drivers require the following
precision highp float;

// Variables pass from vertex shader
in vec3 worldPos;
in vec2 texCoord;
in vec3 normal;

// Buffers assigned by GBuffer class
layout(location = 0) out vec3 positionBuffer;
layout(location = 1) out vec3 diffuseBuffer;
layout(location = 2) out vec3 normalBuffer;
layout(location = 3) out vec3 texCoordBuffer;

uniform sampler2D diffuseMap;	// Model's texture

void main(void)
{
	diffuseBuffer	= texture2D(diffuseMap, texCoord).rgb;	// Write the color from model's texture
	texCoordBuffer	= vec3(texCoord, 0.0);					// Write texture coordinates (and an aditional value for specular)
	positionBuffer	= worldPos;								// Write fragment's position in world space
	normalBuffer	= normalize(normal);					// Write the normal vector
}
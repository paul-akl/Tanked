#version 330
// Some drivers require the following
//precision highp float;

out vec4 fragColor;

// Variables pass from vertex shader
in vec2 texCoord;

uniform sampler2D diffuseMap;

void main(void)
{
	fragColor	= texture(diffuseMap, texCoord).rgba;	// Write the color from model's texture
}
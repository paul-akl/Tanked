#version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 3) in vec2 vertexUV;

out vec2 blurTexCoords[14];
out vec2 texCoord;

uniform float blurOffset;

void main()
{
    texCoord = vertexUV;

    blurTexCoords[ 0] = texCoord + vec2(-0.028*blurOffset, 0.0);
    blurTexCoords[ 1] = texCoord + vec2(-0.024*blurOffset, 0.0);
    blurTexCoords[ 2] = texCoord + vec2(-0.020*blurOffset, 0.0);
    blurTexCoords[ 3] = texCoord + vec2(-0.016*blurOffset, 0.0);
    blurTexCoords[ 4] = texCoord + vec2(-0.012*blurOffset, 0.0);
    blurTexCoords[ 5] = texCoord + vec2(-0.008*blurOffset, 0.0);
    blurTexCoords[ 6] = texCoord + vec2(-0.004*blurOffset, 0.0);
    blurTexCoords[ 7] = texCoord + vec2( 0.004*blurOffset, 0.0);
    blurTexCoords[ 8] = texCoord + vec2( 0.008*blurOffset, 0.0);
    blurTexCoords[ 9] = texCoord + vec2( 0.012*blurOffset, 0.0);
    blurTexCoords[10] = texCoord + vec2( 0.016*blurOffset, 0.0);
    blurTexCoords[11] = texCoord + vec2( 0.020*blurOffset, 0.0);
    blurTexCoords[12] = texCoord + vec2( 0.024*blurOffset, 0.0);
    blurTexCoords[13] = texCoord + vec2( 0.028*blurOffset, 0.0);
	
    gl_Position = vec4(vertexPosition, 1.0);
}
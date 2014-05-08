#version 330
precision highp float;

#define EMISSIVE_CAP_DISTANCE 25

uniform vec3 cameraPos;
uniform vec2 screenSize;

uniform sampler2D positionMap;
uniform sampler2D emissiveMap;

//in vec2 texCoord;

out vec4 fragColor;

vec2 calcTexCoord(void)
{
    return gl_FragCoord.xy / screenSize;
}

void main(void)
{
	vec2 texCoord = calcTexCoord();
	vec3 worldPos = texture(positionMap, texCoord).xyz;
	fragColor = texture(emissiveMap, texCoord) * clamp((EMISSIVE_CAP_DISTANCE / length(worldPos - cameraPos)), 0.2, 0.8);
}
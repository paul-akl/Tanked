#version 330 core

//layout(location = 0) in vec3 position;
//layout(location = 3) in float timeToLive;

//layout(location = 4) out vec4 emissiveBuffer;
//layout(location = 5) out vec4 finalBuffer;

uniform vec4 colour;
uniform float maxLifeTime;

out vec4 fragColor;

in float timeToLive;

void main()
{    
	//vec4 outColour = vec4(colour.xyz, timeToLive / maxLifeTime);
	//emissiveBuffer = outColour;
	//finalBuffer = outColour;

	fragColor = vec4(colour.xyz, (timeToLive / maxLifeTime)*0.5);
	//finalBuffer = vec4(0.0, 1.0, 0.0, 1.0);;
}
#version 330 core
//precision mediump float;

out vec4 fragColorrrrr;

in vec2 blurTexCoords[14];
in vec2 texCoord;

uniform sampler2D emissiveMap;

void main(void)
{
    vec4 fragColor = vec4(0.0);
    fragColor += texture(emissiveMap, blurTexCoords[ 0])*0.0044299121055113265;
    fragColor += texture(emissiveMap, blurTexCoords[ 1])*0.00895781211794;
    fragColor += texture(emissiveMap, blurTexCoords[ 2])*0.0215963866053;
    fragColor += texture(emissiveMap, blurTexCoords[ 3])*0.0443683338718;
    fragColor += texture(emissiveMap, blurTexCoords[ 4])*0.0776744219933;
    fragColor += texture(emissiveMap, blurTexCoords[ 5])*0.115876621105;
    fragColor += texture(emissiveMap, blurTexCoords[ 6])*0.147308056121;
    fragColor += texture(emissiveMap, texCoord         )*0.259576912161;
    fragColor += texture(emissiveMap, blurTexCoords[ 7])*0.147308056121;
    fragColor += texture(emissiveMap, blurTexCoords[ 8])*0.115876621105;
    fragColor += texture(emissiveMap, blurTexCoords[ 9])*0.0776744219933;
    fragColor += texture(emissiveMap, blurTexCoords[10])*0.0443683338718;
    fragColor += texture(emissiveMap, blurTexCoords[11])*0.0215963866053;
    fragColor += texture(emissiveMap, blurTexCoords[12])*0.00895781211794;
    fragColor += texture(emissiveMap, blurTexCoords[13])*0.0044299121055113265;
	fragColorrrrr = fragColor;
	//fragColorrrrr = texture(emissiveMap, texCoord);
	//fragColorrrrr = vec4(0.0, 1.0, 0.0, 1.0);
}
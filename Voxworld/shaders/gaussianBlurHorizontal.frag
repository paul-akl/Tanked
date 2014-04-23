#version 330 core
//precision mediump float;

out vec4 fragColorrrrr;

in vec2 blurTexCoords[14];
in vec2 texCoord;

uniform sampler2D blurMap;
 
void main(void)
{
    vec4 fragColor = vec4(0.0);
    fragColor += texture(blurMap, blurTexCoords[ 0])*0.0044299121055113265;
    fragColor += texture(blurMap, blurTexCoords[ 1])*0.00895781211794;
    fragColor += texture(blurMap, blurTexCoords[ 2])*0.0215963866053;
    fragColor += texture(blurMap, blurTexCoords[ 3])*0.0443683338718;
    fragColor += texture(blurMap, blurTexCoords[ 4])*0.0776744219933;
    fragColor += texture(blurMap, blurTexCoords[ 5])*0.115876621105;
    fragColor += texture(blurMap, blurTexCoords[ 6])*0.147308056121;
    fragColor += texture(blurMap, texCoord         )*0.259576912161;
    fragColor += texture(blurMap, blurTexCoords[ 7])*0.147308056121;
    fragColor += texture(blurMap, blurTexCoords[ 8])*0.115876621105;
    fragColor += texture(blurMap, blurTexCoords[ 9])*0.0776744219933;
    fragColor += texture(blurMap, blurTexCoords[10])*0.0443683338718;
    fragColor += texture(blurMap, blurTexCoords[11])*0.0215963866053;
    fragColor += texture(blurMap, blurTexCoords[12])*0.00895781211794;
    fragColor += texture(blurMap, blurTexCoords[13])*0.0044299121055113265;
	fragColorrrrr = fragColor;
	//fragColorrrrr = texture(blurMap, texCoord);
	//fragColorrrrr = vec4(0.0, 1.0, 0.0, 1.0);
}
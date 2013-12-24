#version 330 core

out vec4 fragColor;
                                                      
struct BaseLight
{
    vec3  color;
    float ambientIntensity;
    float diffuseIntensity;
};
struct DirectionalLight
{
    BaseLight base;
    vec3 direction;
};
struct Attenuation
{
    float constant;
    float linear;
    float exp;
};

uniform sampler2D positionMap;
uniform sampler2D diffuseMap;
uniform sampler2D normalMap;

uniform	DirectionalLight directionalLight;

uniform float	specularIntensity;
uniform float	specularPower;

uniform vec3	cameraPos;
uniform vec2	screenSize;

vec4 calcLightInternal(BaseLight light_arg, vec3 lightDirection_arg, vec3 worldPos_arg, vec3 normal_arg)
{
    vec4  ambientColor	= vec4(light_arg.color, 1.0) * light_arg.ambientIntensity;
    float diffuseFactor = dot(normal_arg, -lightDirection_arg);

    vec4 diffuseColor  = vec4(0.0, 0.0, 0.0, 0.0);
    vec4 specularColor = vec4(0.0, 0.0, 0.0, 0.0);

    if(diffuseFactor > 0) 
	{
        diffuseColor = vec4(light_arg.color, 1.0) * light_arg.diffuseIntensity * diffuseFactor;

        vec3 vertexToEye = normalize(cameraPos - worldPos_arg);
        vec3 lightReflect = normalize(reflect(lightDirection_arg, normal_arg));
        float specularFactor = dot(vertexToEye, lightReflect);
        specularFactor = pow(specularFactor, specularPower);
        if(specularFactor > 0) 
		{
            specularColor = vec4(light_arg.color, 1.0) * specularIntensity * specularFactor;
        }
    }

    return (ambientColor + diffuseColor + specularColor);
}


vec4 calcDirectionalLight(vec3 worldPos_arg, vec3 normal_arg)
{
	return calcLightInternal(directionalLight.base, directionalLight.direction, worldPos_arg, normal_arg);
}

vec2 calcTexCoord()
{
    return gl_FragCoord.xy / screenSize;
}

void main()
{    
    vec2 texCoord = calcTexCoord();
	vec3 worldPos = texture(positionMap, texCoord).xyz;
	vec3 color = texture(diffuseMap, texCoord).xyz;
	vec3 normal = texture(normalMap, texCoord).xyz;
	normal = normalize(normal);

	fragColor = vec4(color, 1.0) * calcDirectionalLight(worldPos, normal);
	
	//calcLightInternal(directionalLight.base, directionalLight.direction, worldPos, normal);
}
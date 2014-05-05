#version 330

#define MAX_NUM_POINT_LIGHTS 100
#define MAX_NUM_SPOT_LIGHTS 25
#define GLOSS_VALUE_MULTIPLIER 512
#define EMISSIVE_CAP_DISTANCE 25

// Some drivers require the following
precision highp float;

struct BaseLight
{
    vec3  color;
    float ambientIntensity;
    float diffuseIntensity;

    float attenConstant;
    float attenLinear;
    float attenQuad;
};

struct PointLight
{
    BaseLight base;
    vec3 position;
};
struct SpotLight
{
    BaseLight base;
    vec3 position;
	//vec3 direction;
	//float cutoff;
};

layout (std140) uniform PointLights
{
	PointLight pointLights[MAX_NUM_POINT_LIGHTS];
};
layout (std140) uniform SpotLights
{
	SpotLight spotLights[MAX_NUM_SPOT_LIGHTS];
};

uniform vec3 cameraPos;
uniform vec2 screenSize;

uniform int numPointLights;
uniform int numSpotLights;

uniform sampler2D positionMap;
uniform sampler2D diffuseMap;
uniform sampler2D normalMap;
uniform sampler2D emissiveMap;

out vec4 fragColor;

vec3 worldPos;
vec3 normal;
float specularPower;
float specularIntensity;

vec4 calcLightInternal(BaseLight baseLight, vec3 lightDirection)
{
    vec4  ambientColor	= vec4(baseLight.color, 1.0) * baseLight.ambientIntensity;
    float diffuseFactor = dot(normal, -lightDirection);

    vec4 diffuseColor  = vec4(0.0, 0.0, 0.0, 0.0);
    vec4 specularColor = vec4(0.0, 0.0, 0.0, 0.0);

    if(diffuseFactor > 0) 
	{
        diffuseColor = vec4(baseLight.color, 1.0) * baseLight.diffuseIntensity * diffuseFactor;

        vec3 vertexToEye = normalize(cameraPos - worldPos);
        vec3 lightReflect = normalize(reflect(lightDirection, normal));
        float specularFactor = dot(vertexToEye, lightReflect);
        specularFactor = pow(specularFactor, specularPower);
        if (specularFactor > 0) 
		{
			specularColor = vec4(baseLight.color, 1.0) * specularIntensity * specularFactor;
        }
    }
    return (ambientColor + diffuseColor + specularColor);
}

vec4 calcPointLight(PointLight light)
{
    vec3 lightDirection = worldPos - light.position;
    float distance = length(lightDirection);
    lightDirection = normalize(lightDirection);

    vec4 lightColor = calcLightInternal(light.base, lightDirection);
	
	float attenuation =  light.base.attenConstant	+
		                 light.base.attenLinear		* distance +
			             light.base.attenQuad		* distance * distance;

    return lightColor / attenuation;
}
/*vec4 calcSpotLight(SpotLight light)
{
	vec3 lightToFragment = normalize(worldPos - light.position);
	float spotLightFactor = dot(lightToFragment, light.direction);

	if(spotLightFactor > light.cutoff)
	{
		vec3 lightDirection = worldPos - light.position;
		float distance = length(lightDirection);
		lightDirection = normalize(lightDirection);
		
		vec4 lightColor = calcLightInternal(light.base, lightDirection);
		
		float attenuation =  light.base.attenConstant	+
			                 light.base.attenLinear		* distance +
				             light.base.attenQuad		* distance * distance;

		vec4 spotLightColor = (lightColor / attenuation) * (1.0 - (1.0 - spotLightFactor) * 1.0 / (1.0 - light.cutoff));
		return spotLightColor;
	}
	else
	{
		return vec4(0.0, 0.0, 0.0, 0.0);
	}
}*/

vec2 calcTexCoord(void)
{
    return gl_FragCoord.xy / screenSize;
}

void main(void)
{
	vec2 texCoord = calcTexCoord();
	vec3 color = texture(diffuseMap, texCoord).xyz;
	vec4 lightColor = vec4(0.0, 0.0, 0.0, 0.0);
	vec4 normalAndGloss = texture(normalMap, texCoord).xyzw;

	worldPos = texture(positionMap, texCoord).xyz;
	normal = normalize(normalAndGloss.xyz);

	specularPower = normalAndGloss.w * GLOSS_VALUE_MULTIPLIER;
	specularIntensity = 20.0;
	
	float distanceToFragment = length(worldPos - cameraPos);
	vec4 emissiveColor = texture(emissiveMap, texCoord) * clamp((EMISSIVE_CAP_DISTANCE / distanceToFragment), 0.2, 0.8);

	for(int i = 0; i < numPointLights; i++)
	{
		lightColor += calcPointLight(pointLights[i]);
	}
	for(int i = 0; i < numSpotLights; i++)
	{
		//lightColor += calcSpotLight(spotLights[i]);
	}
	
	vec3 lightDirection = worldPos - cameraPos;
	float distance = length(lightDirection);

	fragColor = vec4(color, 1.0) * lightColor + emissiveColor;
}
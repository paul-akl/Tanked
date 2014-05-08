#version 330

#define MAX_NUM_POINT_LIGHTS 50
#define MAX_NUM_SPOT_LIGHTS 5
#define GLOSS_VALUE_MULTIPLIER 512
#define SPECULAR_EXPONENT 20
#define EMISSIVE_CAP_DISTANCE 25
#define MAX_LIGHT_MULTIPLIER 1
#define EMISSIVE_COLOR_DIVIDER 50

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
	vec3 direction;
	float cutoff;
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
//uniform sampler2D emissiveMap;

layout(location = 0) out vec4 emissiveBuffer;
layout(location = 1) out vec4 finalBuffer;

//out vec4 fragColor;

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
vec4 calcSpotLight(SpotLight light)
{
	// Calculate direction from position of light to current pixel
	vec3 lightToFragment = normalize(worldPos - light.position);

	// 
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
}

vec2 calcTexCoord(void)
{
    return gl_FragCoord.xy / screenSize;
}

void main(void)
{
	// Calculate screen-space texture coordinates, for buffer access
	vec2 texCoord = calcTexCoord();

	// Get diffuse color (full-bright) from diffuse buffer
	vec3 color = texture(diffuseMap, texCoord).xyz;

	vec4 lightColor = vec4(0.0, 0.0, 0.0, 0.0);
	vec4 finalColor = vec4(0.0, 0.0, 0.0, 0.0);
	vec4 emissiveColor = vec4(0.0, 0.0, 0.0, 0.0);

	// Gloss map value (specular power) is packed in 4th float of normal map, to save space
	vec4 normalAndGloss = texture(normalMap, texCoord).xyzw;

	// Get pixel's world position from position buffer
	worldPos = texture(positionMap, texCoord).xyz;

	// Get normal from normal and gloss buffer (vec3 only) and normalize it to minimize floating point approximation errors
	normal = normalize(normalAndGloss.xyz);

	// Calculate gloss map value. Specular intensity should ideally be set from specular map (vec3 color of specular reflection)
	specularPower = normalAndGloss.w * GLOSS_VALUE_MULTIPLIER;
	specularIntensity = SPECULAR_EXPONENT;

	// ------------------------------------------------------------------------------------------------------------------------------------
	//	Because of old GPU drivers in labs (ATI driver bug), shaders do no support dynamic for loops (that are controlled by a uniform.
	//	A costly fix (checking every light MAX_NUM_LIGHTS times) is due, it would be removed otherwise.
	// ------------------------------------------------------------------------------------------------------------------------------------

	for(int i = 0; i < MAX_NUM_POINT_LIGHTS; i++)
	{
		if(i < numPointLights)
		{
			// Calculate color of the light (ADS lighting)
			lightColor = calcPointLight(pointLights[i]);

			// Write color (that is brighter than a set treshold) to emissive buffer
			emissiveColor += clamp(lightColor / EMISSIVE_COLOR_DIVIDER, 0.0, 0.1);

			// Clamp the color of the light for diffuse light
			finalColor += clamp(lightColor, 0.0, MAX_LIGHT_MULTIPLIER);
		}
	}
	for(int i = 0; i < MAX_NUM_SPOT_LIGHTS; i++)
	{
		if(i < numSpotLights)
		{
			lightColor = calcSpotLight(spotLights[i]);
			emissiveColor += clamp(lightColor / EMISSIVE_COLOR_DIVIDER, 0.0, 0.1);
			finalColor += clamp(lightColor, 0.0, MAX_LIGHT_MULTIPLIER);
		}
	}

	// Emissive color is for bloom simulation, instead of having over-brightened pixels, due to a large number of lights,
	// light color is clamped (saturated) and over brightness of that color is written into emissive buffer, for bluring.
	emissiveBuffer += emissiveColor;

	// Multiply the diffuse color by the amound of light the current pixel recieves
	finalBuffer = vec4(color, 1.0) * finalColor;
}
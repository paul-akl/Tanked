#version 330 core

//layout(location = 0) out vec4 fragColor;
out vec4 fragColor;
                                                      
struct BaseLight
{
    vec3  color;
    float ambientIntensity;
    float diffuseIntensity;
};
struct Attenuation
{
    float constant;
    float linear;
    float exp;
};
struct SpotLight
{
    BaseLight base;
    vec3 position;
	vec3 direction;
    Attenuation atten;
	float cutoff;
};


uniform sampler2D positionMap;
uniform sampler2D diffuseMap;
uniform sampler2D normalMap;

uniform SpotLight light;

uniform float	specularIntensity;
uniform float	specularPower;

uniform vec3 cameraPos;
uniform vec2 screenSize;

vec4 calcLightInternal(BaseLight baseLight, vec3 lightDirection, vec3 worldPos, vec3 normal)
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

vec4 calcSpotLight(vec3 worldPos, vec3 normal)
{
	vec3 lightToFragment = normalize(worldPos - light.position);
	float spotLightFactor = dot(lightToFragment, light.direction);

	if(spotLightFactor > light.cutoff)
	{
		vec3 lightDirection = worldPos - light.position;
		float distance = length(lightDirection);
		lightDirection = normalize(lightDirection);
		
		vec4 lightColor = calcLightInternal(light.base, lightDirection, worldPos, normal);
		
		float attenuation =  light.atten.constant	+
			                 light.atten.linear	* distance +
				             light.atten.exp	* distance * distance;

		vec4 spotLightColor = (lightColor / attenuation) * (1.0 - (1.0 - spotLightFactor) * 1.0 / (1.0 - light.cutoff));

		vec4 shade1 = 	smoothstep(vec4(0.1),vec4(0.11),spotLightColor);
		vec4 shade2 = 	smoothstep(vec4(0.2),vec4(0.41),spotLightColor);
		vec4 shade3 = 	smoothstep(vec4(0.9),vec4(0.95),spotLightColor);

			
		//vec4 shade1 = 	step(vec4(0.1),spotLightColor);
		//vec4 shade2 = 	step(vec4(0.45),spotLightColor);
		//vec4 shade3 = 	step(vec4(0.8),spotLightColor);
		vec4 colour = 	max( max(0.3*shade1,0.4*shade2), shade3  );
		return colour;

	}
	else
	{
		return vec4(0.0, 0.0, 0.0, 0.0);
	}
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
	
	//fragColor = vec4(0.0, 1.0, 0.0, 1.0);
    fragColor = vec4(color, 1.0) * calcSpotLight(worldPos, normal);
}
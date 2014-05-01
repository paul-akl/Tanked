#version 330 core
precision highp float;

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
    float quad;
};
struct PointLight
{
    BaseLight base;
    vec3 position;
    Attenuation atten;
};

uniform sampler2D positionMap;
uniform sampler2D diffuseMap;
uniform sampler2D normalMap;

uniform vec3 cameraPos;
uniform vec2 screenSize;

uniform PointLight	light;

vec4 calcLightInternal(BaseLight baseLight, vec3 lightDirection, vec3 worldPos, vec3 normal, float specularPower, float specularIntensity)
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
			//specularColor = vec4(baseLight.color, 1.0) * baseLight.diffuseIntensity * specularFactor;
        }
    }

    return (ambientColor + diffuseColor + specularColor);
}
vec4 calcPointLight(vec3 worldPos, vec3 normal, float specularPower, float specularIntensity)
{
    vec3 lightDirection = worldPos - light.position;
    float distance = length(lightDirection);
    lightDirection = normalize(lightDirection);

    vec4 lightColor = calcLightInternal(light.base, lightDirection, worldPos, normal, specularPower, specularIntensity);

    float attenuation =  light.atten.constant	+
                         light.atten.linear	* distance +
                         light.atten.quad	* distance * distance;

    return lightColor / attenuation;
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

	float specularPower = texture(normalMap, texCoord).w * 500.0;
	float specularIntensity = 20.0;
	
	//float specularIntensity = texture(normalMap, texCoord).w * 50.0;
	//float specularPower = 500.0;

	// ==========================================
	// specularIntensity	= specular strength
	// specularPower		= specular exponent
	// ==========================================

	fragColor = vec4(color, 1.0) * calcPointLight(worldPos, normal, specularPower, specularIntensity);
	//fragColor = vec4(normal, 1.0);
	//fragColor = calcPointLight(worldPos, normal);
	//fragColor = vec4(normal, 1.0);
}
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
struct PointLight
{
    BaseLight base;
    vec3 position;
    Attenuation atten;
};

uniform sampler2D positionMap;
uniform sampler2D diffuseMap;
uniform sampler2D normalMap;

uniform PointLight		 pointLight;

uniform float	specularIntensity;
uniform float	specularPower;

uniform vec3 cameraPos;
uniform vec2 screenSize;

vec4 calcLightInternal(BaseLight light, vec3 lightDirection, vec3 worldPos, vec3 normal)
{
    vec4  ambientColor	= vec4(light.color, 1.0) * light.ambientIntensity;
    float diffuseFactor = dot(normal, -lightDirection);

    vec4 diffuseColor  = vec4(0.0, 0.0, 0.0, 0.0);
    vec4 specularColor = vec4(0.0, 0.0, 0.0, 0.0);

    if(diffuseFactor > 0) 
	{
        diffuseColor = vec4(light.color, 1.0) * light.diffuseIntensity * diffuseFactor;

        vec3 vertexToEye = normalize(cameraPos - worldPos);
        vec3 lightReflect = normalize(reflect(lightDirection, normal));
        float specularFactor = dot(vertexToEye, lightReflect);
        specularFactor = pow(specularFactor, specularPower);
        if (specularFactor > 0) 
		{
            specularColor = vec4(light.color, 1.0) * specularIntensity * specularFactor;
        }
    }

    return (ambientColor + diffuseColor + specularColor);
}
vec4 calcPointLight(vec3 worldPos, vec3 normal)
{
    vec3 lightDirection = worldPos - pointLight.position;
    float distance = length(lightDirection);
    lightDirection = normalize(lightDirection);

    vec4 lightColor = calcLightInternal(pointLight.base, lightDirection, worldPos, normal);

    float attenuation =  pointLight.atten.constant	+
                         pointLight.atten.linear	* distance +
                         pointLight.atten.exp		* distance * distance;

    //attenuation = min(1.0, attenuation);

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
	
    //vec3 vertexToEye = normalize(cameraPos - worldPos);
	//fragColor = vec4(vertexToEye, 1.0);
    fragColor = vec4(color, 1.0) * calcPointLight(worldPos, normal);
	//fragColor = vec4(1.0, 0.0, 0.0, 1.0);
	//fragColor = vec4(color, 1.0);
}
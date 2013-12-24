// Phong fragment shader phong-tex.frag matched with phong-tex.vert
#version 330

// Some drivers require the following
precision highp float;

struct lightStruct
{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
};

struct materialStruct
{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec4 emissive;
	float shininess;
};

uniform lightStruct light;
uniform materialStruct material;
uniform sampler2D textureUnit0;

in vec3 ex_N;
in vec3 ex_V;
in vec3 ex_L;
in vec2 ex_TexCoord;
layout(location = 0) out vec4 out_Color;
 
void main(void) {
    
	// Ambient intensity
	vec4 ambientI = light.ambient * material.ambient;

	// Diffuse intensity
	vec4 diffuseI = light.diffuse * material.diffuse * max(dot(ex_N,ex_L),0);

	// Specular intensity
	// Calculate R - reflection of light
	vec3 R = normalize(-reflect(ex_L,ex_N));
	vec4 specularI = light.specular * material.specular;
	specularI = specularI * pow(max(dot(R,ex_V),0), material.shininess);

	// Fragment colour
	if(material.emissive.w > 0.0)
		out_Color = material.emissive*texture2D(textureUnit0, ex_TexCoord);

	
	else
		out_Color = (ambientI + diffuseI + specularI) * texture2D(textureUnit0, ex_TexCoord);

	//out_Color = texture2D(textureUnit0, ex_TexCoord);
}
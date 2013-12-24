// phong-tex.vert
// Vertex shader for use with a Phong or other reflection model fragment shader
// Calculates and passes on V, L, N vectors for use in fragment shader, phong2.frag
#version 330

uniform mat4 modelview;
uniform mat4 projection;
uniform vec4 lightPosition;

in  vec3 in_Position;
in  vec3 in_Normal;
out vec3 ex_N;
out vec3 ex_V;
out vec3 ex_L;

in vec2 in_TexCoord;
out vec2 ex_TexCoord;

// multiply each vertex position by the MVP matrix
// and find V, L, N vectors for the fragment shader
void main(void) {

	// vertex into eye coordinates
	vec4 vertexPosition = modelview * vec4(in_Position,1.0);
    gl_Position = projection * vertexPosition;

	// Find V - in eye coordinates, eye is at (0,0,0)
	ex_V = normalize(-vertexPosition).xyz;

	// Vertex normal in eye coordinates
	ex_N = normalize(modelview * vec4(in_Normal,0.0)).xyz;

	// L - to light source from vertex
	ex_L = normalize(lightPosition.xyz - vertexPosition.xyz);

	ex_TexCoord = in_TexCoord;

}
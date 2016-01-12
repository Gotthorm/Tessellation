#version 420 core

// Per-vertex inputs
layout (location = 0) in vec4 position;
layout (location = 1) in vec3 normal;

// Matrix inputs
uniform mat4 model_matrix;
uniform mat4 projection_matrix;
uniform mat4 view_matrix;

// Light and material properties
uniform vec3 light_position = vec3(100.0, 100.0, 100.0);
uniform vec3 diffuse_albedo = vec3(0.2, 0.5, 0.2);
uniform vec3 specular_albedo = vec3(0.7);
uniform float specular_power = 128.0;
uniform vec3 ambient = vec3(0.1, 0.1, 0.1);

// Outputs to the fragment shader
out VS_OUT
{
	vec3 color;
} vs_out;

void main(void)
{
	// Calculate the view space position
	vec4 P = model_matrix * position;

	// Calculate the normal in view space
	vec3 N = mat3(model_matrix) * normal;

	// Calculate the light vector in view space
	vec3 L = light_position - P.xyz;

	// Calculate the view vector
	vec3 V = -P.xyz;

	// Normalize the vectors
	N = normalize(N);
	L = normalize(L);
	V = normalize(V);

	// Calculate R by reflecting -L around the plane defined by N
	vec3 R = reflect(-L, N);

	// Calculate the diffuse and specular contributions
	vec3 diffuse = max(dot(N, L), 0.0) * diffuse_albedo;
	vec3 specular = pow(max(dot(R, V), 0.0), specular_power) * specular_albedo;
	
	// Send the resulting color output to the fragment shader
	vs_out.color = ambient + diffuse + specular;

    // Calculate the position of each vertex
    gl_Position = projection_matrix * view_matrix * P;
}

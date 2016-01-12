#version 410 core

// Per-vertex inputs
layout (location = 0) in vec4 position;
layout (location = 1) in vec3 normal;

/*
// Matrices we'll need
layout (std140) uniform constants
{
    mat4 mv_matrix;
    mat4 view_matrix;
    mat4 proj_matrix;
};
*/

uniform mat4 model_matrix;
uniform mat4 projection_matrix;
uniform mat4 view_matrix;

// Inputs from vertex shader
out VS_OUT
{
    vec3 N;
    vec3 L;
    vec3 V;
} vs_out;

// Position of light
uniform vec3 light_pos = vec3(500.0, 500.0, 500.0);

void main(void)
{
    // Calculate view-space coordinate
    vec4 P = model_matrix * position;

    // Calculate normal in view-space
    vs_out.N = mat3(model_matrix) * normal;

    // Calculate light vector
    vs_out.L = light_pos - P.xyz;

    // Calculate view vector
    vs_out.V = -P.xyz;

    // Calculate the clip-space position of each vertex
    gl_Position = projection_matrix * view_matrix * P;
}

#version 420 core

// Output
layout (location = 0) out vec4 color;

// Input from the vertex shader
in VS_OUT
{
	vec3 color;
} fs_in;

void main(void)
{
    // Write final color to the framebuffer
    color = vec4(fs_in.color, 1.0);
}

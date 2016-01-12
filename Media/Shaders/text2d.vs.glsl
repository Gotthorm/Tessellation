#version 450 core

layout (location = 1) in float height;

// "color" is an input vertex attribute
layout (location = 2) in vec4 color;

// "vs_color" is an output that will be sent to the next shader stage
out vec4 vs_color;

void main(void)
{
	const vec4 vertices[] = vec4[](	vec4(-1.0, height, 0.0, 1.0),
									vec4( 1.0, height, 0.0, 1.0),
									vec4(-1.0,    1.0, 0.0, 1.0),
									vec4( 1.0,    1.0, 0.0, 1.0));

    gl_Position = vertices[gl_VertexID];
	
	//gl_Position = vec4(float((gl_VertexID >> 1) & 1) * 2.0 - 1.0, float((gl_VertexID & 1)) * 2.0 - 1.0, 0.0, 1.0);

	vs_color = color;
}

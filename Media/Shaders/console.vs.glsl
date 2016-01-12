#version 450 core

layout (location = 1) in float height;

void main(void)
{
	const vec4 vertices[] = vec4[](	vec4(-1.0, height, 0.0, 1.0),
									vec4( 1.0, height, 0.0, 1.0),
									vec4(-1.0,  1.0, 0.0, 1.0),
									vec4( 1.0,  1.0, 0.0, 1.0));

    gl_Position = vertices[gl_VertexID];
}

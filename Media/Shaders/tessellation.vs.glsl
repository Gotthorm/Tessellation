#version 420 core

// This is hard coded for a 64X64 grid of 16x16 quads
// The grid will be centered on the origin

out VS_OUT
{
    vec2 tc;
} vs_out;

void main(void)
{
    const vec4 vertices[] = vec4[](vec4( 8.0, -8.0,  0.0, 1.0 ),
                                   vec4(-8.0, -8.0,  0.0, 1.0 ),
                                   vec4( 8.0,  8.0,  0.0, 1.0 ),
                                   vec4(-8.0,  8.0,  0.0, 1.0 ));

    int x = gl_InstanceID & 63;
    int y = gl_InstanceID >> 6;
	int spacing = 16 * 32 - 8;

	vec2 offs = vec2(x * 16, y * 16);
    vs_out.tc = (vertices[gl_VertexID].xy + offs + vec2(8.0)) / 1024.0;

    gl_Position = vertices[gl_VertexID] + vec4(float(x * 16 - spacing), float(y * 16 - spacing), 0.0, 0.0);
}

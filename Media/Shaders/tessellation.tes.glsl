#version 420 core

layout (quads) in;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;

void main(void)
{
	vec4 poop1 = mix(gl_in[0].gl_Position, gl_in[1].gl_Position, gl_TessCoord.x);
	vec4 poop2 = mix(gl_in[2].gl_Position, gl_in[3].gl_Position, gl_TessCoord.x);
	vec4 poop3 = mix(poop1, poop2, gl_TessCoord.y);

	gl_Position = proj_matrix * mv_matrix * poop3;
}
#version 420 core

layout (quads) in;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

void main(void)
{
	vec4 poop1 = mix(gl_in[0].gl_Position, gl_in[1].gl_Position, gl_TessCoord.x);
	vec4 poop2 = mix(gl_in[2].gl_Position, gl_in[3].gl_Position, gl_TessCoord.x);
	vec4 poop3 = mix(poop1, poop2, gl_TessCoord.y);

	gl_Position = projection_matrix * (view_matrix * (model_matrix * poop3));
}
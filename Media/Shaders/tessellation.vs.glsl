#version 420 core
 
void main(void)
{
    const vec4 vertices[] = vec4[](vec4( 0.5, -0.5,  0.0, 1.0),
                                   vec4(-0.5, -0.5,  0.0, 1.0),
                                   vec4( 0.5,  0.5,  0.0, 1.0),
                                   vec4(-0.5,  0.5,  0.0, 1.0));

    int x = gl_InstanceID & 7;
    int y = gl_InstanceID >> 3;

    gl_Position = vertices[gl_VertexID] + vec4(float(x - 4), float(y - 4), 0.0, 0.0);
}

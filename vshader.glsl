#version 330

uniform mat4 mvp_matrix;

in vec4 a_position;
in vec2 a_texcoord;
out vec2 v_texcoord;
//! [0]
void main()
{
    gl_Position = mvp_matrix * a_position;
    if(a_texcoord.x == 0. && a_texcoord.y == 0.)
        v_texcoord = vec2(0.,0.);
    else v_texcoord = a_texcoord;
}
//! [0]

#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform mat4 mvp_matrix;

attribute vec4 a_position;

varying float h;

//! [0]
void main()
{    
    // Calculate vertex position in screen space
    gl_Position = mvp_matrix * vec4(a_position.xyz, 1.0f);
    h = a_position.y;
    gl_PointSize = 10.0f;
}
//! [0]

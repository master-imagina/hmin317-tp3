#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif


uniform mat4 mvp_matrix;
uniform sampler2D height_map;
uniform float map_size;

attribute vec4 a_position;
attribute vec4 a_color;

varying vec4 v_color;
varying float render;

//! [0]
void main()
{
    // Calculate vertex position in screen space
    float h = texture2D(height_map, a_position.xz / map_size).r;
    render = a_position.y < (h * 5.0f)  ? 0.0f : 1.0f;
    gl_Position = mvp_matrix * vec4(a_position.xyz, 1.0f);
    gl_PointSize = a_position.w;
    v_color = a_color;
}

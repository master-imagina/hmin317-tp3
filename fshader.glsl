#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform sampler2D texture;
uniform vec3 v_color;

varying vec2 v_texcoord;

//! [0]
void main()
{
    // Set fragment color from texture
    gl_FragColor = texture2D(texture, v_texcoord) * vec4(v_color, 1.0) * 0.005;
}
//! [0]


#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform sampler2D texture;
uniform float r;
uniform float g;
uniform float b;

varying vec2 v_texcoord;

//! [0]
void main()
{
    // Set fragment color from texture
    gl_FragColor = texture2D(texture, v_texcoord)* vec4(r,g,b,1.0);
}
//! [0]


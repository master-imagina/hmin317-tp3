#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

varying vec4 v_color;
varying float render;

uniform vec3 lightColor;

//! [0]
void main()
{
    if(render > 0.9f) {
        gl_FragColor = v_color;
    } else {
        discard;
    }
}
//! [0]

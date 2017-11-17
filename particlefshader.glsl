#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

// Interpolated values from the vertex shaders

varying vec4 v_color;
varying float render;

uniform vec3 lightColor;

void main(){
    // Output color = color of the texture at the specified UV
    if(render > 0.9f) {
        gl_FragColor = v_color;
    } else {
        discard;
    }

}

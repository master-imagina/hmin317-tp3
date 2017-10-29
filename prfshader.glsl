#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

varying vec4 v_color;

uniform vec3 lightColor;

//! [0]
void main()
{
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
    vec4 result = vec4(v_color.xyz * ambient, v_color.w);
    gl_FragColor = result;
}
//! [0]


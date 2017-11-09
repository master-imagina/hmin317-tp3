#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform sampler2D sand;
uniform sampler2D rock;
uniform vec3 lightColor;

varying vec2 v_texcoord;
varying float h;
varying vec3 normal;
varying vec3 lightVector;

//! [0]
void main()
{
    float rockH = 0.3f;
    float sandH = 0.20f;
    // Set fragment color from texture
    float diffuse = max(dot(normal, lightVector), 0.0);
    if(h > rockH) {
        gl_FragColor = texture2D(rock, v_texcoord * 5) * diffuse;
    } else if (h > sandH && h < rockH ){
        gl_FragColor = mix(texture2D(sand, v_texcoord * 5),texture2D(rock, v_texcoord * 5), (h - sandH) / (rockH - sandH)) * diffuse;
    } else {
        gl_FragColor = texture2D(sand, v_texcoord * 5) * diffuse;
    }
    //debug normal
   // gl_FragColor = vec4(normal, 1.0);
}
//! [0]


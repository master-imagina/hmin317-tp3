#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

varying vec4 colorP;
//! [0]
void main()
{
    gl_FragColor = colorP;
}
//! [0]


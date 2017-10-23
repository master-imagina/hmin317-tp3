#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

varying float h;

//! [0]
void main()
{
    if(h == 0.0f) {
        gl_FragColor = vec4(1.0f, 0.0f, 1.0f, 1.0f);
    } else {
        gl_FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    }
}
//! [0]


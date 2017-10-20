#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform mat4 mvp_matrix;
uniform vec3 camera_up, camera_right;
attribute vec3 squareVertices;
attribute vec4 xyzs, color;

varying vec4 colorP;
//! [0]
void main()
{
    float particleSize = xyzs.w;
    vec3 particleCenter = xyzs.xyz;
    vec3 vertexPosition_worldSpace = particleCenter
            + camera_right * squareVertices.x * particleSize
            + camera_up * squareVertices.y * particleSize;
    // Calculate vertex position in screen space
    gl_Position = mvp_matrix * vec4(vertexPosition_worldSpace, 1.0f);
    colorP = color;
}
//! [0]

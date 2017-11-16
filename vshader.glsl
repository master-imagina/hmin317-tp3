#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform mat4 mvp_matrix;
uniform sampler2D height_map;
uniform float sizeV;

attribute vec4 a_position;
attribute vec2 a_texcoord;

varying vec2 v_texcoord;
varying float h;
varying vec3 normal;
varying vec3 lightVector;

const vec3 sun = vec3(-30.0, 100.0, 10.0);

//! [0]
void main()
{
    vec4 a_height = vec4(a_position.x, (texture2D(height_map, a_texcoord).r) * 5.0, a_position.z, 1.0);
    // Calculate vertex position in screen space

    //calcul normal
    float up = texture2D(height_map, a_texcoord - vec2(0.0, 1.0 / sizeV)).r * 5.0;
    float down = texture2D(height_map, a_texcoord + vec2(0.0, 1.0 / sizeV)).r * 5.0;
    float left = texture2D(height_map, a_texcoord - vec2(1.0 / sizeV, 0.0)).r * 5.0;
    float right = texture2D(height_map, a_texcoord + vec2(1.0 / sizeV, 0.0)).r * 5.0;
    normal.x = left - right;
    normal.y = 2.0;
    normal.z = down - up;
    normal = normalize(normal);
    lightVector = normalize(sun - a_height.xyz);
    gl_Position = mvp_matrix * a_height;
    // Pass texture coordinate to fragment shader
    // Value will be automatically interpolated to fragments inside polygon faces
    v_texcoord = a_texcoord;
    h = a_height.y;

}
//! [0]

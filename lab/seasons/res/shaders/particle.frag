#version 330

// http://www.geeks3d.com/20140815/particle-billboarding-with-the-geometry-shader-glsl/


in vec2 vertexUV;

uniform vec4 particleColor;

out vec4 fragColor;


void main()
{
    vec2 uv = vertexUV.xy;
    uv.y *= -1.0;

    fragColor = particleColor;
}

#version 330

// http://www.geeks3d.com/20140815/particle-billboarding-with-the-geometry-shader-glsl/


in vec2 vertexUV;

uniform float textureFlag;
uniform vec4 particleColor;
uniform sampler2D particleTexture;

out vec4 fragColor;


void main()
{
    vec2 uv = vertexUV.xy;
    uv.y *= -1.0;

    // Pretty bad, don't do that
    if (textureFlag == 0.f) {
        fragColor = particleColor;
    }
    else {
        fragColor = texture2D(particleTexture, uv);
    }
}

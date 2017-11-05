#version 330

// http://www.geeks3d.com/20140815/particle-billboarding-with-the-geometry-shader-glsl/


in vec3 vertexPos;

uniform mat4 viewMatrix;


void main()
{
    gl_Position = viewMatrix * vec4(vertexPos, 1.0f);
}

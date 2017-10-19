#version 330

in vec3 particleWorldPos;

uniform mat4 viewMatrix;


void main()
{
    gl_Position = viewMatrix * vec4(particleWorldPos, 1.0f);
}

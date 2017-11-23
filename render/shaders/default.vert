#version 330

uniform mat4 modelMatrix;
uniform mat4 worldMatrix;

in vec3 vertexPos;
in vec3 normal;
in vec2 texCoords;


void main()
{
    gl_Position = worldMatrix * modelMatrix * vec4(vertexPos, 1.0);
}

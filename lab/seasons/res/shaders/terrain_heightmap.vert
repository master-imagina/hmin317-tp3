#version 330

uniform mat4 worldMatrix;

in vec3 vertexPos;

out float vertexHeight;
out vec3 outVertexPos;

void main()
{
    gl_Position = worldMatrix * vec4(vertexPos, 1.0);

    vertexHeight = vertexPos.y;
    outVertexPos = vertexPos;
}

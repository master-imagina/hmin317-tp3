#version 330

uniform mat4 modelMatrix;
uniform mat4 worldMatrix;

in vec3 vertexPos;

out float vertexHeight;
out vec3 outVertexPos;
out vec2 texCoord;

void main()
{
    gl_Position = worldMatrix * modelMatrix * vec4(vertexPos, 1.0);
    texCoord = vec2(mod(gl_VertexID, 256), floor(gl_VertexID / 256.f)) / 4.f;

    vertexHeight = vertexPos.y;
    outVertexPos = vertexPos;
}

#version 330

// Based on https://learnopengl.com/code_viewer_gh.php?code=src/2.lighting/3.1.materials/3.1.materials.vs


uniform mat4 modelMatrix;
uniform mat4 worldMatrix;

in vec3 vertexPos;
in vec3 normal;
in vec2 texCoords;

out vec3 worldPos;
out vec3 worldNormal;


void main()
{
    worldPos = vec3(modelMatrix * vec4(vertexPos, 1.0));
    worldNormal = mat3(transpose(inverse(modelMatrix))) * normal;

    gl_Position = worldMatrix * modelMatrix * vec4(vertexPos, 1.0);
}

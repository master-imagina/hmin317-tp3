#version 330

uniform float minHeight;            // Terrain minimum height
uniform float maxHeight;            // Terrain maximum height
uniform vec4 terrainColor;          // Terrain color

in float vertexHeight;

out vec4 fragColor;


void main()
{
    float a = 255. / maxHeight;
    float b = (255. * minHeight) / (maxHeight - minHeight);

    fragColor = floor(a * vertexHeight * terrainColor + b) / 255.;
}

#version 330

uniform float minHeight;            // Terrain minimum height
uniform float maxHeight;            // Terrain maximum height
uniform vec4 terrainColor;          // Terrain color

in float vertexHeight;

out vec4 fragColor;
uniform sampler2D tex;
in vec2 texCoord;

void main()
{
    float a = 255. / maxHeight;
    float b = (255. * minHeight) / (maxHeight - minHeight);

    fragColor = /*texture2D(tex,texCoord) */floor(a * vertexHeight * terrainColor + b) / 255.;
}

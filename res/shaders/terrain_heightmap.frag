#version 330

uniform float minHeight;            // Terrain minimum height
uniform float maxHeight;            // Terrain maximum height

in float vertexHeight;

out vec4 fragColor;


void main()
{
    float a = 255. / maxHeight;
    float b = (255. * minHeight) / (maxHeight - minHeight);

    float grayscaleValue = floor(a * vertexHeight + b) / 255.;

    fragColor = vec4(grayscaleValue, grayscaleValue, grayscaleValue, 1.0);
}

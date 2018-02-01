#version 330

// http://madebyevan.com/shaders/grid/
// License: CC0 (http://creativecommons.org/publicdomain/zero/1.0/)


in vec3 outVertexPos;

out vec4 fragColor;


void main()
{
      // Pick a coordinate to visualize in a grid
      vec2 coord = outVertexPos.xz;

      // Compute anti-aliased world-space grid lines
      vec2 grid = abs(fract(coord - 0.5) - 0.5) / fwidth(coord);
      float line = min(grid.x, grid.y);

      // Just visualize the grid lines directly
      fragColor = vec4(vec3(1.0 - min(line, 1.0)), 1.0);
}

#version 330

// http://www.geeks3d.com/20140815/particle-billboarding-with-the-geometry-shader-glsl/


layout (points) in;
layout (triangle_strip) out;
layout (max_vertices = 4) out;

uniform float particlesSize;
uniform mat4 projectionMatrix;

out vec2 vertexUV;


void main()
{
    vec4 particlePos = gl_in[0].gl_Position;

    // left-bottom
    vec2 leftBottomXY = particlePos.xy + vec2(-0.5, -0.5) * particlesSize;
    gl_Position = projectionMatrix * vec4(leftBottomXY, particlePos.zw);
    vertexUV = vec2(0.0, 0.0);
    EmitVertex();

    // left-top
    vec2 leftTopXY = particlePos.xy + vec2(-0.5, 0.5) * particlesSize;
    gl_Position = projectionMatrix * vec4(leftTopXY, particlePos.zw);
    vertexUV = vec2(0.0, 1.0);
    EmitVertex();

    // right-bottom
    vec2 rightBottomXY = particlePos.xy + vec2(0.5, -0.5) * particlesSize;
    gl_Position = projectionMatrix * vec4(rightBottomXY, particlePos.zw);
    vertexUV = vec2(1.0, 0.0);
    EmitVertex();

    // right-top
    vec2 rightTopXY = particlePos.xy + vec2(0.5, 0.5) * particlesSize;
    gl_Position = projectionMatrix * vec4(rightTopXY, particlePos.zw);
    vertexUV = vec2(1.0, 1.0);
    EmitVertex();


    EndPrimitive();
}

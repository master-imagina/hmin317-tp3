#version 330

layout (points) in;
layout (triangle_strip) out;
layout (max_vertices = 4) out;

uniform mat4 projMatrix;

out vec2 vertexUV;


void main()
{
    float particleSize = 4;

    vec4 P = gl_in[0].gl_Position;

    // a: left-bottom
    vec2 va = P.xy + vec2(-0.5, -0.5) * particleSize;
    gl_Position = projMatrix * vec4(va, P.zw);
    vertexUV = vec2(0.0, 0.0);
    EmitVertex();

    // b: left-top
    vec2 vb = P.xy + vec2(-0.5, 0.5) * particleSize;
    gl_Position = projMatrix * vec4(vb, P.zw);
    vertexUV = vec2(0.0, 1.0);
    EmitVertex();

    // d: right-bottom
    vec2 vd = P.xy + vec2(0.5, -0.5) * particleSize;
    gl_Position = projMatrix * vec4(vd, P.zw);
    vertexUV = vec2(1.0, 0.0);
    EmitVertex();

    // c: right-top
    vec2 vc = P.xy + vec2(0.5, 0.5) * particleSize;
    gl_Position = projMatrix * vec4(vc, P.zw);
    vertexUV = vec2(1.0, 1.0);
    EmitVertex();

    EndPrimitive();
}

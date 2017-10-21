#version 330 core

layout(points) in;
layout (triangle_strip, max_vertices=4) out;

in float mustDiscard[];

out float mustDiscard_out;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

const vec4 offset [4] =
  vec4 [] ( vec4 (-0.5,  0.5, 0.0, 1.0),
            vec4 ( 0.5,  0.5, 0.0, 1.0),
            vec4 ( 0.5, -0.5, 0.0, 1.0),
            vec4 (-0.5, -0.5, 0.0, 1.0) );

void main()
{
    mat4 modelView = viewMatrix;
    modelView[0][0] = 1.0;
    modelView[0][1] = 0.0;
    modelView[0][2] = 0.0;



    // Thrid colunm.
    modelView[2][0] = 0.0;
    modelView[2][1] = 0.0;
    modelView[2][2] = 1.0;

    gl_Position = projectionMatrix * modelView * (gl_in[0].gl_Position + offset[0]);
    mustDiscard_out = mustDiscard[0];
    EmitVertex();

    gl_Position = projectionMatrix * modelView * (gl_in[0].gl_Position + offset[1]);
    mustDiscard_out = mustDiscard[0];
    EmitVertex();

    gl_Position = projectionMatrix * modelView * (gl_in[0].gl_Position + offset[3]);
    mustDiscard_out = mustDiscard[0];
    EmitVertex();

    gl_Position = projectionMatrix * modelView * (gl_in[0].gl_Position + offset[2]);
    mustDiscard_out = mustDiscard[0];
    EmitVertex();


    EndPrimitive();
}

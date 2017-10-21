#version 330 core

in vec2 index;

out float mustDiscard;

uniform mat4 mvp;

uniform sampler2D particuleTex;

void main()
{
    vec3 position = texture2D(particuleTex,index).rgb;
    mustDiscard = position.g;
    position.x *= 200.0;
    position.y *= 40.0;
    position.z *= 200.0;

    gl_Position = mvp * vec4(position,1.0);

}

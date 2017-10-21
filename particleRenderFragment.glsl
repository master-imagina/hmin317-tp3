#version 330 core

in float mustDiscard;

out vec4 FragColor;

uniform sampler2D snowFlake;
uniform sampler2D rain;

uniform int calendar;

void main()
{
    float alpha = 0;
    if(calendar>280)
        alpha = texture2D(rain, gl_PointCoord).r;
    else
        alpha = texture2D(snowFlake, gl_PointCoord).r;

    if(mustDiscard > 0.9 || alpha<0.5)
        discard;
    vec3 color = vec3(1.0);
    if(calendar>280)
        color = vec3(0.1,0.56,0.67);
    FragColor =vec4(color,1.0);
}

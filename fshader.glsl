#version 330


uniform sampler2D texture;
uniform int season;


in vec2 v_texcoord;
void main()
{


    if(season == 1)
        gl_FragColor = texture2D(texture, v_texcoord)*vec4(0,1,0,0);
    else if(season == 2)
        if(v_texcoord.x == 0. && v_texcoord.y == 0.)
            gl_FragColor = vec4(0.13,0.7,0.69,0.);
        else
            gl_FragColor = texture2D(texture, v_texcoord)*vec4(0.957, 0.643, 0.376,0.);
    else if(season == 3)
        if(v_texcoord.x == 0. && v_texcoord.y == 0.)
            gl_FragColor = vec4(1,1,1,0);
        else gl_FragColor = texture2D(texture, v_texcoord)*vec4(1,1,1,0);
    else if(season == 4)
        gl_FragColor = texture2D(texture, v_texcoord)*vec4(0,0.5,0,0);

}


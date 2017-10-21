#version 330 core

in vec2 textureCoord;

uniform sampler2D particlesTex;
uniform sampler2D extraTex;
uniform sampler2D heightMap;
uniform vec3 gravityVector;
uniform float speed;
uniform vec3 windVector;
uniform float randomParameter;
uniform float particlesCount;
uniform float particlesFactor;
uniform int calendar;

uniform vec2 resolution;

layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec3 extraMap;

float random (in vec2 st) {
    return fract(sin(dot(st.xy,
                         vec2(12.9898,78.233)))*
        43758.5453123*randomParameter);
}


void main(){

    vec2 uv = gl_FragCoord.xy/resolution.xy;


    vec4 particule = texture2D(particlesTex,textureCoord);
    vec3 extraData = texture2D(extraTex,textureCoord).rgb;
    float height = texture2D(heightMap,vec2(particule.x,particule.z)).r;

    vec4 outParticule = particule;
    float randomValue = random(uv);
    if(particule.g <= height+(0.1/4.0) || outParticule.a==1.0){
        //random position
        outParticule = vec4( uv.x + uv.x/particlesCount - randomValue/particlesCount,1.0, uv.y + uv.y/particlesCount - randomValue/particlesCount,1.0);
        outParticule.a = (max(random(uv/0.9),0.0)+(1-particlesFactor)>0.5)?1.0:0.0;
    }else{
        //outParticule.rgb = particule.rgb ;// * speed + windVector;
        outParticule.r +=  max(sin(outParticule.g*40)/(400.0)-random(uv),0.0);
        if(outParticule.a==0.0){
            if(calendar>280)
                outParticule.g -= (randomValue+0.8)/40.0;
            else
                outParticule.g -= (randomValue+0.4)/150.0;
        }
    }

    FragColor = outParticule;
    extraMap = extraData;
}

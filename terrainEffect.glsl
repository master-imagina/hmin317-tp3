#version 330 core

in vec2 textureCoord;

uniform sampler2D particlesTex;
uniform sampler2D snowMap;
uniform sampler2D heightMap;

uniform int calendar;

layout(location = 0) out vec3 FragColor;

uniform vec2 resolution = vec2(256.0,256.0);

void main(){

    vec2 uv = gl_FragCoord.xy/resolution.xy;


    vec3 particule = texture2D(particlesTex,textureCoord).rgb;
    float height = texture2D(heightMap,vec2(particule.x,particule.z)).r;
    float snow =  texture2D(snowMap,uv).r;

    float left = texture2D(heightMap,vec2(particule.x,particule.z) - vec2(1.0/256.0,0)).r;
    float right = texture2D(heightMap,vec2(particule.x,particule.z) + vec2(1.0/256.0,0)).r;
    float up = texture2D(heightMap,vec2(particule.x,particule.z) + vec2(0,1.0/256.0)).r;
    float down = texture2D(heightMap,vec2(particule.x,particule.z) - vec2(0,1.0/256.0)).r;

    float upperLeft = texture2D(heightMap,vec2(particule.x,particule.z) + vec2(-1.0/256.0,1.0/256.0)).r;
    float upperRight = texture2D(heightMap,vec2(particule.x,particule.z) + vec2(1.0/256,1.0/256)).r;
    float downLeft = texture2D(heightMap,vec2(particule.x,particule.z) + vec2(-1.0/256,-1.0/256.0)).r;
    float downRight = texture2D(heightMap,vec2(particule.x,particule.z) + vec2(1.0/256,-1.0/256.0)).r;

    float leftWater = texture2D(snowMap,uv - vec2(1.0/256.0,0)).r;
    float rightWater = texture2D(snowMap,uv + vec2(1.0/256.0,0)).r;
    float upWater = texture2D(snowMap,uv + vec2(0,1.0/256.0)).r;
    float downWater = texture2D(snowMap,uv - vec2(0,1.0/256.0)).r;

    float upperLeftWater = texture2D(snowMap,uv + vec2(-1.0/256.0,1.0/256.0)).r;
    float upperRightWater = texture2D(snowMap,uv + vec2(1.0/256,1.0/256)).r;
    float downLeftWater = texture2D(snowMap,uv + vec2(-1.0/256,-1.0/256.0)).r;
    float downRightWater = texture2D(snowMap,uv + vec2(1.0/256,-1.0/256.0)).r;

    if(particule.x+0.005 >= uv.x && particule.x-0.005 <= uv.x && particule.z+0.005 >= uv.y && particule.z-0.005 <= uv.y && particule.y <= height+(0.1/4.0) ){
        if(calendar<170)
            snow += 0.8;
        else if(calendar>270){
            snow += 1.0;
        }


    }
    else{
         snow -= 0.02;
    }
    if(calendar>270){
                float base_keep = 0.80;
                float neigh =0.0;
                float keep=base_keep;
                float neigh_test =8.0;
                float snowheight = 80.0;
                if(left+leftWater/snowheight<height){
                    keep -= base_keep/neigh_test;
                    neigh++;
                }
                if(right+rightWater/snowheight<height){
                    keep -= base_keep/neigh_test;
                    neigh++;
                }
                if(up+upWater/snowheight<height){
                    keep -= base_keep/neigh_test;
                    neigh++;
                }
                if(down+downWater/snowheight<height){
                    keep -= base_keep/neigh_test;
                    neigh++;
                }
                if(upperRight+upperRightWater/snowheight <height){
                    keep -= base_keep/neigh_test;
                    neigh++;
                }
                if(upperLeft+upperLeftWater/snowheight <height){
                    keep -= base_keep/neigh_test;
                    neigh++;
                }
                if(downLeft+downLeftWater/snowheight<height){
                    keep -= base_keep/neigh_test;
                    neigh++;
                }
                if((downRight+downRightWater/snowheight)<height){
                    keep -= base_keep/neigh_test;
                    neigh++;
                }

                snow *= keep;
                keep += 0.05;

                if(left+leftWater/snowheight>=height){
                    snow += leftWater * (base_keep-keep*neigh);
                }
                if(right+rightWater/snowheight>=height){
                    snow += rightWater * (base_keep-keep*neigh);;
                }
                if(up+upWater/snowheight>=height){
                    snow += upWater * (base_keep-keep*neigh);
                }
                if(down+downWater/snowheight>=height){
                    snow += downWater * (base_keep-keep*neigh);
                }
                if(upperRight+upperRightWater/snowheight>=height){
                    snow += upperRightWater * (base_keep-keep*neigh);
                }
                if(upperLeft+upperLeftWater/snowheight>=height){
                    snow += upperLeftWater * (base_keep-keep*neigh);;
                }
                if(downRight+downRightWater/snowheight>=height){
                    snow += downRightWater * (base_keep-keep*neigh);
                }
                if(downLeft+downLeftWater/snowheight>=height){
                    snow += downLeftWater * (base_keep-keep*neigh);
                }
                //snow = neigh/8.0;
            }
    if(calendar>=358){
        snow=0.0;
    }
    FragColor = vec3(snow);

}

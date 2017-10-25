#include "firstpersoncamera.h"
#include <iostream>
#include <math.h>


FirstPersonCamera::FirstPersonCamera()
{
    position.x=10;position.y=10;position.z=10;
    lookAt.x=0;lookAt.y=0;lookAt.z=0;
    pitch=20;

}


void FirstPersonCamera::move(float dx, float dy, int z, int s, int q, int d,int dHeigh){

    pitch += dy * 0.3f;
    yaw += dx * 0.3f;

    if(pitch > 89.0)
         pitch = 89.0;
    else if(pitch < -89.0)
        pitch = -89.0;


    float phiRadian = pitch * M_PI / 180.0 ;
    float thetaRadian = yaw * M_PI / 180.0 ;

    lookAt.x = cos(phiRadian) * -sin(thetaRadian);

    lookAt.y = -sin(phiRadian);

    lookAt.z = cos(phiRadian) * cos(thetaRadian);

    glm::vec3 right;
    right.x = sin(thetaRadian- 3.14f/2.0f);
    right.y = 0;
    right.z = -cos(thetaRadian- 3.14f/2.0f);

    if(z){

        position.x += lookAt.x *1.5f;
        position.y += lookAt.y *1.5f ;
        position.z += lookAt.z *1.5f ;

    }
    if(s){

        position.x -= lookAt.x *1.5f;
        position.y -= lookAt.y *1.5f ;
        position.z -= lookAt.z *1.5f ;

    }
    if(q){
        position.x -= right.x *1.5f;

        position.z -= right.z *1.5f ;
    }
    if(d){

        position.x += right.x *1.5f;

        position.z += right.z *1.5f ;

    }


    position.y += 0.7f*dHeigh;



}



QMatrix4x4 FirstPersonCamera::getViewMatrix(){
    viewMatrix.setToIdentity();
    viewMatrix.lookAt(QVector3D(position.x,position.y,position.z),QVector3D(position.x+lookAt.x,position.y+lookAt.y,position.z+lookAt.z),QVector3D(0,1,0));

    return viewMatrix;
}


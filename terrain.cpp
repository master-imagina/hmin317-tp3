#include "terrain.h"

Terrain::Terrain(){
}

Terrain::Terrain(string path, float size_x, float size_z, float height_max){
    Load(path,size_x,size_z,height_max);
}

void Terrain::Load(string path, float size_x, float size_z, float height_max){

    this->path = path;
    this->size_x = size_x;
    this->size_z = size_z;
    this->height_max = height_max;

    QImage hmap(path.c_str());

    float w =  hmap.width();
    float h =  hmap.height();

    std::vector<VertexData3D> v;

    for(int i=0;i<w;i++){
        for(int j=0;j<h;j++){
            float x = i * size_x/w - size_x/2;
            float y = (float)(hmap.pixel(i,j) & 0xff) * height_max / 255.0;
            float z = j * size_z/h - size_z/2;
            float tx = (float)i / (float)(w-1);
            float ty = (float)(h-1-j) / (float)(h-1);

            v.push_back({QVector3D(x,y,z), QVector2D(tx, ty)});
        }
    }

    std::vector<GLuint> f;

    for(int i=0;i<w-1;i++){
        f.push_back( i*h );
        for(int j=0;j<h-1;j++){
            f.push_back( i*h+j );
            f.push_back( (i+1)*h+j );
            f.push_back( i*h+(j+1) );
            f.push_back( (i+1)*h+(j+1) );
        }
        f.push_back( (i+1)*h+(h-1) );
    }

    vbo_terrain.mode = GL_TRIANGLE_STRIP;
    vbo_terrain.indices = f;
    vbo_terrain.vertices = v;
}

void Terrain::Draw(){
    Draw_3D::DrawVBO(vbo_terrain);
}

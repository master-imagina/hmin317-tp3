#ifndef SHAPES_H
#define SHAPES_H

#include "../gameobject/geometry.h"

#include <QImage>

// Create a cube in the geometry mesh
void GeometryCube(Geometry * component);

// Create a plane in the geometry mesh
void GeometryPlane(Geometry * component);

// Create a plane in the geometry mesh
void GeometryUIPlane(Geometry * component);

// Create a terrain by a heightmap in the geometry mesh
void GeometryTerrain(Geometry * component, const QImage& heightmap);

#endif // SHAPES_H

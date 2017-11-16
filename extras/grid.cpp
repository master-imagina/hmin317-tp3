#include "grid.h"

#include <cmath>


Geometry grid(int size, float height)
{
    if (size == 0) {
        return Geometry();
    }

    Geometry ret;

    // Helpers for computing vertices
    const std::size_t vertexCount = size * size;
    ret.vertices.resize(vertexCount);

    // Helpers for computing indices
    const std::size_t maxXZCoord = size - 1;

    const std::size_t indexCount = 3 * 2 * std::pow(maxXZCoord, 2);
    ret.indices.resize(indexCount);

    int indexCounter = 0;

    // Compute...
    for (int z = 0; z < size; z++) {
        for (int x = 0; x < size; x++) {
            // ...vertices
            const int vertexIndex = z * size + x;

            const float xCoord = vertexIndex % size;
            const float zCoord = std::floor(vertexIndex / static_cast<float>(size));

            QVector3D &vertex = ret.vertices.at(vertexIndex);
            vertex = {xCoord, height, zCoord};

            // ... and indices
            if (x == maxXZCoord || z == maxXZCoord) {
                continue;
            }

            const uint32 firstSharedIndex = (z + 1) * size + x;
            const uint32 secondSharedIndex = z * size + (x + 1);

            ret.indices[indexCounter++] = vertexIndex;
            ret.indices[indexCounter++] = firstSharedIndex;
            ret.indices[indexCounter++] = secondSharedIndex;

            ret.indices[indexCounter++] = firstSharedIndex;
            ret.indices[indexCounter++] = (z + 1) * size + (x + 1);
            ret.indices[indexCounter++] = secondSharedIndex;
        }
    }

    ret.primitiveCount = (size - 1) * (size - 1) * 6;
    ret.primitiveType = Geometry::Triangles;

    ret.isIndexed = true;
    ret.isDirty = true;

    return ret;
}

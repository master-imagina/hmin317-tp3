#ifndef GEOMETRYDESC_H
#define GEOMETRYDESC_H

#include <vector>

#include "vertexattrib.h"


class GeometryDesc
{
public:
    GeometryDesc();

    void addAttribute(const VertexAttrib &attrib);

    const std::vector<VertexAttrib> &attributes() const;

private:
    std::vector<VertexAttrib> m_attributes;
};

#endif // GEOMETRYDESC_H

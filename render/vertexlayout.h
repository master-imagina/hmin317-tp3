#ifndef VERTEXLAYOUT_H
#define VERTEXLAYOUT_H

#include <vector>

#include "vertexattrib.h"


class VertexLayout
{
public:
    VertexLayout();

    void addAttribute(const VertexAttrib &attrib);

    const std::vector<VertexAttrib> &attributes() const;

private:
    std::vector<VertexAttrib> m_attributes;
};

#endif // VERTEXLAYOUT_H

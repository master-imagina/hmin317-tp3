#include "geometrydesc.h"


GeometryDesc::GeometryDesc() :
    m_attributes()
{}

void GeometryDesc::addAttribute(const VertexAttrib &attrib)
{
    auto attribFound =
            std::find(m_attributes.begin(), m_attributes.end(), attrib);

    if (attribFound == m_attributes.end()) {
        m_attributes.push_back(attrib);
    }
}

const std::vector<VertexAttrib> &GeometryDesc::attributes() const
{
    return m_attributes;
}

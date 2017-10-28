#include "vertexlayout.h"


VertexLayout::VertexLayout() :
    m_attributes()
{}

void VertexLayout::addAttribute(const VertexAttrib &attrib)
{
    auto attribFound =
            std::find(m_attributes.begin(), m_attributes.end(), attrib);

    if (attribFound == m_attributes.end()) {
        m_attributes.push_back(attrib);
    }
}

const std::vector<VertexAttrib> &VertexLayout::attributes() const
{
    return m_attributes;
}

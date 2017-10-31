#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include <map>

class ShaderProgram;


class ShaderManager
{
public:
    ShaderManager();

    void addShaderProgram(ShaderProgram *shaderProgram, unsigned int shaderId);

    unsigned int shaderIdForShaderProgram(ShaderProgram *shaderProgram);

private:
    std::map<ShaderProgram *, unsigned int> m_shaderProgramToId;
};

#endif // SHADERMANAGER_H

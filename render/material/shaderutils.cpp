#include "shaderutils.h"

#include <iostream>

#include <QFile>


namespace {

void readShaderFromFile(const std::string &filePath, std::string &sourceCode)
{
    if (filePath.empty()) {
        return;
    }

    QFile file(QString::fromStdString(filePath));

    if (!file.open(QFile::ReadOnly)) {
        std::cerr << "[ERROR] shaderProgramFromFile() : "
                  << "can't open " << filePath
                  << std::endl;
        return;
    }

    sourceCode = file.readAll().toStdString();
}

} // anon namespace

uptr<ShaderProgram> shaderProgramFromFile(const std::string &vertFilePath,
                                          const std::string &fragFilePath)
{
    auto ret = std::make_unique<ShaderProgram>();

    readShaderFromFile(vertFilePath, ret->vertexShaderSource);
    readShaderFromFile(fragFilePath, ret->fragmentShaderSource);

    return ret;
}

uptr<ShaderProgram> shaderProgramFromFile(const std::string &vertFilePath,
                                          const std::string &geomFilePath,
                                          const std::string &fragFilePath)
{
    auto ret = std::make_unique<ShaderProgram>();

    readShaderFromFile(vertFilePath, ret->vertexShaderSource);
    readShaderFromFile(geomFilePath, ret->geometryShaderSource);
    readShaderFromFile(fragFilePath, ret->fragmentShaderSource);

    return ret;
}

uptr<ShaderProgram> shaderProgramFromSource(const std::string &vertShaderSource,
                                            const std::string &geomShaderSource,
                                            const std::string &fragShaderSource)
{
    auto ret = std::make_unique<ShaderProgram>();

    ret->vertexShaderSource = vertShaderSource;
    ret->geometryShaderSource = geomShaderSource;
    ret->fragmentShaderSource = fragShaderSource;

    return ret;
}

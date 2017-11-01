#include "shaderutils.h"

#include <iostream>

#include <QFile>


namespace {

void readShaderFromFile(const std::string &filePath, QByteArray &sourceCode)
{
    if (filePath.empty()) {
        return;
    }

    QFile file(QString::fromStdString(filePath));

    if (!file.open(QFile::ReadOnly)) {
        std::cerr << "shaderProgramFromFile() : "
                  << "can't open " << filePath;
        return;
    }

    sourceCode = file.readAll()/*.constData()*/;
    sourceCode.append('\0');
}

} // anon namespace


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

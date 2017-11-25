#include "builtins.h"

#include <QColor>
#include <QVector3D>

#include "render/material/renderpass.h"
#include "render/material/shaderparam.h"
#include "render/material/shaderprogram.h"

#include "render/renderassets.h"


Material defaultMaterial()
{
    Material ret;
    ret.setParam("color", QColor(Qt::white));

    RenderPass *basePass = ret.addRenderPass("base");
    uptr<ShaderProgram> shader =
            shaderProgramFromFile("builtins/shaders/default.vert",
                                  "builtins/shaders/default.frag");

    basePass->setShaderProgram(std::move(shader));

    return ret;
}

Material phongMaterial()
{
    Material ret;

    RenderPass *basePass = ret.addRenderPass("base");
    uptr<ShaderProgram> shader =
            shaderProgramFromFile("builtins/shaders/phong.vert",
                                  "builtins/shaders/phong.frag");

    basePass->setShaderProgram(std::move(shader));

    ret.setParam("ka", QColor::fromRgbF(1.f, 0.5f, 0.31f));
    ret.setParam("kd", QColor::fromRgbF(1.f, 0.5f, 0.31f));
    ret.setParam("ks", QColor::fromRgbF(0.5f, 0.5f, 0.5f));
    ret.setParam("shininess", 32.f);

    return ret;
}

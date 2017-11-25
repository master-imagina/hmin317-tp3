#include "builtins.h"

#include <QColor>
#include <QVector3D>

#include "render/material/renderpass.h"
#include "render/material/shaderparam.h"
#include "render/material/shaderprogram.h"

#include "render/renderassets.h"


Material phongMaterial()
{
    Material ret;

    RenderPass *basePass = ret.addRenderPass("base");
    uptr<ShaderProgram> shader =
            shaderProgramFromFile("common/shaders/phong.vert",
                                  "common/shaders/phong.frag");

    basePass->setShaderProgram(std::move(shader));

    ret.setParam("material.ka", QVector3D(1.f, 0.5f, 0.31f));
    ret.setParam("material.kd", QVector3D(1.f, 0.5f, 0.31f));
    ret.setParam("material.ks", QVector3D(0.5f, 0.5f, 0.5f));
    ret.setParam("material.shininess", 32.f);

    return ret;
}

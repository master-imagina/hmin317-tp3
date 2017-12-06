#include "api.h"

#include <type_traits>

extern "C" {
#include <lua5.3.4/lua.h>
#include <lua5.3.4/lauxlib.h>
#include <lua5.3.4/lualib.h>
}

#include "3rdparty/luabridge/luabridge/LuaBridge.h"

#include "3rdparty/entityx/Entity.h"

#include "input/keyboard.h"

#include "render/camera.h"

#include "extras/cameraactions.h"


namespace {

Camera *getComponentCamera(entityx::Entity entity)
{
    Camera *ret = nullptr;

    if (entity.has_component<Camera>()) {
        ret = entity.component<Camera>().get();
    }

    return ret;
}

Keyboard *getComponentKeyboard(entityx::Entity entity)
{
    Keyboard *ret = nullptr;

    if (entity.has_component<Keyboard>()) {
        ret = entity.component<Keyboard>().get();
    }

    return ret;
}

inline QVector3D qvector3D_new(float x, float y, float z)
{
    return QVector3D(x, y, z);
}

inline QVector3D qvector3D_add(const QVector3D &lhs, const QVector3D &rhs)
{
    return lhs + rhs;
}

inline QVector3D qvector3D_mul(const QVector3D &lhs, const QVector3D &rhs)
{
    return lhs * rhs;
}

inline QVector3D qvector3D_mul(const QVector3D &lhs, float rhs)
{
    return lhs * rhs;
}

std::vector<int> keys {
    Qt::Key_A,
    Qt::Key_B,
    Qt::Key_C,
    Qt::Key_D,
    Qt::Key_E,
    Qt::Key_F,
    Qt::Key_G,
    Qt::Key_H,
    Qt::Key_I,
    Qt::Key_J,
    Qt::Key_K,
    Qt::Key_L,
    Qt::Key_M,
    Qt::Key_N,
    Qt::Key_O,
    Qt::Key_P,
    Qt::Key_Q,
    Qt::Key_R,
    Qt::Key_S,
    Qt::Key_T,
    Qt::Key_U,
    Qt::Key_V,
    Qt::Key_W,
    Qt::Key_X,
    Qt::Key_Y,
    Qt::Key_Z,
    Qt::Key_Control,
    Qt::Key_Alt,
    Qt::Key_Shift,
};

} // namespace luabridge


void exposeEngineAPI(lua_State *lState)
{
    luabridge::getGlobalNamespace(lState)
        .beginNamespace("Key")
            .addVariable("A", &keys[0], false)
            .addVariable("B", &keys[1], false)
            .addVariable("C", &keys[2], false)
            .addVariable("D", &keys[3], false)
            .addVariable("E", &keys[4], false)
            .addVariable("F", &keys[5], false)
            .addVariable("G", &keys[6], false)
            .addVariable("H", &keys[7], false)
            .addVariable("I", &keys[8], false)
            .addVariable("J", &keys[9], false)
            .addVariable("K", &keys[10], false)
            .addVariable("L", &keys[11], false)
            .addVariable("M", &keys[12], false)
            .addVariable("N", &keys[13], false)
            .addVariable("O", &keys[14], false)
            .addVariable("P", &keys[15], false)
            .addVariable("Q", &keys[16], false)
            .addVariable("R", &keys[17], false)
            .addVariable("S", &keys[18], false)
            .addVariable("T", &keys[19], false)
            .addVariable("U", &keys[20], false)
            .addVariable("V", &keys[21], false)
            .addVariable("W", &keys[22], false)
            .addVariable("X", &keys[23], false)
            .addVariable("Y", &keys[24], false)
            .addVariable("Z", &keys[25], false)
            .addVariable("Ctrl", &keys[26], false)
            .addVariable("Alt", &keys[27], false)
            .addVariable("Shift", &keys[28], false)
        .endNamespace();

    luabridge::getGlobalNamespace(lState)
        .beginClass<QVector3D>("vec3")
            .addConstructor<void (*) (void)>()
            .addFunction("normalized", &QVector3D::normalized)
        .endClass()
        .addFunction("vec3_new", &qvector3D_new)
        .addFunction("vec3_add", &qvector3D_add)
        .addFunction("vec3_mul", (QVector3D (*)(const QVector3D &, const QVector3D &))&qvector3D_mul)
        .addFunction("vec3_mul", (QVector3D (*)(const QVector3D &, float))&qvector3D_mul);

    luabridge::getGlobalNamespace(lState)
        .beginClass<entityx::Entity>("Entity")
        .endClass();

    luabridge::getGlobalNamespace(lState)
        .beginClass<Keyboard>("Keyboard")
            .addFunction("keyIsPressed", &Keyboard::keyIsPressed)
        .endClass();

    luabridge::getGlobalNamespace(lState)
        .beginClass<Camera>("Camera")
            .addProperty("eyePos", &Camera::eyePos, &Camera::setEyePos)
            .addProperty("targetPos", &Camera::targetPos, &Camera::setTargetPos)
            .addProperty("upVector", &Camera::upVector, &Camera::setUpVector)
            .addProperty("aspectRatio", &Camera::aspectRatio, &Camera::setAspectRatio)
            .addFunction("viewVector", &Camera::viewVector)
            .addFunction("rightVector", &Camera::rightVector)
        .endClass();

    luabridge::getGlobalNamespace(lState)
        .beginNamespace("CameraActions")
            .addFunction("move", &moveCamera)
        .endNamespace();

    luabridge::getGlobalNamespace(lState)
        .addFunction("getComponentCamera", &getComponentCamera)
        .addFunction("getComponentKeyboard", &getComponentKeyboard);
}

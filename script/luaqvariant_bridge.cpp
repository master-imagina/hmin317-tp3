#include "luaqvariant_bridge.h"

#include <QVector3D>

#include "core/scene.h"


QVariant luaRefToVariant(const luabridge::LuaRef &luaValue)
{
    QVariant ret;

    if (luaValue.isNumber()) {
        ret = luaValue.cast<float>();
    }
    else if (luaValue.isString()) {
        ret = QString::fromStdString(luaValue.cast<std::string>());
    }
    // Userdata only
    else if (luaValue.is<QVector3D>()) {
        ret = QVariant::fromValue(luaValue.cast<QVector3D>());
    }
    else if (luaValue.is<entityx::Entity>()) {
        auto entity = luaValue.cast<entityx::Entity>();

        ret = QVariant::fromValue(entity.id());
    }

    return ret;
}

void commitParamValueToLua(const std::string &name,
                           const QVariant &qtValue,
                           luabridge::LuaRef &luaPropsTable,
                           entityx::EntityManager &entityManager)
{
    const int type = qtValue.userType();

    if (type == QMetaType::Int) {
        luaPropsTable[name] = qtValue.toInt();
    }
    else if (type == QMetaType::Float) {
        luaPropsTable[name] = qtValue.toFloat();
    }
    else if (type == QMetaType::QVector3D) {
        luaPropsTable[name] = qtValue.value<QVector3D>();
    }
    else if (type == qMetaTypeId<entityx::Entity::Id>()) {
        auto entityId = qtValue.value<entityx::Entity::Id>();

        luaPropsTable[name] = entityManager.get(entityId);
    }
}

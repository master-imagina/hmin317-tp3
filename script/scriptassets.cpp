#include "scriptassets.h"

#include <iostream>

#include <QFile>
#include <QVector3D>

extern "C" {
#include <lua5.3.4/lua.h>
#include <lua5.3.4/lauxlib.h>
#include <lua5.3.4/lualib.h>
}

#include "3rdparty/luabridge/luabridge/LuaBridge.h"

#include "core/assetmanager.h"

#include "script/luaserver.h"


namespace {

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

    return ret;
}

} // anon namespace

Script scriptFromFile(const std::string &path, LuaServer &luaServer)
{
    Script ret;
    ret.path = path;

    AssetManager *assetManager = AssetManager::self();

    if (assetManager->bigFile().entryCount() == 0) {
        const QString assetPath = assetManager->pathFor(QString::fromStdString(path));

        QFile file(assetPath);

        if (!file.open(QFile::ReadOnly)) {
            std::cerr << "[ERROR] scriptFromFile() : "
                      << "can't open " << path
                      << std::endl;
        }
        else {
            ret.sourceCode = file.readAll();
        }
    }
    else {
        ret.sourceCode = assetManager->bigFile().data(path);
    }

    // Retrieve script properties
    luaServer.evaluateScript(ret);

    luabridge::LuaRef propsTable = luaServer.getPropertiesTable();
    LuaKeyValueMap props = luaServer.getKeyValueMap(propsTable);

    for (auto &p : props) {
        ret.addParam(p.first, luaRefToVariant(p.second));
    }

    return ret;
}

#include "scriptassets.h"

#include <iostream>

#include <QFile>
#include <QVector3D>

#include <entityx/Entity.h>

#include "core/assetmanager.h"

#include "script/lua_includes.h"
#include "script/luaqvariant_bridge.h"
#include "script/luaserver.h"


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

    // Retrieve script parameters
    luaServer.evaluateScript(ret);

    luabridge::LuaRef propsTable = luaServer.getPropertiesTable();
    LuaKeyValueMap props = luaServer.getKeyValueMap(propsTable);

    for (auto &p : props) {
        ret.addParam(p.first, luaRefToVariant(p.second));
    }

    // Retrieve private properties
    luabridge::LuaRef privatePropsTable = luaServer.getPrivatePropertiesTable();

    for (const auto &p : luaServer.getKeyValueMap(privatePropsTable)) {
        ret.setPrivateProperty(p.first, luaRefToVariant(p.second));
    }

    return ret;
}

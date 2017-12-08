#ifndef SERIALIZATION_H
#define SERIALIZATION_H

class QDataStream;

class LuaServer;
class Scene;


QDataStream &operator<<(QDataStream &os, const Scene &scene);

QDataStream &deserializeScene(QDataStream &os, Scene &scene, LuaServer &luaServer);

#endif // SERIALIZATION_H

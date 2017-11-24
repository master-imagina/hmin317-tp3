#ifndef SERIALIZATION_H
#define SERIALIZATION_H

class QDataStream;

class Scene;


QDataStream &operator<<(QDataStream &os, const Scene &scene);
QDataStream &operator>>(QDataStream &os, Scene &scene);

#endif // SERIALIZATION_H

#include "serialization.h"

#include "core/scene.h"

#include "extras/particles/particleeffect.h"
#include "extras/particles/quick.h"

#include "input/keyboard.h"

#include "physics/collider.h"
#include "physics/rigidbody.h"

#include "render/camera.h"
#include "render/light.h"
#include "render/mesh.h"
#include "render/transform.h"

#include "render/material/material.h"
#include "render/material/renderpass.h"
#include "core/param.h"
#include "render/material/shaderprogram.h"

#include "script/script.h"
#include "script/scriptassets.h"


QDataStream &operator<<(QDataStream &os, const Scene &scene)
{
    qRegisterMetaTypeStreamOperators<entityx::Entity::Id>("entityx::Entity::Id");


    os << (quint64) scene.count();

    for (entityx::Entity &entity : scene.entityCache()) {
        os << QString("entity");

        if (entity.has_component<Transform>()) {
            os << QString("transform");
            os << *entity.component<Transform>().get();
        }
        if (entity.has_component<Mesh>()) {
            os << QString("mesh");
            os << *entity.component<Mesh>().get();
        }
        if (entity.has_component<ParticleEffect>()) {
            os << QString("particleeffect");
            os << *entity.component<ParticleEffect>().get();
        }
        if (entity.has_component<Light>()) {
            os << QString("light");
            os << *entity.component<Light>().get();
        }
        if (entity.has_component<Material>()) {
            os << QString("material");
            os << *entity.component<Material>().get();
        }
        if (entity.has_component<Camera>()) {
            os << QString("camera");
            os << *entity.component<Camera>().get();
        }
        if (entity.has_component<Keyboard>()) {
            os << QString("keyboard");
        }
        if (entity.has_component<Script>()) {
            os << QString("script");
            os << *entity.component<Script>().get();
        }
        if (entity.has_component<Collider>()) {
            os << QString("collider");
            os << *entity.component<Collider>().get();
        }
        if (entity.has_component<RigidBody>()) {
            os << QString("rigidbody");
            os << *entity.component<RigidBody>().get();
        }

        os << QString("endentity");
    }
}

QDataStream &deserializeScene(QDataStream &os, Scene &scene, LuaServer &luaServer)
{
    quint64 entityCount = 0;
    os >> entityCount;

    for (int i = 0; i < entityCount; i++) {
        QString dummyEntityFlag;
        os >> dummyEntityFlag;

        entityx::Entity entity = scene.createEntity();

        QString endEntityFlag;
        os >> endEntityFlag;

        while (endEntityFlag != "endentity") {
            const QString componentType = endEntityFlag;

            if (componentType == "transform") {
                Transform transform;
                os >> transform;

                entity.replace<Transform>(transform);
            }
            else if (componentType == "mesh") {
                Mesh mesh;
                os >> mesh;

                entity.assign<Mesh>(mesh);
            }
            else if (componentType == "particleeffect") {
                ParticleEffect particleEffect;
                os >> particleEffect;

                createParticleEffect(entity,
                                     particleEffect.direction(),
                                     particleEffect.count(),
                                     particleEffect.maxLife(),
                                     particleEffect.radius(),
                                     particleEffect.speed(),
                                     particleEffect.particleSize(),
                                     particleEffect.spawnRate());
            }
            else if (componentType == "light") {
                Light light;
                os >> light;

                entity.assign<Light>(light);
            }
            else if (componentType == "material") {
                Material material;
                os >> material;

                entity.assign<Material>(material);
            }
            else if (componentType == "camera") {
                Camera camera;
                os >> camera;

                entity.assign<Camera>(camera);
            }
            else if (componentType == "keyboard") {
                entity.assign<Keyboard>();
            }
            else if (componentType == "script") {
                Script script;
                os >> script;
                script.sourceCode = scriptFromFile(script.path, luaServer).sourceCode;

                entity.assign<Script>(script);
            }
            else if (componentType == "collider") {
                Collider collider;
                os >> collider;

                entity.assign<Collider>(collider);
            }
            else if (componentType == "rigidbody") {
                RigidBody rigidBody;
                os >> rigidBody;

                entity.assign<RigidBody>(rigidBody);
            }

            os >> endEntityFlag;
        }
    }
}

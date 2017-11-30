#include "hooksystems.h"

#include "core/systemengine.h"

#include "editor/componentview.h"

#include "extras/particles/particleeffect.h"

#include "input/keyboard.h"

#include "render/camera.h"
#include "render/light.h"
#include "render/mesh.h"
#include "render/transform.h"

#include "render/geometry/geometry.h"

#include "render/material/material.h"

#include "script/script.h"


void registerHookSystems(SystemEngine &systemEngine,
                         ComponentView &componentView)
{
    systemEngine.registerSystem<ComponentAddedHook<Transform>>(componentView);
    systemEngine.registerSystem<ComponentAddedHook<Geometry>>(componentView);
    systemEngine.registerSystem<ComponentAddedHook<Material>>(componentView);
    systemEngine.registerSystem<ComponentAddedHook<ParticleEffect>>(componentView);
    systemEngine.registerSystem<ComponentAddedHook<Mesh>>(componentView);
    systemEngine.registerSystem<ComponentAddedHook<Light>>(componentView);
    systemEngine.registerSystem<ComponentAddedHook<Camera>>(componentView);
    systemEngine.registerSystem<ComponentAddedHook<Keyboard>>(componentView);
    systemEngine.registerSystem<ComponentAddedHook<Script>>(componentView);

    systemEngine.configure();
}

#include "hooksystems.h"

#include "core/systemengine.h"

#include "editor/componentview.h"

#include "extras/particles/particleeffect.h"

#include "render/transform.h"


void registerHookSystems(SystemEngine &systemEngine,
                         ComponentView &componentView)
{
    systemEngine.registerSystem<ComponentAddedHook<Transform>>(componentView);
    systemEngine.registerSystem<ComponentAddedHook<ParticleEffect>>(componentView);

    systemEngine.initialize();
}

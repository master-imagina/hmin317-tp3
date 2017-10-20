#include "particulesengine.h"

particulesEngine::particulesEngine()
{

}

bool particulesEngine::operator <(particulesEngine const &p) const {
    return this->cameraDistance > p.cameraDistance;
}

/**
 * @brief particulesEngine::findUnusedParticle
 * @param lastUsedParticle
 * @param particleContainer
 * @return indice where a new particle could be create
 */
int particulesEngine::findUnusedParticle(std::vector<particulesEngine> &particleContainer, int lastUsedParticle){

    for (int i = lastUsedParticle; i < particleContainer.size(); ++i) {
        if(particleContainer.at(i).life < 0){
            lastUsedParticle = i;
            return i;
        }
    }


    for (int i = 0; i < lastUsedParticle; ++i) {
        if(particleContainer.at(i).life < 0){
            lastUsedParticle = i;
            return i;
        }
    }

    return 0;
}

void particulesEngine::sortParticles(std::vector<particulesEngine> &particleContainer){
    std::sort(particleContainer.begin(), particleContainer.end());
}

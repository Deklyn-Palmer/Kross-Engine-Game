/*
 *  Author: Jake Warren.
 *  Editors:
 *      - Jake Warren.
 */

#pragma once

#include "../../Core.h"
#include "Particle.h"

namespace Kross
{
    class KROSS_API WaterParticle : public Particle
    {
    public:
        WaterParticle()
        {
            AddParticleFlag(b2_waterParticle | b2_fixtureContactFilterParticle);
            AddParticleGroupFlag(b2_solidParticleGroup);
            SetShape(1.0f);

            //SetColor(Vector4(0.0f, 100.0f, 207.0f, 255.0f));

            SetPosition(Vector2(0, 1));
        }
    };
}
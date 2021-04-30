/*
 *  Author: Jake Warren.
 *  Editors:
 *      - Jake Warren.
 */
#pragma once

#include "../Core.h"

#include "Component.h"
#include "../Physics/Physics.h"
#include "../Physics/PhysicsScene.h"
#include "../Physics/Particles/Particle.h"
#include "../Manager/ResourceManager.h"
#include "../Renderer/Shader/Shader.h"
#include "../Renderer/LineRenderer.h"

namespace Kross
{

    class KROSS_API ParticleEmitter : public Component
    {
    private:
        ParticleSystem* p_ParticleSystem;
        Particle* p_Particle;
        PhysicsScene* p_PhysicsScene;
        World* p_World;
        LineRenderer* p_Lines;
        Shader* p_DebugShader;

        int m_ParticleCount;

    protected:
        friend class PhyscisScene;
        friend class Particle;
        /* ParticleEmitter start method */
        void OnStart() override;

        /* ParticleEmitter update method */
        void OnUpdate() override;

        /* ParticleEmitter render method */
        void OnRender() override;

    public:

        ParticleEmitter() : p_Particle(new Particle()), p_Lines(new LineRenderer()) {};
        ~ParticleEmitter();

        PhysicsScene* GetPhysicsScene() const { return p_PhysicsScene; }
        ParticleSystem* GetParticleSystem() { return p_ParticleSystem; }

        void SetParticleCount(int count) { m_ParticleCount = count; };

        /* This is to create the particle system which is used to create the particles */
        void OnCreateParticleSystem();

        /* Creates the individual particle */
        void OnCreateParticle();

        /* Creates the particle group */
        void OnCreateParticleGroup();

        Particle* GetParticle() { return p_Particle; }

        /* The Radius sets the size of the particles */
        /* This is can only be set when there are no particles present */
        /* That may be a lie */
        void SetRadius(float radius) { p_ParticleSystem->SetRadius(radius); }
        /* Gets the particle radius*/
        float GetRadius() { return p_ParticleSystem->GetRadius(); }

        /* Sets the density of the particle */
        /* The density affects the mass of the particles which means */
        /* that this will change how they interact with Rigidbodies */
        /* but does not affect how the particles interact with each other */
        void SetDensity(float density) { p_ParticleSystem->SetDensity(density); }
        /* Returns the Density of the particle system */
        float GetDensity() { return p_ParticleSystem->GetDensity(); }

        /* Sets the maximum number of particles */
        /* A value of 0 means there is no maximum */
        void SetMaxCount(int max) { p_ParticleSystem->SetMaxParticleCount(max); }
        /* Returns the max particle count of the particle system */
        float GetMaxCount() { return p_ParticleSystem->GetMaxParticleCount(); }

        /* Enable / disable destruction of particles in CreateParticle() */
        /* when no more particles can be created due to a prior call to SetMaxParticleCount() */
        /* when enabled the oldest particle in CreateParticle() is destroyed */
        void SetDestroyByAge(bool destroy) { p_ParticleSystem->SetDestructionByAge(destroy); }
        /* Returns destruction by age */
        bool GetDestroyByAge() { return p_ParticleSystem->GetDestructionByAge(); }

        /* Sets the lifetime of a particle in seconds */
        /* A lifetime less than or equal to 0 */
        /* will result in the particle living forever or unitl manually destroyed */
        void SetLifetime(int index, float lifetime) { p_ParticleSystem->SetParticleLifetime(index, lifetime); }
        /* Gets the lifetime in seconds if it a value above 0.0f */
        /* otherwise if the value is bellow 0.0f the lifetime is infinite */
        float GetLifetime(int index) { return p_ParticleSystem->GetParticleLifetime(index); }

        /* Set the strict particle/body contact check */
        void SetStrictContactCheck(bool contact) { p_ParticleSystem->SetStrictContactCheck(contact); }
        /* Gets the strict contact check */
        bool GetStrictContactCheck() { return p_ParticleSystem->GetStrictContactCheck(); }

        /* This is used to pause/unpause the particle system */
        /* When paused the world step skips over the particle system */
        void SetPaused(bool paused) { p_ParticleSystem->SetPaused(paused); }
        /* Returns if the particle system is being updated int the world step */
        bool GetNotPaused() { return p_ParticleSystem->GetPaused(); }

        /* Changes the particle gravity scale */
        void SetGravityScale(float scale) { p_ParticleSystem->SetGravityScale(scale); }
        /* Returns the particle gravity scale */
        float GetGravityScale() { return p_ParticleSystem->GetGravityScale(); }

        /* Sets flags for a specified particle */
        void SetParticleFlags(int index, ParticleFlag flags) { p_ParticleSystem->SetParticleFlags(index, flags); }
        /* Gets the flags of a specific particle */
        int GetParticleFlags(int index) { return p_ParticleSystem->GetParticleFlags(index); }

        /* Sets how many iterations until a particle is determined to be stuck */
        void SetStuckThreshold(int iterations) { p_ParticleSystem->SetStuckThreshold(iterations); }
        /* Gets the potentially stuck particles from the last step */
        /* The user must decide if they are stuck or otherwise and what to do with them */
        const int32* GetStuckCandidates() {return p_ParticleSystem->GetStuckCandidates(); }

        /* Sets the physics scene */
        void SetPhysicsScene(PhysicsScene* physicsScene) { p_PhysicsScene = physicsScene; }

    };
}
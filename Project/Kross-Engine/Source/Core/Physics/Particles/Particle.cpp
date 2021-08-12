/*
 *  Author: Jake Warren.
 *  Editors:
 *      - Jake Warren.
 */


#include "Particle.h"

namespace Kross
{
	ParticleSystem* Particle::CreateParticleSystem(ParticleProperties* properties, PhysicsScene* physicsScene)
	{
		return physicsScene->GetPhysicsWorld()->CreateParticleSystem(properties->GetParticleSystemDef());
	}
	
	ParticleDef Particle::CreateParticleDef(ParticleProperties* properties)
	{
		ParticleDef particleDef;
	
		particleDef.flags = properties->GetParticleFlags();
	
		particleDef.position = properties->GetPosition();
		particleDef.color = properties->GetColor();
	
		particleDef.velocity = properties->GetLinearVelocity();
	
		particleDef.lifetime = properties->GetLifetime();
		particleDef.userData = properties->GetUserData();
		particleDef.group = properties->GetGroup();
	
		return particleDef;
	}
	
	ParticleGroupDef Particle::CreateParticleGroupDef(ParticleProperties* properties)
	{
		return *properties->GetParticleGroupDef();
	}

}
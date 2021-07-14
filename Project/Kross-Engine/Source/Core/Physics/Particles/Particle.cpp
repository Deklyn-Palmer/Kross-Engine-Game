/*
 *  Author: Jake Warren.
 *  Editors:
 *      - Jake Warren.
 */

#include "Particle.h"

#include "../../Core.h"

namespace Kross
{
	Particle::~Particle()
	{
		delete p_Shape;
		delete p_Filter;
	}
	void Particle::CreateParticle(ParticleSystem* particleSystem)
	{
		particleSystem->CreateParticle(CreateParticleDef());
	}

	ParticleDef Particle::CreateParticleDef()
	{
		/* Creates the particleDef and assigns all avaliable variables */
		ParticleDef particleDef;
		particleDef.flags = GetParticleFlag();

		particleDef.position.Set(GetPosition().x, GetPosition().y);
		particleDef.color.Set(GetColor().r, GetColor().g, GetColor().b, GetColor().a);

		particleDef.userData = GetCollisionFilter();

		return particleDef;
	}

	void Particle::CreateParticleGroup(ParticleSystem* particleSystem)
	{
		particleSystem->CreateParticleGroup(CreateParticleGroupDef());
	}

	ParticleGroupDef Particle::CreateParticleGroupDef()
	{
		/* Creates the particleGroupDef and assigns all avaliable variables */
		ParticleGroupDef particleDef;

		particleDef.flags = GetParticleFlag();
		particleDef.groupFlags = GetParticleGroupFlag();

		particleDef.shape = GetShape();
		particleDef.strength = GetStrength();

		particleDef.angle = GetAngle();
		particleDef.angularVelocity = GetAngularVelocity();

		particleDef.position.Set(GetPosition().x, GetPosition().y);
		particleDef.color.Set(GetColor().r, GetColor().g, GetColor().b, GetColor().a);

		particleDef.userData = GetCollisionFilter();

		return particleDef;
	}
}
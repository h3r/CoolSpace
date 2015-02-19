#pragma once
#ifndef ENTITY_MOVILE_H
#define ENTITY_MOVILE_H

#include "EntityMesh.h"

class EntityMovile : public EntityMesh{
	public:
		EntityMovile();
		EntityMovile(std::string aMesh, std::string aTexture);
		~EntityMovile(void);
		float speed;
		float restart;
		bool ischased;
		double distanceToGoal;

		float shield,hull,energy;
		float maxShield,maxHull,maxEnergy;
		float shieldRecoveryRate,hullRepairRate,energyConsumptionRate;
		float shieldBoost,hullBoost,energyBoost;

		void update(double elapsed_time);
		virtual bool  EntityMovile::testRayCollision(Vector3 start, Vector3 dir, Vector3 & result, bool meshCheck, float max = 1000000);
		virtual bool testMesh2SphereCollision(Entity* ent);
};

#endif

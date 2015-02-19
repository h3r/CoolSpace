#pragma once
#ifndef ENTITY_MESH_H
#define ENTITY_MESH_H

#include "Entity.h"
#include "mesh.h"
#include "texture.h"

class EntityMesh: public Entity{

	public:
		Mesh* mesh;
		Mesh* mesh_low = NULL;
		Texture* texture;
		Texture* texture_low = NULL;
		Vector4 color;

		virtual void render(void);
		virtual void render(float);
		virtual void update(double);

		virtual Vector3 getCenter();
		virtual float getHalfSize();

		virtual bool collision(Entity* ent);
		virtual Vector3 collisionPoint(Entity* ent);
		virtual bool testMesh2MeshCollision(Entity * ent);
		virtual bool testMesh2SphereCollision(Entity* ent);

		virtual bool  EntityMesh::testRayCollision(Vector3 start, Vector3 dir, Vector3 & result, bool meshCheck, float max = 1000000);

		
};
#endif
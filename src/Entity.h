#pragma once
#ifndef ENTITY_H
#define ENTITY_H

#include "framework.h"
#include "includes.h"

class Entity
{
	static int ID;
public:
	//Shader shader;

	Entity(void);
	virtual ~Entity(void);

	virtual void render(void);
	virtual void render(float);
	virtual void update(double elapsed_time);
	std::string Material;

	std::string name;
	Matrix44 model;
	bool alive; //false inactive - true active 
	//La idea es guardar un vector de Entidades de cierto tipo en alguna parte y en ves de crearlas y destruirlas todo el tiempo
	//guardar las que ya nos se utilizan, y solo crear mas si la lista no tiene ninguna entidad inactiva.

	Vector3 getPosition();
	Vector3 getRotation();

	Vector3 dir;
	Vector3	rot;
	Vector3 agi;

	Entity* parent;
	std::vector<Entity*> children;

	void addChild(Entity* ent);
	void removeChild(Entity* ent);

	void move(float x, float y, float z);
	void move(Vector3);
	void scale(float x, float y, float z);
	void rotate(float angle, Vector3 vec);
	void rotate(Vector3 vec);
	void rotate(double x, double y, double z);
	void rotateToVecAngle(Vector3 unitDir, float angle);

	virtual bool testRayCollision(Vector3 start, Vector3 dir, Vector3 & result, bool meshCheck, float max = 1000000);

	virtual bool testMesh2MeshCollision(Entity * ent);
	virtual bool testMesh2SphereCollision(Entity * ent);

	Matrix44 getGlobalMatrix();

	//overlap and collision things
	//sphere overlap
	virtual Vector3 getCenter();
	virtual float distance(Entity * ent);


	virtual float getHalfSize();
	virtual bool collision(Entity* ent);
	virtual Vector3 collisionPoint(Entity* ent);


	virtual void destroy();

	virtual void reset();

};
#endif

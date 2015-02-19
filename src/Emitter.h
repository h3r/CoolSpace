#pragma once
#ifndef EMITTER_H
#define EMITTER_H

#include "Entity.h"
#include "framework.h"
#include <stdlib.h>
#include "ParticleManager.h"
#include "Particle.h"
#include "camera.h"

class Emitter : public Entity{

public:
	Emitter();

	virtual ~Emitter(void);

	void generateTrail();
	void generateExplotion();
	void generateCrazyAssExplotion();
	void generateShield();
	void generateHullHit();


	bool encendido;

	bool dies;

	float ttl;
	float life_time;
	int aux;
	float density;
	float randomness;
	Camera* _cam;


	int type;
	std::string texture;
	Vector4 color;
	Vector3 size;
	Vector3 auxDir;

	virtual void render(void);
	virtual void render(float);
	virtual void update(double);

	ParticleManager* manager;
	Particle* particle = NULL;

	Vector3 lastPosition;
	
};
#endif
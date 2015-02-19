#pragma once
#include "framework.h"
#include "Particle.h"

#include "mesh.h"
#include "Atlas.h"
#include "includes.h"
#include "camera.h"
#include <algorithm>
#include "texture.h"

class ParticleManager
{

	static ParticleManager* instance;

public:
	

	static ParticleManager* getInstance(){
		if (instance == NULL){
			instance = new ParticleManager();
		}
		return instance;
	}

	ParticleManager();
	~ParticleManager();
	ParticleManager(int qty, Atlas* inputAtlas, Camera* cam);

	void setup(int qty, Atlas* inputAtlas, Camera* cam);

	Atlas* atlas;
	Texture* texture;

	std::vector<Particle*> particles_vector;
	std::vector<int> dead_positions;

	void render();
	void update(float elapsed_time);
	Particle* getParticle(); //initializes particle and gives a pointer to it to be configured.

		
	unsigned int next_position;
	int max_position;
	Camera* camera;

	Mesh tMesh;

};
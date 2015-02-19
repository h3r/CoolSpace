#pragma once
#include "framework.h"
#include "Bullet.h"

#include "mesh.h"
#include "includes.h"
#include "ParticleManager.h"
#include "Particle.h"

class BulletManager
{

	static BulletManager* instance;



public:

	static BulletManager* getInstance(){

		if (instance == NULL){
			instance = new BulletManager();
		}
		return instance;
	}

	BulletManager();
	~BulletManager();

	void setup();
	std::vector<Bullet*> bullets_vector;
	ParticleManager* particles;
	void render();
	void update(float elapsed_time);
	void createBullet(Vector3 pos, Vector3 vel, std::string author);

	
	unsigned int first_inactive_position;

	Mesh bulletGeometry;
};
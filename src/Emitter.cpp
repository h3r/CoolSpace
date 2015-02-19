#include "Emitter.h"

Emitter::Emitter(){

	manager = ParticleManager::getInstance();
	_cam = manager->camera;
}

Emitter::~Emitter(void){}


void Emitter::render(){
	render(0.0);
}

void Emitter::render(float distance){
	
}

void Emitter::generateTrail(){
	//ponemos 2 flares
	Matrix44 _model = getGlobalMatrix();

	particle = manager->getParticle();
	particle->pos = Vector3(_model.m[12], _model.m[13], _model.m[14]);
	particle->first_update = false;
	particle->normal= Vector3(0.0, 1.0, 0.0);
	particle->angle = 0;
	particle->cameraType = 0;
	particle->type = 2;
	particle->kind = "FLARE";
	particle->frame = 0;
	particle->life_time = 0.1; particle->ttl = 0.1;
	particle->start_Color = Vector4(0.8, 0.8, 1.0, 1.0);
	particle->end_color = Vector4(0.1, 0.1, 0.0, 0.1);
	particle->start_size.random(Vector3(0.02, 0.0, 0.02));
	particle->end_size.random(0.02);
	//y un poco de humo
	particle = manager->getParticle();
	particle->pos = Vector3(_model.m[12], _model.m[13], _model.m[14]);
	particle->normal = Vector3(0.0, 1.0, 0.0);
	particle->angle = 0;
	particle->cameraType = 0;
	particle->type = 1;
	particle->kind = "RING";
	particle->frame = 1;
	particle->life_time = 0.25; particle->ttl = 0.25;
	particle->start_Color = Vector4(0.4, 0.4, 0.7, 1.0);
	particle->end_color = Vector4(0.1, 0.1, 0.1, 1.0);
	particle->start_size = Vector3(0.008, 0.0, 0.008);
	particle->end_size = Vector3(0.0, 0.0, 0.0);

	//lastPosition = Vector3(_model.m[12], _model.m[13], _model.m[14]);
}



void Emitter::generateExplotion(){
	//explosion1
	Matrix44 _model = getGlobalMatrix();
	float time = (1 - (ttl / life_time));
	if (time < 0.8){
		particle = manager->getParticle();
		Vector3 randPos;
		randPos.random(Vector3(10.0, 10.0, 10.0));
		particle->pos = Vector3(_model.m[12], _model.m[13], _model.m[14]) + randPos;
		particle->first_update = false;
		particle->cameraType = 0;
		particle->normal = Vector3(0.0, 1.0, 0.0);
		particle->angle = 0;
		particle->type = 1;
		particle->kind = "FLARE";
		particle->frame = 0;
		particle->life_time = particle->ttl = (life_time - ttl)*0.25;
		particle->start_Color = Vector4(0.5, 0.5, 0.5, 1.0);
		particle->end_color = Vector4(0.5, 0.5, 1.0, 1.0);
		particle->start_size = Vector3(0.09, 0.0, 0.09);
		particle->end_size = Vector3(0.1, 0.0, 0.1);

	}
	else{


		particle = manager->getParticle();
		Vector3 randPos;
		randPos.random(Vector3(20.0, 20.0, 20.0));
		particle->pos = Vector3(_model.m[12], _model.m[13], _model.m[14]) + randPos;
		particle->normal = Vector3(1.0, 1.0, 1.0).normalize();
		particle->angle = 0;
		particle->first_update = false;
		particle->cameraType = 0;
		particle->type = 0;
		particle->kind = "HEX";
		particle->frame = 1;
		particle->life_time = particle->ttl = particle->life_time = 0.35;
		particle->start_Color = Vector4(0.5, 0.5, 0.5, 1.0);
		particle->end_color = Vector4(0.5, 0.5, 1.0, 0.0);
		particle->start_size = Vector3(0.04, 0.0, 0.04);
		particle->end_size = Vector3(0.05, 0.0, 0.05);
	}
	if (ttl < 0.0){ encendido = false; }
}

void Emitter::generateHullHit(){
	//explosion1
	Matrix44 _model = getGlobalMatrix();
	float time = (1 - (ttl / life_time));

		particle = manager->getParticle();
		Vector3 randPos;
		randPos.random(Vector3(1.0, 1.0, 1.0));
		particle->pos = Vector3(_model.m[12], _model.m[13], _model.m[14]) + randPos;
		particle->velocity = auxDir*10;
		particle->first_update = false;
		particle->cameraType = 0;
		particle->normal = Vector3(0.0, 1.0, 0.0);
		particle->angle = 0;
		particle->type = 1;
		particle->kind = "CLOUD";
		particle->frame = rand()%22;
		particle->life_time = particle->ttl = 0.55;
		particle->start_Color = Vector4(0.5, 0.5, 0.5, 0.2);
		particle->end_color = Vector4(0.5, 0.5, 1.0, 0.0);
		particle->start_size = Vector3(0.1, 0.0, 0.1);
		particle->end_size = Vector3(0.05, 0.0, 0.05);

	if (ttl < 0.0){ encendido = false; }
}

void Emitter::generateCrazyAssExplotion(){
	//explosion1
	Matrix44 _model = getGlobalMatrix();
	float time = (1 - (ttl / life_time));
	if (time < 0.5){
		
		if (aux>1){
			particle = manager->getParticle();
			
			particle->pos = Vector3(_model.m[12], _model.m[13], _model.m[14]);
			particle->first_update = false;
			particle->cameraType = 0;
			if (aux == 2){ particle->normal = Vector3(1.0, 1.0, 1.0); }
			else{ particle->normal = Vector3(0.1, 1.0, 1.0); }
			particle->angle = 0;
			particle->type = 4;
			particle->kind = "RING";
			particle->frame = 0;
			particle->life_time = particle->ttl = 5;
			particle->start_Color = Vector4(0.3, 0.3, 0.7, 1.0);
			particle->end_color = Vector4(0.2, 0.2, 0.7, 0.0);
			particle->start_size = Vector3(0.0, 0.0, 0.0);
			particle->end_size = Vector3(2.0, 0.0, 2.0);
			aux--;
		}

		particle = manager->getParticle();
		Vector3 randPos;
		randPos.random((1 - ttl / (life_time*2)) * 400);
		particle->pos = Vector3(_model.m[12], _model.m[13], _model.m[14]) + randPos;
		particle->first_update = false;
		particle->cameraType = 0;
		particle->normal = Vector3(0.0, 1.0, 0.0);
		particle->angle = 0;
		particle->type = 1;
		particle->kind = "FLARE";
		particle->frame = 0;
		particle->life_time = particle->ttl = (life_time - ttl)*2.0;
		particle->start_Color = Vector4(0.5, 0.5, 0.5, 0.5);
		particle->end_color = Vector4(0.5, 0.5, 1.0, 0.5);
		particle->start_size = Vector3(0.9, 0.0, 0.9);
		particle->end_size = Vector3(1.0, 0.0, 1.0);
		particle = manager->getParticle();

		randPos.random((1 - ttl / (life_time * 2)) * 500);
		particle->pos = Vector3(_model.m[12], _model.m[13], _model.m[14]) + randPos;
		particle->first_update = false;
		particle->cameraType = 0;
		particle->normal = Vector3(0.0, 1.0, 0.0);
		particle->angle = 0;
		particle->type = 1;
		particle->kind = "FLARE";
		particle->frame = 0;
		particle->life_time = particle->ttl = (life_time - ttl)*2.0;
		particle->start_Color = Vector4(0.5, 0.5, 0.5, 0.5);
		particle->end_color = Vector4(0.5, 0.5, 1.0, 0.5);
		particle->start_size = Vector3(0.9, 0.0, 0.9);
		particle->end_size = Vector3(1.0, 0.0, 1.0);

		particle = manager->getParticle();
		randPos.random((1 - ttl /(life_time*2)) * 550);
		particle->pos = Vector3(_model.m[12], _model.m[13], _model.m[14]) + randPos;
		particle->first_update = false;
		particle->cameraType = 0;
		particle->normal = Vector3(0.0, 1.0, 0.0);
		particle->angle = 0;
		particle->type = 1;
		particle->kind = "CLOUD";
		particle->frame = rand()%20;
		particle->life_time = particle->ttl = (life_time - ttl)*2.0;
		particle->start_Color = Vector4(0.2, 0.2, 0.5, 0.5);
		particle->end_color = Vector4(0.5, 0.5, 1.0, 0.0);
		particle->start_size = Vector3(0.0, 0.0, 0.0);
		particle->end_size = Vector3(2.0, 0.0, 2.0);

	}
	else{

		if (aux>0){
			particle = manager->getParticle();
			particle->pos = Vector3(_model.m[12], _model.m[13], _model.m[14]);
			particle->first_update = false;
			particle->cameraType = 0;
			particle->normal = Vector3(0.0, 1.0, 0.0);
			particle->angle = 0;
			particle->type = 4;
			particle->kind = "RING";
			particle->frame = 0;
			particle->life_time = particle->ttl = (life_time - ttl) * 2.0;
			particle->start_Color = Vector4(0.3, 0.3, 0.7, 1.0);
			particle->end_color = Vector4(0.2, 0.2, 0.7, 1.0);
			particle->start_size = Vector3(0.0, 0.0, 0.0);
			particle->end_size = Vector3(15.0, 0.0, 15.0);
			particle = manager->getParticle();
			particle->pos = Vector3(_model.m[12], _model.m[13], _model.m[14]);
			particle->first_update = false;
			particle->cameraType = 0;
			particle->normal = Vector3(0.0, 0.0, 1.0);
			particle->angle = 0;
			particle->type = 4;
			particle->kind = "RING";
			particle->frame = 0;
			particle->life_time = particle->ttl = (life_time - ttl) * 2.0;
			particle->start_Color = Vector4(0.3, 0.3, 0.7, 1.0);
			particle->end_color = Vector4(0.2, 0.2, 0.7, 1.0);
			particle->start_size = Vector3(0.0, 0.0, 0.0);
			particle->end_size = Vector3(20.0, 0.0, 20.0);

			aux--;
		}
		particle = manager->getParticle();
		Vector3 randPos;
		randPos.random((1 - ttl / (life_time * 2)) * 500);
		particle->pos = Vector3(_model.m[12], _model.m[13], _model.m[14]) + randPos;
		particle->normal = Vector3(1.0, 1.0, 1.0).normalize();
		particle->angle = 0;
		particle->first_update = false;
		particle->cameraType = 0;
		particle->type = 0;
		particle->kind = "HEX";
		particle->frame = 1;
		particle->life_time = particle->ttl = 0.55;
		particle->start_Color = Vector4(0.5, 0.5, 0.5, 0.5);
		particle->end_color = Vector4(0.5, 0.5, 1.0, 0.0);
		particle->start_size = Vector3(0.4, 0.0, 0.4);
		particle->end_size = Vector3(0.5, 0.0, 0.5);

		particle = manager->getParticle();

		randPos.random((1 - ttl / (life_time * 2)) * 500);
		particle->pos = Vector3(_model.m[12], _model.m[13], _model.m[14]) + randPos;
		particle->normal = Vector3(1.0, 1.0, 1.0).normalize();
		particle->angle = 0;
		particle->first_update = false;
		particle->cameraType = 0;
		particle->type = 0;
		particle->kind = "HEX";
		particle->frame = 1;
		particle->life_time = particle->ttl = 0.55;
		particle->start_Color = Vector4(0.5, 0.5, 0.5, 0.2);
		particle->end_color = Vector4(0.5, 0.5, 1.0, 0.0);
		particle->start_size = Vector3(0.4, 0.0, 0.4);
		particle->end_size = Vector3(0.5, 0.0, 0.5);

		particle = manager->getParticle();

		randPos.random((1 - ttl / (life_time * 2)) * 500);
		particle->pos = Vector3(_model.m[12], _model.m[13], _model.m[14]) + randPos;
		particle->normal = Vector3(1.0, 1.0, 1.0).normalize();
		particle->angle = 0;
		particle->first_update = false;
		particle->cameraType = 0;
		particle->type = 0;
		particle->kind = "HEX";
		particle->frame = 1;
		particle->life_time = particle->ttl = 0.55;
		particle->start_Color = Vector4(0.5, 0.5, 0.5, 0.2);
		particle->end_color = Vector4(0.5, 0.5, 1.0, 0.0);
		particle->start_size = Vector3(0.4, 0.0, 0.4);
		particle->end_size = Vector3(0.5, 0.0, 0.5);

		particle = manager->getParticle();

		randPos.random((1 - ttl / (life_time * 2)) * 500);
		particle->pos = Vector3(_model.m[12], _model.m[13], _model.m[14]) + randPos;
		particle->normal = Vector3(1.0, 1.0, 1.0).normalize();
		particle->angle = 0;
		particle->first_update = false;
		particle->cameraType = 0;
		particle->type = 0;
		particle->kind = "HEX";
		particle->frame = 1;
		particle->life_time = particle->ttl = 0.55;
		particle->start_Color = Vector4(0.5, 0.5, 0.5, 0.2);
		particle->end_color = Vector4(0.5, 0.5, 1.0, 0.0);
		particle->start_size = Vector3(0.4, 0.0, 0.4);
		particle->end_size = Vector3(0.5, 0.0, 0.5);
	}
	if (ttl < 0.0){ encendido = false; }
}

void Emitter::generateShield(){
	//explosion1
	Matrix44 _model = getGlobalMatrix();
	
	particle = manager->getParticle();
	particle->pos = Vector3(_model.m[12], _model.m[13], _model.m[14]);
	particle->first_update = false;
	particle->cameraType = 0;
	particle->angle = 0;
	particle->type = 4;
	particle->kind = "ATFIELD";
	particle->frame = 0;
	particle->normal = auxDir;
	particle->life_time = 2.0; particle->ttl = 2.0;
	particle->start_Color = Vector4(0.5, 0.5, 1.0, 1.0);
	particle->end_color = Vector4(0.5, 0.5, 1.0, 0.0);
	particle->start_size = Vector3(0.009, 0.0, 0.009)*size.x;
	particle->end_size = Vector3(0.05, 0.0, 0.05)*size.x;
	
	encendido = false;
}



void Emitter::update(double elapsed_time){
	

	if (encendido){
		if (dies){ ttl -= (float)elapsed_time; }
		
		switch(type){
			case 0:
				if ((_cam->eye - getGlobalMatrix()*Vector3() ).length()  <500)
				generateTrail();
				break;
			case 1:
				generateExplotion();
				break;
			case 2:
				generateShield();
				break;
			case 3:
				generateCrazyAssExplotion();
				break;
			case 4:
				generateHullHit();
				break;
		}
	}
	else{
		if (parent){
			parent->removeChild(this);
			delete this;
		
		}
	}

	

}
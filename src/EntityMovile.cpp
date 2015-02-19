#include "EntityMovile.h"
#include "game.h"
#include "utils.h"
#include "Emitter.h"
#include "Level1.h"

EntityMovile::EntityMovile(void){
	shield = hull = energy = -1.0;
	maxShield = maxHull = maxEnergy = -1.0;
	shieldRecoveryRate = hullRepairRate = energyConsumptionRate = -1.0;
	restart = 3.0;
	ischased = false;
}

EntityMovile::~EntityMovile(void){
}
/*
void EntityMovile::update(double elapsed_time){
	this->move(dir);
	if (agi.x +agi.y+agi.z == 3.0){
		agi = agi * (1.0f/getHalfSize());
		std::cout <<"Radio "<< 1.0f / getHalfSize() << std::endl;
	}
	this->rotate(rot.x * agi.x, rot.y * agi.y, rot.z * agi.z);
	for (unsigned int i = 0; i< children.size(); i++){
		children[i]->update(elapsed_time);
	}
}*/
void EntityMovile::update(double elapsed_time){
	if (agi.x + agi.y + agi.z == 3.0){
		agi = Vector3(1.0 / getHalfSize(), 1.0 / getHalfSize(), 1.0 / getHalfSize());
	}
	Vector3 finaltime = agi * 250 * elapsed_time;
	if (restart > 0){
		restart -= elapsed_time;
	}
	else{
		shield += elapsed_time * 0.5;// shieldRecoveryRate;
	}
	
	energy -= elapsed_time * (0.025 + abs(dir.z)*0.075);
	if (shield > maxShield) shield = maxShield;
	if (energy < 0.0){ energy = 0.0; }
	else{
		speed = dir.z;
		this->rotate(rot.x * finaltime.y, rot.y * finaltime.y, rot.z * finaltime.z);
		this->move(dir * finaltime.z);
	}
	
	
	for (unsigned int i = 0; i< children.size(); i++){
		children[i]->update(elapsed_time);
	}
	
}/*
bool EntityMovile::testMesh2SphereCollision(Entity* ent){

	EntityMesh* tEnt = (EntityMesh*)ent;

	mesh->collisionModel->setTransform(model.m);
	return mesh->collisionModel->sphereCollision(tEnt->getCenter().v, tEnt->getHalfSize());
}*/
bool EntityMovile::testMesh2SphereCollision(Entity* ent){

	EntityMesh* tEnt = (EntityMesh*)ent;

	tEnt->mesh->collisionModel->setTransform(tEnt->model.m);
	//std::cout <<std::endl<< "it enters here"<<std::endl;
	
	
		if (tEnt->mesh->collisionModel->sphereCollision(getCenter().v, getHalfSize())){
			Vector3 result;
			tEnt->mesh->collisionModel->getCollisionPoint(result.v, false);
			Vector3 impactNormal = (Vector3(result.x, result.y, result.z) - getCenter()).normalize();
			//std::cout << "asteroid shield collision ocurred in " << result.x << " " << result.y << " " << result.z << std::endl;
			if (shield > 0){
				shield = 0;
				this->restart = 3;
				Emitter* emit = new Emitter();
				emit->auxDir = impactNormal;
				emit->dies = true;
				emit->size.x = getHalfSize()*0.02;
				emit->aux = getHalfSize();
				emit->encendido = true;
				emit->ttl = 1.2;
				emit->life_time = 2.0;
				emit->type = 2;
				emit->model.setTranslation(result.x, result.y, result.z);

				Game* game = Game::getInstance();
				Level1 * level = (Level1*)(game->states.back());
				level->addChild(emit, "EMITTER");

				model.setTranslation(result.x + (impactNormal.x * (ent->getHalfSize() + 50)), result.y + (impactNormal.y * (ent->getHalfSize() + 50)), result.z + (impactNormal.z * (ent->getHalfSize() + 50)));
			}
			else{
				/*muere*/
				//std::cout << "Dies" << std::endl;
				Emitter* emit = new Emitter();
				emit->dies = true;
				emit->encendido = true;
				emit->ttl = 1.2;
				emit->life_time = 2.0;
				emit->type = 1;
				emit->model.setTranslation(result.v[0], result.v[1], result.v[2]);

				Game* game = Game::getInstance();
				Level1 * level = (Level1*)(game->states.back());
				
				if (name == "player1"){ 
					Vector3 carrierpos = level->getChildren("CarrierAllied")->model*Vector3();
					Vector3 vectorRandom; vectorRandom.random(Vector3(1, 1, 1)); vectorRandom = vectorRandom + Vector3(1.0, 1.0, 1.0); //Rango entre 0 y 2
					float azimuth, altitude;
					azimuth = vectorRandom.x * M_PI;
					altitude = vectorRandom.y * M_PI;
					float x, y, z, r;
					r = level->getChildren("CarrierAllied")->getHalfSize();
					x = r * sin(altitude) * cos(azimuth);
					y = r * sin(altitude) * sin(azimuth);
					z = r * cos(altitude);
					model.setTranslation(carrierpos.x + x, carrierpos.y + y, carrierpos.z + z);
					 return true; 
				}
				alive = false;

				


				
				level->addChild(emit, "EMITTER");
				
				if (name != "CarrierAllied" && name != "CarrierEnemy" && name != "player1")destroy();//?
			}
			
			return true;
		}
	
	
	return false;
/*	tEnt->mesh->collisionModel->setTransform(model.m);
	return mesh->collisionModel->sphereCollision(tEnt->getCenter().v, tEnt->getHalfSize());*/
}
bool EntityMovile::testRayCollision(Vector3 start, Vector3 dir, Vector3 & result, bool meshCheck, float max){
	if (SphereRayCollision(getCenter().v, getHalfSize()*2, start.v, dir.v, result.v)){
		
		shield = (shield > 1) ? shield - 1.0 : 0.0;
		if (meshCheck && shield <= 0.0){//Si ha colisionado con esfera, comprobamos si ha colisionado con mesh	
			
			mesh->collisionModel->setTransform(model.m);
			if (mesh->collisionModel->rayCollision(start.v, (dir.normalize()).v, true, 0.0F, (dir).length())){
				hull = (hull > 0.0) ? hull - 1 : 0;
				mesh->collisionModel->getCollisionPoint(result.v, false);
				std::cout << "Hull Impact " << hull << "left " << std::endl;
				if (hull > 0.0){
					restart = 3;
					/*añade particula impacto en hull*/
					
					Vector3 impactNormal = (Vector3(result.x, result.y, result.z) - model * Vector3()).normalize();
					Emitter* emit = new Emitter();
					emit->auxDir = impactNormal;
					emit->dies = true;
					emit->aux = getHalfSize();
					emit->encendido = true;
					emit->ttl = 0.5;
					emit->life_time = 0.5;
					emit->type = 4;
					emit->model.setTranslation(result.x, result.y, result.z);

					Game* game = Game::getInstance();
					Level1 * level = (Level1*)(game->states.back());
					level->addChild(emit, "EMITTER");
				}
				else{
					/*muere*/
					//std::cout << "Dies" << std::endl;
					Emitter* emit = new Emitter();

					emit->dies = true;
					emit->encendido = true;
					emit->ttl = 1.2;
					emit->life_time = 2.0;
					emit->type = 1;
					emit->model.setTranslation(result.v[0], result.v[1], result.v[2]);

					Game* game = Game::getInstance();
					Level1 * level = (Level1*)(game->states.back());
					level->addChild(emit, "EMITTER");
					
					if (name == "player1"){
						hull = maxHull;
						shield = maxShield;
						energy = maxEnergy;
						Vector3 carrierpos = level->getChildren("CarrierAllied")->model*Vector3();
						Vector3 vectorRandom; vectorRandom.random(Vector3(1, 1, 1)); vectorRandom = vectorRandom + Vector3(1.0, 1.0, 1.0); //Rango entre 0 y 2
						float azimuth, altitude;
						azimuth = vectorRandom.x * M_PI;
						altitude = vectorRandom.y * M_PI;
						float x, y, z, r;
						r = level->getChildren("CarrierAllied")->getHalfSize();
						x = r * sin(altitude) * cos(azimuth);
						y = r * sin(altitude) * sin(azimuth);
						z = r * cos(altitude);
						model.setTranslation(carrierpos.x + x, carrierpos.y + y, carrierpos.z + z);
						return true;
					}
					alive = false;

					this->destroy();
					//if (name != "CarrierAllied" && name != "CarrierEnemy" && name != "player1")destroy();//?
				}
				
				return true;
			}
		}
		else{ 
			//std::cout << "Shielded " <<shield <<" left"<< std::endl;
			Vector3 impactNormal = (Vector3(result.x, result.y, result.z) - model * Vector3()).normalize();
			/*añade particula impacto en shield*/
			
			Emitter* emit = new Emitter();
			emit->auxDir = impactNormal;
			emit->dies = true;
			emit->size.x = getHalfSize()*0.02;
			emit->aux = getHalfSize();
			emit->encendido = true;
			emit->ttl = 1.2;
			emit->life_time = 2.0;
			emit->type = 2;
			emit->model.setTranslation(result.x, result.y, result.z);

			Game* game = Game::getInstance();
			Level1 * level = (Level1*)(game->states.back());
			level->addChild(emit, "EMITTER");

			
			
			return true; 
		}
	}
	return false;
}





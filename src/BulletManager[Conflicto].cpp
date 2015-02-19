#include "BulletManager.h"

BulletManager* BulletManager::instance = NULL;


BulletManager::BulletManager(){
	bullets_vector = std::vector<Bullet*>(1000);
	//initialize bullets
	for (int i = 0; i < 1000; i++){
		bullets_vector[i] = new Bullet();
		bullets_vector[i]->status = false;
	}
	
	first_inactive_position = 0;

	bulletGeometry = Mesh();
	bulletGeometry.primitive = GL_LINES;
}

void BulletManager::setup(){
	
	//initialize bullets
	for (int i = 0; i < bullets_vector.size(); i++){
		bullets_vector[i]->status = false;
		bullets_vector[i]->ttl = -1.0;
	}

	first_inactive_position = 0;

}

BulletManager::~BulletManager(){

}


void BulletManager::render(){

	bulletGeometry.clear();

	for (int i = 0; i<first_inactive_position; i++){
		Bullet* tBullet = bullets_vector[i];
		bulletGeometry.createBullet(tBullet->last_position, tBullet->position);
	}


	if (bulletGeometry.vertices.size() > 0){
		glColor3f(0.75,0.75,0.0);
		glLineWidth(3.0);
		bulletGeometry.render();
		glColor3f(1.0, 1.0, 1.0);
		glLineWidth(1);
	}
}

void BulletManager::update(float elapsed_time){
	std::vector<int> dead_bullets;

	//bulletGeometry.clear();

	for (unsigned int i = 0; i < first_inactive_position; i++){
		Bullet* tBullet = bullets_vector[i];
		if (tBullet->ttl>=0){
			tBullet->ttl -= elapsed_time;
			//le sumamos a cada bala (su vector de velocidad + gravedad) * elapsed_time
			tBullet->velocity = (tBullet->velocity - (Vector3(0.0, 9.8, 0.0)*elapsed_time)*0.01);
			tBullet->last_position = tBullet->position;
			tBullet->position = tBullet->position + (tBullet->velocity*elapsed_time);
		
			//bulletGeometry.createBullet(tBullet->last_position, tBullet->position);
		}
		else{
			dead_bullets.insert(dead_bullets.begin(),i);
		}
	}

	for (unsigned int i = 0; i < dead_bullets.size(); i++){
		//como nuestro vector de muertos esta alrevez,
		//sustancialmente estamos recorriendo el vector de fin a principio,
		//asi que no se modifican los valores hasta que no dejamos de necesitarlos
		bullets_vector.push_back(bullets_vector[dead_bullets[i]]);
		bullets_vector.erase(bullets_vector.begin()+dead_bullets[i]);
	}

	first_inactive_position -= dead_bullets.size();
	dead_bullets.clear();
}

void BulletManager::createBullet(Vector3 pos, Vector3 vel, std::string author){

	if (first_inactive_position < bullets_vector.size()){
		bullets_vector[first_inactive_position]->setup(pos, vel, author);
	}
	else{
		Bullet* tBullet = new Bullet();
		tBullet->setup(pos, vel, author);
		bullets_vector.push_back(tBullet);
	}

	first_inactive_position++;
}

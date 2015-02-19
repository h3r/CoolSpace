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
	particles = ParticleManager::getInstance();
	//initialize bullets
	for (unsigned int i = 0; i < bullets_vector.size(); i++){
		bullets_vector[i]->status = false;
		bullets_vector[i]->ttl = -1.5;

	}

	first_inactive_position = 0;

	bulletGeometry.primitive = GL_LINES;
}


void BulletManager::render(){

	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	//glEnableClientState(GL_VERTEX_ARRAY);

	bulletGeometry.clear();

	for (unsigned int i = 0; i<first_inactive_position; i++){
		
		Bullet* tBullet = bullets_vector[i];
		bulletGeometry.createBullet(tBullet->last_position, tBullet->position);
	}


	if (bulletGeometry.vertices.size() > 0){
		glColor4f(.5, 0.2, 0.2, 1.0);
		glLineWidth(0.6);
		bulletGeometry.render();
		
		

		glColor4f(0.2, 0.2, .5, 1.0);
		//glColor4f(0.24, 0.57, 0.52, 0.25);
		glLineWidth(3);
		bulletGeometry.render();
		glColor4f(1.0, 1.0, 1.0, 1.0);
		glLineWidth(1);
	}

	
	//glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);


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
			tBullet->last_position = tBullet->last_position+(tBullet->velocity*elapsed_time)*(1.0-tBullet->ttl/tBullet->maxttl);
			//tBullet->last_position = tBullet->position;
			tBullet->position = tBullet->position + (tBullet->velocity*elapsed_time)*(tBullet->ttl/tBullet->maxttl);
			
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

	/*Particle* shoot = particles->getParticle();
	shoot->pos = pos;
	shoot->first_update = false;
	shoot->normal = Vector3(0.0, 1.0, 0.0);
	shoot->angle = 0;
	shoot->cameraType = 0;
	shoot->type = 1;
	shoot->kind = "WAVERING";
	shoot->frame = 0;
	shoot->life_time = 1.0; shoot->ttl = 1.0;
	shoot->start_Color = Vector4(1.0, 1.0, 1.0, 1.0);
	shoot->end_color = Vector4(0.1, 0.1, 0.0, 0.0);
	shoot->start_size = Vector3(0.025,0.025,.025);
	shoot->end_size = Vector3(0.0255, 0.025, 0.025);
	*/

	Vector3 _vel = vel * 5;

	if (first_inactive_position < bullets_vector.size()){
		bullets_vector[first_inactive_position]->setup(pos, _vel, author);
	}
	else{
		Bullet* tBullet = new Bullet();
		tBullet->setup(pos, _vel, author);
		bullets_vector.push_back(tBullet);
	}

	first_inactive_position++;
}

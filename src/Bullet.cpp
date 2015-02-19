#include "Bullet.h"

Bullet::Bullet(){

}

Bullet::~Bullet(void){
}

void Bullet::setup(Vector3 pos, Vector3 vel, std::string auth, int pow){
	position = pos;
	last_position = pos;
	velocity = vel;
	author = auth;

	power = pow;

	ttl = 1.0;
	maxttl = ttl;
	status = true;

}
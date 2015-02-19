#include "EntityMovile.h"
#include "game.h"


EntityMovile::EntityMovile(void){

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
	speed = dir.z;
	this->rotate(rot.x * finaltime.y, rot.y * finaltime.y, rot.z * finaltime.z);
	this->move(dir * finaltime.z);
	
	for (unsigned int i = 0; i< children.size(); i++){
		children[i]->update(elapsed_time);
	}

	
}





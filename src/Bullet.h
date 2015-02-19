#pragma once
#include "framework.h"
class Bullet
{
public:
	Bullet();
	~Bullet();


	void setup(Vector3 pos, Vector3 vel, std::string auth, int pow=10);

	//Atributos
	Vector3 position;
	Vector3 last_position;
	Vector3 velocity;
	float ttl;
	float maxttl;
	int power;
	std::string author;
	int type;
	
	bool status; //true is active, false is inactive
};
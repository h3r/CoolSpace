#pragma once
#include "framework.h"
#include "includes.h"
#include "mesh.h"

class Particle
{

public:
	bool cameraType; //0 complete alignment with camera / 1 partial alignment with camera

	int type; //
			  //0 dust and gas		:becomes transparent with proximity to camera
			  //1 fast explotion dies from ttl
			  //2 doesn't lose ttl, but rather it's destroyed after its first render
			  //3 particula con efecto yo-yo
	 
	
	float	ttl; //under 0, the particle is queued to be put to rest 
	float	life_time; //expected life time

	Vector3 pos;
	Vector3 velocity;
	int aux;

	Vector3 start_size;
	Vector3 size; //no nos interesa tanto que empiese o termine en un lugar, simplificamos el codigo
	Vector3 end_size;

	//lel, esto no funciona
	Vector3 normal; //rotacion de la parciula
	float angle;


	Vector4 start_Color;
	Vector4 current_Color; //guardamos el color inicial en current
	Vector4 end_color;

	std::string kind; //refers to the kind of graphic inside the atlas
	int frame; // refers to the number of graphic from those on kind vector
	
	double camera_dist;

	bool first_update =false;
};


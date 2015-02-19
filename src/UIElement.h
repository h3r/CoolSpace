#pragma once
#include "framework.h"
#include "includes.h"

class UIElement
{

public:
	int type; //0, kills it after the first render for things that should be just redrawn each time
			  //1, for things that should lay on screen for a while and changing colors/size, etc
			  //por el momento solo implementare 0
	int texture; //the numer of texture in the texture vector of the unity manager
	bool inUse;

	float	ttl; //under 0, the part
	Vector3 pos;
	float aux;

	Vector3 size; //no nos interesa tanto que empiese o termine en un lugar, simplificamos el codigo
	Vector3 size_speed;
	Vector3 size_accel;

	float rotation; //rotacion de la parciula
	float rotation_speed;

	Vector4 current_Color; //guardamos el color inicial en current
	Vector4 end_color;

	int currentframe; 
	float frameTime; // con que velocidad se pasan los frames, al azar para dar mas variedad
	double camera_dist;

};


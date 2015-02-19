#pragma once
#ifndef HOWTO_H
#define HOWTO_H

#include "State.h"
#include <string>
#include <vector>
#include "EntityMesh.h"


class HowToPlay:public State{
public:

	EntityMesh* screen;
	State* underState;
	//Funciones
	HowToPlay(void);
	~HowToPlay();

	virtual void init();
	virtual void clean();

	virtual void pause(){};
	virtual void resume(){};

	virtual void update(double elapsed_time);
	virtual void render(Camera* cam);

};

#endif
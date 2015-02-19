#pragma once
#ifndef PAUSE_H
#define PAUSE_H

#include "State.h"
#include <string>
#include <vector>



class Pause:public State{
public:

	//Funciones
	Pause(void);
	~Pause();

	virtual void init();
	virtual void clean();

	virtual void pause(){};
	virtual void resume(){};

	virtual void update(double elapsed_time);
	virtual void render(Camera* cam);

	
};

#endif
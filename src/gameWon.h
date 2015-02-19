#pragma once
#ifndef GWON_H
#define GWON_H

#include "State.h"
#include <string>
#include <vector>
#include "EntityMesh.h"


class gameWon:public State{
public:

	EntityMesh* screen;
	//Funciones
	gameWon(void);
	~gameWon();

	virtual void init();
	virtual void clean();

	virtual void pause(){};
	virtual void resume(){};

	virtual void update(double elapsed_time);
	virtual void render(Camera* cam);

	int game_status;
	int s_level;
	int s_difficulty;
	double s_gameTime;
	int s_alliedShipsDestroyed;
	int s_enemyShipsDestroyed;
	int s_playerShipsDestroyed;
	int s_recursosRecolectados;
};

#endif
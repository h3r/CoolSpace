#pragma once
#ifndef LEVEL1_H
#define LEVEL1_H

#include <vector>
#include <map>
#include <cassert>
#include <algorithm>

#include "includes.h"
#include "Clipper\clipper.h"

#include "Entity.h"
#include "Emitter.h"
#include "EntityMesh.h"
#include "EntityMovile.h"
#include "EntityBillBoard.h"

#include "BulletManager.h"
#include "ControllerManager.h"

#include "ParticleManager.h"
//#include "UIManager.h"

#include "State.h"

#include "game.h"
#include "Pause.h"
#include "gameOver.h"
#include "gameWon.h"

class Level1:State
{
	//Data Structures

	std::vector<std::string> categories;

	ParticleManager* particles;

//	UIManager* ui;

	Mesh * positionsAlly;
	Mesh * positionsEnemy;
	Mesh * test2;
	EntityMesh * mapGrid;

	Clipper fustrum;

	Game* game;
	EntityMesh* skybox;
	EntityMovile* player; //modelo al que se  le asigna el foco de las camaras y se asume que es el control del jugador
	EntityMovile*  CarrierAllied;
	EntityMovile*  CarrierEnemy;

public:
	std::map <std::string, std::vector<Entity *> > children;
	std::vector<Entity*> childrens;
	std::map<std::string, Entity*> childrensByID;

	Level1(void);
	~Level1(void);

	BulletManager * bullets;
	ControllerManager controllers;

	void render(Camera*);
	void renderMap (Camera*);
	void renderMinimap(Camera*);
	void renderGUI(Camera*,int);
	void renderMenu(Camera*, int);
	void update(double elapsed_time);

	void addChild(Entity* ent);
	void addChild(Entity* ent, std::string Material);
	void removeChild(Entity* ent);
	Entity* getChildren(std::string ID);

	void checkCollisions();


	int game_status;
	int s_level;
	int s_difficulty;
	double s_gameTime;
	int s_alliedShipsDestroyed;
	int s_enemyShipsDestroyed;
	int s_playerShipsDestroyed;
	int s_recursosRecolectados;
	float change_timer;

	virtual void init();
	virtual void clean();

	virtual void pause(){};
	virtual void resume(){};


	//init related
	void createAsteroids(Vector3 centeredOn, int  qty);
	EntityMovile* createDrone(Vector3 centeredOn, std::string team, std::string name  =  "");

	
	//actions
	EntityMovile* createAttacker(std::string team);
	EntityMovile* createDefender(std::string team);
	EntityMovile* createMiner(std::string team);
	EntityMovile* createAtackPlayer(std::string team);

	void interactions(double elapsed_time);
	int constructionMode;
	float nextUnit;

	void enemyLogic(double elapsed_time);
	float enemyCounter;

};

#endif
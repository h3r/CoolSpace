#pragma once
#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <vector>
#include <map>
#include "framework.h"
#include "Entity.h"
#include "BulletManager.h"
#include "State.h"


class Controller{

public:
	State* currentLevel;

	Entity* from;
	Entity* target;
	Entity * goal;

	Vector3 pos;
	Vector3 newpos;
	int state;
	bool isLoop;
	bool isChasing;
	int aux;
	int recolectState;
	bool greatOrbit;
	bool littleOrbit;
	int resources;

	double elapsed_time;
	int speed;
	int doing;
	int last_doing;
	Entity *auxEnt;

	Controller();
	Controller(Entity*);
	~Controller();

	void run(double elapsed_time);
	float shootrate;
	float current_shoot_channel;
	float timer1;
	float timer2;
	//Actions
	void Manual();

	void iAIdle();
	void iAMove(Vector3);
	void iAMove(Entity*);
	void iAFollow(Entity*);

	void iAShoot(Entity*);
	void iAAttack(Entity*);
	void iADefend();
	void iARepair();
	void iARepair(Entity*);
	void iARecolect();
	void iABuild();
	void iAOrbit(Entity*);
	void iAcheckEvents(Entity *);
	void iAAvoid(Entity  *);
	void iAAvoid(Vector3);
	bool iAProximity_Check(std::string, float);
	void iASpaceAsteroid();
	

	//Behavior
	void iABeAgresive();
	void iABeDefensive();
	void iAKamikaze();
	void iAConservative();

	void applyForce(const Vector3& force);
	void brake(const float rate);
	Vector3 rawForce(const Vector3& force);
	Vector3 vecLimitDeviationAngleUtility(const bool insideOrOutside, const Vector3& source, const float cosineOfConeAngle, const Vector3& basis);


	Vector3 velocity(void); 
	Vector3 seek(Vector3 loc); 
	void follow(Entity * ent);
	Vector3 flee(Vector3 loc);
	Vector3 avoidNeighbors(/*Neighbors*/); 
	Vector3 Intercept(const Entity * ent);
};
#endif
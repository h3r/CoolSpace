#pragma once
#ifndef CONTROLLER_MANAGER_H
#define CONTROLLER_MANAGER_H

#include <vector>
#include <map>

#include "Controller.h"
#include "Entity.h"

#include "State.h"

class ControllerManager
{
public:
	ControllerManager();
	~ControllerManager();
	
	std::vector<Controller* > controllers;
	std::map<std::string, Controller*> controllersByID;
	
	void addtoControl(Entity*);
	void addtoAIControl(Entity*);
	void removeControlled(std::string ID);
	void update(double elapsed_time);
	Controller * getControlled(std::string ID);

	State* currentLevel;

};
#endif
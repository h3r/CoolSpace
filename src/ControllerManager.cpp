#include "ControllerManager.h"
#include "Level1.h"
/*-<==>-----------------------------------------------------------------
/	Main constructor of the class
/----------------------------------------------------------------------*/
ControllerManager::ControllerManager()
{
	controllers.clear();
	controllersByID.clear();
	currentLevel = Game::getInstance()->states.back();
}
/*-<==>-----------------------------------------------------------------
/	Main destructor of the class
/----------------------------------------------------------------------*/
ControllerManager::~ControllerManager()
{
}
/*-<==>-----------------------------------------------------------------
/	Adds and  Entity to manual control (keyboard, mouse, pad...)
/----------------------------------------------------------------------*/
void ControllerManager::addtoControl(Entity* ent)
{
	Controller* inControl = new Controller(ent);
	inControl->currentLevel = currentLevel;
	inControl->from = ent;
	inControl->doing = -1;
	controllers.push_back(inControl);
	controllersByID[ent->name] = inControl;
	
}
/*-<==>-----------------------------------------------------------------
/	Adds an Entity to the artificial intelligence control
/----------------------------------------------------------------------*/
void ControllerManager::addtoAIControl(Entity* ent)
{
	Controller* inControl = new Controller(ent);
	inControl->doing = -1;
	controllers.push_back(inControl);
	controllersByID[ent->name] = inControl;
}
/*-<==>-----------------------------------------------------------------
/	Removes a certain entity (by it's ID) from the control list
/----------------------------------------------------------------------*/
void ControllerManager::removeControlled(std::string ID)
{
}
/*-<==>-----------------------------------------------------------------
/	Gets the controller for a certain ID
/----------------------------------------------------------------------*/
Controller * ControllerManager::getControlled(std::string ID)
{
	return controllersByID[ID];
}

/*-<==>-----------------------------------------------------------------
/	Updates all controlled entities
/----------------------------------------------------------------------*/
void ControllerManager::update(double elapsed_time)
{
	for each (Controller * oneController in controllers)
	{
		if (oneController->from->alive)
			oneController->run(elapsed_time);
		
	}
}
#include "Controller.h"
#include "game.h"

#include "Level1.h"

/*-<==>-----------------------------------------------------------------
/	Runs the actual task
/----------------------------------------------------------------------*/
Controller::Controller(){
	target = new Entity();
	newpos = Vector3(1.0, 1.0, 1.0);
	doing = -1;
	speed = 0;
	shootrate = timer1 = timer2 = rand() % 7;
	littleOrbit = false;
	greatOrbit = false;
	state = 0;
	last_doing = state;
	isChasing = false;
	recolectState = 0;
	current_shoot_channel = 2;
	auxEnt = NULL;

}
Controller::Controller(Entity* ent){
	target = new Entity();
	from = ent;
	newpos = Vector3(1.0, 1.0, 1.0);
	doing = -1;
	speed = 0;
	shootrate = timer1 = timer2 = rand() % 4;
	littleOrbit = false;
	greatOrbit = false;
	state = 0;
	isChasing = false;
	recolectState = 0;

	last_doing = state;
	auxEnt = NULL;
}

/*-<==>-----------------------------------------------------------------
/	Runs the actual task
/----------------------------------------------------------------------*/
void Controller::run(double elapsed_time){
	this->elapsed_time = elapsed_time;

	switch (doing){
	case 0:/* Manual*/	Manual();  break;
	case 1:/* Move */	iAMove(pos); break;
	case 2:/* Follow */ iAFollow(target); break;
	case 3:/* Shoot */	iAShoot(target); break;
	case 4:/* Attack */	iAAttack(target); break;
	case 5:/* Defend */ iADefend(); break;
	case 6:/* Repair */ iARepair(target); break;
	case 7:/* Recolect*/iARecolect(); break;
	case 8:/* Build */	iABuild(); break;
	case 9:/* Orbit */	iAOrbit(target); break;
	case 10:iASpaceAsteroid(); break;
	default: iAIdle(); break;
	}
}
/*-<==>-----------------------------------------------------------------
/	Moves according to keyboard, mouse, pad input
/----------------------------------------------------------------------*/
void Controller::Manual()
{
	Game * game = Game::getInstance();
	Vector3 dir = Vector3(0.0, 0.0, 0.0);
	Vector3 rot = Vector3(0.0, 0.0, 0.0);
	//JoyStick
	if (game->myJoystick1){
		//Xaxis	or Trust
		if (game->joystickstate.axis[1] != 0){
			dir = dir + Vector3(0.0, 0.0, -game->joystickstate.axis[1]);
		}
		//Yaxis	or Strafe
		//Zaxis	or height
		//Pitch or Tilt up||down
		if (game->joystickstate.axis[4] != 0){
			rot = rot + Vector3(game->joystickstate.axis[4] * DEG2RAD, 0.0, 0.0);
		}
		//Yaw or rotate left||right
		if (game->joystickstate.axis[0] != 0){
			rot = rot + Vector3(0.0, game->joystickstate.axis[0] * DEG2RAD, 0.0);
		}
		//Roll or Tilt left||right
		if (game->joystickstate.axis[3] != 0){
			rot = rot + Vector3(0.0, 0.0, -game->joystickstate.axis[3] * DEG2RAD);
		}

		//LockON
		iAProximity_Check("oposed", 2000);
		if (game->LockOn){
			if (isChasing == false){
				game->LockOn = false;
			}
		}
		else{
			isChasing = false;
			if (auxEnt){
				((EntityMovile*)auxEnt)->ischased = false;
			}
			isChasing = false;
		}
		//Shoot
		if (game->joystickstate.axis[2] != 0){
			if(shootrate < 0.0f){
				Vector3 pos = Vector3(from->model.m[12], from->model.m[13], from->model.m[14]);
				Vector3 vectorRandom; vectorRandom.random(Vector3(.3, .3, .3));
				BulletManager::getInstance()->createBullet(pos + vectorRandom, (from->model).frontVector()*(750), from->name);


				game->sfx[2] = BASS_SampleGetChannel(game->pew, false);
				BASS_ChannelPlay(game->sfx[2], true);

				shootrate = 0.05;
			}
		}
		



		shootrate -= elapsed_time;
		from->dir = dir;
		from->rot = rot;
		return;
	}
	//KEYBOARD----------------------------------------------------------------------------------------------
	//Increase Thrust
	if (game->keystate[SDL_SCANCODE_W]){
		dir = dir + Vector3(0.0, 0.0, 1);
	}

	//Decrease Thrust
	if (game->keystate[SDL_SCANCODE_S]){
		dir = dir + Vector3(0.0, 0.0, -1);
	}

	//Yaw
	//Strafe Right
	if (game->keystate[SDL_SCANCODE_D]){
		rot = rot + Vector3(0.0, DEG2RAD, 0.0);
	}

	//Strafe Left
	if (game->keystate[SDL_SCANCODE_A]){
		rot = rot + Vector3(0.0, -DEG2RAD, 0.0);
	}

	//Pitch
	if ((game->mouse_state & SDL_BUTTON_LEFT) || game->mouse_locked){
		double  angle = game->mouse_delta.y * DEG2RAD * 0.1;
		rot = rot + Vector3(-angle, 0.0, 0.0);
	}

	//Roll
	if ((game->mouse_state & SDL_BUTTON_LEFT) || game->mouse_locked)
	{
		double  angle = game->mouse_delta.x * DEG2RAD * 0.1;
		rot = rot + Vector3(0.0, 0.0, angle);
	}

	if (game->keystate[SDL_SCANCODE_SPACE]){
		if (shootrate < 0.0){
			Vector3 pos = Vector3(from->model.m[12], from->model.m[13], from->model.m[14]);
			Vector3 vectorRandom; vectorRandom.random(Vector3(.3, .3, .3));
			BulletManager::getInstance()->createBullet(pos + vectorRandom, (from->model).frontVector()*(750), from->name);
			shootrate = 0.15;
		}
		shootrate -= elapsed_time;
	}
	from->dir = dir;
	from->rot = rot;
}

// ----------------------------------------------------------------------------
// apply a given steering force to our momentum,
// adjusting our orientation to maintain velocity-alignment.
Vector3 Controller::velocity(void) { return from->model.frontVector() * from->dir.z; }

Vector3 Controller::flee(Vector3 loc){ return Vector3(); }
Vector3 Controller::avoidNeighbors(/*Neighbors*/){ return Vector3(); }
Vector3 Controller::Intercept(const Entity * ent){ return Vector3(); }

/*-<==>-----------------------------------------------------------------
/	Stays a certain amount of time idle
/----------------------------------------------------------------------*/
void Controller::iAIdle()
{
	from->dir = Vector3(0.0, 0.0, 0.0);
	from->rot = Vector3(0.0, 0.0, 0.0);
}
/*-<==>-----------------------------------------------------------------
/	Moves to a concrete position in universe
/----------------------------------------------------------------------*/
void Controller::iAMove(Vector3 loc)
{
	Vector3 entity_pos = from->model * Vector3();
	Vector3 target_pos = loc;

	Vector3 to_target = (target_pos - entity_pos);
	double distance = to_target.length();

	Vector3 speedAdjustedDir = (to_target - velocity());
	double adjustedDistance = (speedAdjustedDir.length());
	//Suponemos que nuestro objeto se encuentra N unidades desplazado hacia adelante para preveer la velocidad
	Vector3 unitDir = (speedAdjustedDir).normalize(); //to_target.normalize();

	float forwardness = from->model.frontVector().dot(unitDir);		 //Estamos por detras o por delante del objetivo?

	float azimuth = from->model.rightVector().dot(unitDir);
	float altitude = from->model.topVector().dot(unitDir);
	float roll = from->model.rightVector().dot(unitDir);

	azimuth = (forwardness >= 0.0) ? azimuth : (azimuth >= 0) ? azimuth + 1 : azimuth - 1;
	altitude = (forwardness >= 0.0) ? altitude : (altitude >= 0) ? altitude + 1 : altitude - 1;
	roll = (forwardness >= 0.0) ? roll : (roll >= 0) ? roll + 1 : roll - 1;

	float speed = 1;
	//Guardamos los parametros de direccion y rotacion obtenidos
	if (adjustedDistance < velocity().length() * 2 + 50){
		speed = sin(adjustedDistance / (velocity().length() * 2 + 50));			//Speed reduction in distance
		(speed < 0.0) ? speed = 0.0 : (speed > from->agi.z) ? from->agi.z : speed;	//Speed Clamp
	}

	from->dir = Vector3(0.0, 0.0, speed);
	from->rot = Vector3(altitude, -azimuth, roll)*elapsed_time;

}
void Controller::iAMove(Entity * ent)
{
	if (!ent->alive){
		doing = -1;
		isChasing = false;
		return;
	}
	Vector3 entity_pos = from->model * Vector3();
	Vector3 target_pos = ent->model * Vector3();

	Vector3 to_target = (target_pos - entity_pos);
	double distance = to_target.length();

	Vector3 speedAdjustedDir = (to_target - velocity());
	double adjustedDistance = (speedAdjustedDir.length() - from->getHalfSize() - ent->getHalfSize());

	//Suponemos que nuestro objeto se encuentra N unidades desplazado hacia adelante para preveer la velocidad
	Vector3 unitDir = (speedAdjustedDir).normalize();

	//Estamos por detras o por delante del objetivo?
	float forwardness = from->model.frontVector().dot(unitDir);

	//Tenemos que evitar el objetivo?
	//Vector3 toRadius //Aproximacion "chunga" de la tangente
	//float proximityness = from->model.frontVector();

	float azimuth = from->model.rightVector().dot(unitDir);
	float altitude = from->model.topVector().dot(unitDir);
	float roll = from->model.rightVector().dot(unitDir);

	azimuth = (forwardness >= 0.0) ? azimuth : (azimuth >= 0) ? azimuth + 1 : azimuth - 1;
	altitude = (forwardness >= 0.0) ? altitude : (altitude >= 0) ? altitude + 1 : altitude - 1;
	roll = (forwardness >= 0.0) ? roll : (roll >= 0) ? roll + 1 : roll - 1;



	float speed = 1;
	//Guardamos los parametros de direccion y rotacion obtenidos
	if (adjustedDistance < velocity().length() * 2 + 50){
		speed = sin(adjustedDistance / (velocity().length() * 2 + 50));			//Speed reduction in distance
		(speed < 0.0) ? speed = 0.0 : (speed > from->agi.z) ? from->agi.z : speed;	//Speed Clamp
	}
	from->dir = Vector3(0.0, 0.0, speed);
	from->rot = Vector3(altitude, -azimuth, 0.0)*elapsed_time;


}
/*-<==>-----------------------------------------------------------------
/	Orbit arround a certain distance
/----------------------------------------------------------------------*/
void Controller::iAOrbit(Entity* ent){
	if (!ent->alive || ent == from){
		doing = -1;
		isChasing = false;
		return;
	}
	iAcheckEvents(ent);
	Vector3 to_target = (from->model * Vector3() - ent->model * Vector3());

	switch (state){
	case 0:{iAAvoid(ent)/* iAMove(ent)*/; break; }//lejos -> acercarse
	case 1:	case 2: case 3:{ //cerca -> alejarse
				Vector3 farpos = ent->model * Vector3() + to_target * (210 + from->getHalfSize());
				iAAvoid(farpos);// iAMove(farpos);
				break;
	}
	}
	state = 0;
}
/*-<==>-----------------------------------------------------------------
/	Tracks and follows a moving position
/----------------------------------------------------------------------*/
void Controller::iAFollow(Entity* ent)
{
	if (!ent->alive || ent == from){
		doing = -1;
		isChasing = false;
		return;
	}
	target = ent;
	Vector3 entity_pos = from->model * Vector3();
	Vector3 target_pos = target->model * Vector3();

	Vector3 to_target = (target_pos - entity_pos);
	double distance = to_target.length();
	Vector3 speedAdjustedDir = (to_target - velocity());
	double adjustedDistance = distance - from->getHalfSize() - target->getHalfSize();

	Vector3 unitDir = (speedAdjustedDir).normalize();

	//Estimación de tiempo
	const float seekerToGoalTime = ((adjustedDistance < 0) ? 0 : (adjustedDistance / from->dir.z));
	const float maxPredictionTime = seekerToGoalTime * 0.9f;

	float parallelness = from->model.frontVector().dot(target->model.frontVector()); //Que tan vamos en la direccion del objetivo
	float forwardness = from->model.frontVector().dot(unitDir);					 //Estamos por detras o por delante del objetivo?

	double travelTime = distance / speed;

	int ford = (forwardness < -0.707) ? -1 : (forwardness > 0.707) ? 1 : 0;// (1 means parallel, 0 is pependicular, -1 is anti-parallel)
	int para = (parallelness < -0.707) ? -1 : (parallelness> 0.707) ? 1 : 0;// (1 means dead ahead, 0 is directly to the side, -1 is straight back)


	// Break the pursuit into nine cases, the cross product of the
	// quarry being [ahead, aside, or behind] us and heading
	// [parallel, perpendicular, or anti-parallel] to us.
	float timeFactor = 0; // to be filled in below
	switch (ford)
	{
	case +1:
		switch (para){
		case +1:    timeFactor = 4; break;      // ahead, parallel
		case 0:     timeFactor = 1.8f; break;     // ahead, perpendicular
		case -1:   	timeFactor = 0.85f; break;       // ahead, anti-parallel
		}
		break;
	case 0:
		switch (para){
		case +1:   timeFactor = 1; break;       // aside, parallel
		case 0:    timeFactor = 0.8f; break;       // aside, perpendicular
		case -1:   timeFactor = 4; break;       // aside, anti-parallel
		}
		break;
	case -1:
		switch (para){
		case +1:  timeFactor = 0.5f; break;        // behind, parallel
		case 0:    timeFactor = 2; break;       // behind, perpendicular
		case -1:    timeFactor = 2; break;      // behind, anti-parallel
		}
		break;
	}

	// estimated time until intercept of quarry
	const float et = travelTime * timeFactor;

	// xxx experiment, if kept, this limit should be an argument
	const float etl = (et > maxPredictionTime) ? maxPredictionTime : et;

	// estimated position of quarry at intercept
	Vector3 targetpos = target->model * Vector3() + (target->model.frontVector() * target->dir.z * etl);

	iAMove(target_pos);
}
/*-<==>-----------------------------------------------------------------
/	Shoots a target if possible
/----------------------------------------------------------------------*/
void Controller::iAShoot(Entity* ent)
{
	if (!ent->alive || ent == from){
		doing = -1;
		isChasing = false;
		return;
	}
	Vector3 entity_pos = from->model * Vector3();
	Vector3 target_pos = ent->model * Vector3();

	Vector3 to_target = (target_pos - entity_pos);
	double distance = to_target.length();

	Vector3 speedAdjustedDir = (to_target - velocity());
	double adjustedDistance = (speedAdjustedDir.length() - from->getHalfSize() - target->getHalfSize());

	//Suponemos que nuestro objeto se encuentra N unidades desplazado hacia adelante para preveer la velocidad
	Vector3 unitDir = (speedAdjustedDir).normalize();

	//Estamos por detras o por delante del objetivo?
	float forwardness = from->model.frontVector().dot(unitDir);
	if (forwardness > 0.90 && adjustedDistance < 50){
		if (shootrate < 0.0){
			Vector3 vectorRandom; vectorRandom.random(Vector3(.3, .3, .3));
			BulletManager::getInstance()->createBullet(entity_pos + vectorRandom, (from->model).frontVector()*(750 + from->dir.z), from->name);
			shootrate = 2.0;
		}
		shootrate -= elapsed_time;
	}
}
/*-<==>-----------------------------------------------------------------
/	Tracks and follows a target to shoot him
/----------------------------------------------------------------------*/
/*void Controller::iAAttack(Entity* ent)
{
//2 orbitas, close orbit and far orbit
target = ent;

Vector3 target_pos = Vector3(target->model.m[12], target->model.m[13], target->model.m[14]);
iAMove(target_pos);
iAShoot(ent);
}*/
void Controller::iAAttack(Entity* ent)
{
	if (!ent->alive || ent == from){
	
		Game* game = Game::getInstance();
		Level1 * level = (Level1*)(game->states.back());
		(from->Material == "alliedTeam") ? level->getChildren(" CarrierAllied") : level->getChildren("CarrierEnemy");
		doing = 5;
		isChasing = false;
		return;
	}
	iAcheckEvents(ent);
	Vector3 to_target = (ent->model * Vector3() - from->model * Vector3());

	switch (state){
	case 0:{ iAMove(ent); iAShoot(ent); break; }//lejos -> acercarse
	case 1: case 2: case 3:{ //cerca -> alejarse
				Vector3 farpos = ent->model * Vector3() - to_target * (210 + from->getHalfSize() + from->getHalfSize());
				iAMove(farpos);
				break;
		}
	}

}
/*-<==>-----------------------------------------------------------------
/	Stay at position waiting an enemy  coming into range, it can stay still or follow him
/----------------------------------------------------------------------*/
void Controller::iADefend()
{

	if (iAProximity_Check("oposed", 500)){
		iAAttack(auxEnt);
	}
	else
		iAOrbit(target);

}
/*-<==>-----------------------------------------------------------------
/	Repairs a certain target at range or seeks him in universe
/----------------------------------------------------------------------*/
void Controller::iARepair(Entity* ent)
{
	if (!ent->alive || ent == from){
		doing = -1;
		return;
	}
}
/*-<==>-----------------------------------------------------------------
/	Tries to find mineral and collect sources, returns it to its carrier
/----------------------------------------------------------------------*/
void Controller::iARecolect()
{
	//0	Look for an asteroid
	switch (recolectState){
	case 0:{
		if (iAProximity_Check("asteroids", 2000)){
			recolectState++;
		}
		break;
	}
	//1		iAmove(asteroid);
	case 1:{ 
		iAcheckEvents(auxEnt); 
		if (state > 0){
			recolectState++;
			from->dir = Vector3(0.0,0.0,0.0);
			timer2 = 3.0;
		}else
			iAMove(auxEnt);
		 break; 
	}
	//2	wait 3 seconds
	case 2:{ 
		if (timer2 <= 0){ 
			resources = 10;
			recolectState++; }
		else{ 
			timer2 -= elapsed_time;
		}
		break;
	}
	//3		iAmove(carrier);
	case 3:{ 
		iAcheckEvents(target);
		
		if (state > 0){
			from->dir = Vector3(0.0, 0.0, 0.0);

			recolectState++;
			timer2 = 3.0;
		}
		else{
			iAMove(target);
		}
			
		 break; 
	}
	//4		resources carrier = ship gattered;
	case 4:{
		if (timer2 <= 0){
			((EntityMovile*)target)->energy += 10; 
			resources = 0;
			recolectState = 0; 
		}
		else{
			timer2 -= elapsed_time;
		}
		break;
	}
	

	}
//	std::cout << "Recolect State: " << recolectState << std::endl;
	
	
	
	
	
	
	
	
}
/*-<==>-----------------------------------------------------------------
/	Tries to find mineral and collect sources, returns it to its carrier
/----------------------------------------------------------------------*/
void Controller::iAAvoid(Entity * ent)
{
	/*
	Game* game = Game::getInstance();
	Level1 * level = (Level1*)(game->states.back());

	std::vector<Entity*> * toCheck = &(level->children["enemyTeam"]);
	Vector3 entity_pos = from->model * Vector3();
	double from_HalfSize = abs(from->getHalfSize());
	Vector3 myAdjustedPOS = velocity();
	Vector3 unitDir;
	Vector3 correctedPos = Vector3(1.0, 1.0, 1.0);
	for (unsigned int i = 0; i < toCheck->size(); i++){
	Entity* to = toCheck->at(i);
	if (!to->alive) continue;
	Vector3 target_pos = to->model * Vector3();
	Vector3 hisAdjustedPOS = to->model.frontVector() * to->dir.z;
	Vector3 dir = (myAdjustedPOS - hisAdjustedPOS);

	float to_HalfSize = abs(to->getHalfSize());
	double adjustedDistance = (dir).length() - from_HalfSize - to_HalfSize;

	if (adjustedDistance < 0.0){//Se va a producir una colision! :O
	unitDir = dir.normalize();
	Vector3 idealPos = (unitDir * (from_HalfSize + to_HalfSize + to_HalfSize));
	correctedPos = correctedPos + idealPos;// Vector3(correctedPos.x + idealPos.x, correctedPos.y + idealPos.y, correctedPos.z + idealPos.z);
	}
	}

	toCheck = &(level->children["alliedTeam"]);
	entity_pos = from->model * Vector3();
	from_HalfSize = from->getHalfSize();
	myAdjustedPOS = velocity();
	for (unsigned int i = 0; i < toCheck->size(); i++){
	Entity* to = toCheck->at(i);
	if (!to->alive) continue;
	Vector3 target_pos = to->model * Vector3();
	Vector3 hisAdjustedPOS = to->model.frontVector() * to->dir.z;
	Vector3 dir = (myAdjustedPOS - hisAdjustedPOS);

	float to_HalfSize = to->getHalfSize();
	double adjustedDistance = (dir).length() - from_HalfSize - to_HalfSize;

	if (adjustedDistance < 0.0){//Se va a producir una colision! :O
	unitDir = dir.normalize();
	Vector3 idealPos = (unitDir * (from_HalfSize + to_HalfSize + to_HalfSize));
	correctedPos = correctedPos + idealPos;// Vector3(correctedPos.x + idealPos.x, correctedPos.y + idealPos.y, correctedPos.z + idealPos.z);
	}
	}

	if (correctedPos.x + correctedPos.y + correctedPos.z != 3.0){
	iAMove(entity_pos + correctedPos);
	return;
	}*/
	iAMove(ent);
}
void Controller::iAAvoid(Vector3 pos)
{
	/*
	Game* game = Game::getInstance();
	Level1 * level = (Level1*)(game->states.back());
	std::vector<Entity*> * toCheck = &(level->children["enemyTeam"]);
	Vector3 entity_pos = from->model * Vector3();
	double from_HalfSize = from->getHalfSize();
	Vector3 myAdjustedPOS = velocity();
	Vector3 unitDir;
	Vector3 correctedPos = Vector3(-1.0, -1.0, -1.0);
	for (unsigned int i = 0; i < toCheck->size(); i++){
	Entity* to = toCheck->at(i);
	if (!to->alive) continue;
	Vector3 target_pos = to->model * Vector3();
	Vector3 hisAdjustedPOS = to->model.frontVector() * to->dir.z;
	Vector3 dir = (myAdjustedPOS - hisAdjustedPOS);

	float to_HalfSize = to->getHalfSize();
	double adjustedDistance = (dir).length() - from_HalfSize - to_HalfSize;

	if (adjustedDistance < 0.0){//Se va a producir una colision! :O
	unitDir = dir.normalize();
	Vector3 idealPos = (unitDir * (from_HalfSize + to_HalfSize + to_HalfSize));
	correctedPos = idealPos;// Vector3(correctedPos.x + idealPos.x, correctedPos.y + idealPos.y, correctedPos.z + idealPos.z);
	}
	}

	toCheck = &(level->children["alliedTeam"]);
	entity_pos = from->model * Vector3();
	from_HalfSize = from->getHalfSize();
	myAdjustedPOS = velocity();
	for (unsigned int i = 0; i < toCheck->size(); i++){
	Entity* to = toCheck->at(i);
	if (!to->alive) continue;
	Vector3 target_pos = to->model * Vector3();
	Vector3 hisAdjustedPOS = to->model.frontVector() * to->dir.z;
	Vector3 dir = (myAdjustedPOS - hisAdjustedPOS);

	float to_HalfSize = to->getHalfSize();
	double adjustedDistance = (dir).length() - from_HalfSize - to_HalfSize;

	if (adjustedDistance < 0.0){//Se va a producir una colision! :O
	unitDir = dir.normalize();
	Vector3 idealPos = (unitDir * (from_HalfSize + to_HalfSize + to_HalfSize));
	correctedPos = correctedPos + idealPos;// Vector3(correctedPos.x + idealPos.x, correctedPos.y + idealPos.y, correctedPos.z + idealPos.z);
	}
	}

	if (correctedPos.x + correctedPos.y + correctedPos.z != 3.0){
	iAMove(entity_pos + correctedPos);
	return;
	}*/
	iAMove(pos);
}
bool Controller::iAProximity_Check(std::string team, float distance){
	if (isChasing)return true;
	if (timer1 > 0){
		timer1 -= elapsed_time;
		return false;
	}
	timer1 = 5.0;
	std::vector<std::vector<Entity*>> allChecks;
	Game* game = Game::getInstance();
	Level1 * level = (Level1*)(game->states.back());

	if (team == "oposed"){
		if (from->Material == "enemyTeam")allChecks.push_back((level->children["alliedTeam"]));
		else if (from->Material == "alliedTeam")allChecks.push_back((level->children["enemyTeam"]));
	}
	else if (team == "all"){
		allChecks.push_back((level->children["enemyTeam"]));
		allChecks.push_back((level->children["alliedTeam"]));
		allChecks.push_back((level->children["asteroids"]));
	}
	else if (team == "enemyTeam"){ allChecks.push_back((level->children["enemyTeam"])); }
	else if (team == "alliedTeam"){ allChecks.push_back((level->children["enemyTeam"])); }
	else if (team == "asteroids") { allChecks.push_back((level->children["asteroids"])); }

	std::vector<Entity*>result;
	int found = 0.0;
	Vector3 from_pos = from->model * Vector3();
	for each (std::vector<Entity*> toCheck in allChecks)
	{
		if (found >= 20)break;
		for each (Entity* to in toCheck)
		{
			if (!to->alive)  continue;
			if (((EntityMovile*)to)->ischased){
				int a = 1;
				if (rand() % 4 > 1){ continue; }
			}
			double dist = (to->model * Vector3() - from_pos).length();
			if (dist < distance){
				result.push_back(to);
				found++;
			}
		}
	}
	if (found>0){
		auxEnt = result[rand()%result.size()];//Hacer que sea uno random
		((EntityMovile*)auxEnt)->ischased = true;
		isChasing = true;
		return  true;
	}
	return false;
}
/*-<==>-----------------------------------------------------------------
/	Builds the desired item
/----------------------------------------------------------------------*/
void Controller::iABuild()
{}
/*-<==>-----------------------------------------------------------------
/	Builds the desired item
/----------------------------------------------------------------------*/
void Controller::iAcheckEvents(Entity * ent)
{
	if (!ent->alive || ent == from){
		doing = -1;
		isChasing = false;
		return;
	}

	Vector3 entity_pos = from->model * Vector3();
	Vector3 target_pos = ent->model * Vector3();

	float from_radius = from->getHalfSize();
	float to_radius = ent->getHalfSize();

	Vector3 to_target = (target_pos - entity_pos);
	double distance = to_target.length();

	//Vector3 speedAdjustedDir = (to_target - velocity());
	double adjustedDistance = (distance - from_radius - to_radius);

	//Estado 0 : estamos lejos
	if (adjustedDistance > 200.0){
		state = 0;
		return;
	}

	//Estado 1 : cerca del objetivo (1: en direccion a el, 2: exactamente paralelo a este, 3: nos lo hemos pasado de largo)
	if (adjustedDistance < 25.0){
		Vector3 unitDir = (to_target).normalize();
		float forwardness = from->model.frontVector().dot(unitDir);		 //Estamos por detras o por delante del objetivo?
		(forwardness > 0) ? state = 1 : (forwardness == 0) ? state = 2 : state = 3;//estado 4: shootable
	}


}
void wormhole(){

	/*
	//Hace una cosa MUY MUY wapa XDDDDD tipo wormhole, podria servir para hacer algun efecto de las particulas
	// cambiar la linea del codigo (from->dir....) por la de aqui
	double speed = 0;
	if (adjustedDistance < velocity().length()){
	speed = adjustedDistance / velocity().length();
	}
	else{ speed = speedAdjustedDir.length(); }
	from->dir = Vector3(0.0, 0.0, 5*speed*elapsed_time);
	*/
}


/*
Vector3 rotation = _cam->eye - _cam->center;rotation = (rotation).rotateInAxis(elapsed_time * 0.01 * M_PI, Vector3(0.0, 1.0, 0.0));
Vector3 unitDir = rotation.normalize();
double distance = rotation.length();
*/

void Controller::iASpaceAsteroid(){
	Vector3 from_pos = from->model*Vector3();
	Vector3 orbit_center = this->pos;

	Vector3 rotation = from_pos - orbit_center; rotation = (rotation).rotateInAxis(elapsed_time *0.01 * M_PI, Vector3(0.0, 1.0, 0.0));
	Vector3 dir = (orbit_center + rotation) - from_pos;
	from->move(dir);
	//from->model.setTranslation(from_pos.x*0.75 + rotation.x*0.25, from_pos.y*0.75 + rotation.y*0.25, from_pos.z*0.75 + rotation.z*0.25);
	//from->model.traslateLocal;
	//Vector3 unitDir = rotation.normalize();
	//double distance = rotation.length();

}
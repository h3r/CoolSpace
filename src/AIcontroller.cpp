#include "AIcontroller.h"


AIcontroller::AIcontroller()
{
	EntityID = ID;
	speed = 0;

	//provocara un brinco, probablemente, es para suavizar el mouse, habria que inicializarlo para que sea igual que el mouse.
	smoothX = 0;
	smoothY = 0;
}


AIcontroller::~AIcontroller()
{
}


void Controller::setPlayer(std::string ID){
	EntityID = ID;
}

void Controller::setTarget(std::string ID){
	TargetID = ID;

}

void Controller::update(double elapsed_time){
	Game* game = Game::getInstance();
	Entity *playable = game->world->getChildren(EntityID);

	for (unsigned int i = 0; i < playable->children.size(); i++){
		playable->children[i]->update(elapsed_time);
	}

	mouseSmooth(elapsed_time, game->mouse_delta.y, game->mouse_delta.x);

	//Control del Mesh
	if (game->myJoystick1){//JOYSTICK------------------------------------------------------------------
		//Thrust
		if (Game::getInstance()->joystickstate.axis[1] != 0){
			speed = elapsed_time * -33 * game->joystickstate.axis[1];
			if (speed < 0) speed = 0;
			playable->move(0, 0, speed);
		}

		//Strafe Left/Right
		if (game->joystickstate.axis[0] != 0){
			playable->rotate((200 * elapsed_time * game->joystickstate.axis[0]) * DEG2RAD, Vector3(0, 1, 0));
		}
		//Tilt Up/Down
		if ((game->mouse_state & SDL_BUTTON_LEFT) || game->mouse_locked){
			playable->rotate((200 * elapsed_time * game->joystickstate.axis[4]) * DEG2RAD, Vector3(1, 0, 0));
			//rotate(tehGame->mouse_delta.y * 0.0005, Vector3(-1, 0, 0)); 
		}

		//Tilt Left/Right
		if ((game->mouse_state & SDL_BUTTON_LEFT) || game->mouse_locked){
			playable->rotate((200 * elapsed_time * game->joystickstate.axis[3]) * DEG2RAD, Vector3(0, 0, -1));
			//rotate(tehGame->mouse_delta.x * 0.0005, Vector3(0, 0, 1)); 
		}

		//SHOT

		if (game->joystickstate.axis[2] != 0){
			Vector3 pos = Vector3(playable->model.m[12], playable->model.m[13], playable->model.m[14]);
			Vector3 vectorRandom; vectorRandom.random(Vector3(.3, .3, .3));
			game->world->bullets.createBullet(pos + vectorRandom, (playable->model).frontVector()*(750 + speed), playable->name);
		}
		if (game->keystate[SDL_SCANCODE_SPACE]){
			Vector3 pos = Vector3(playable->model.m[12], playable->model.m[13], playable->model.m[14]);
			Vector3 vectorRandom; vectorRandom.random(Vector3(.3, .3, .3));
			game->world->bullets.createBullet(pos + vectorRandom, (playable->model).frontVector()*(750 + speed), playable->name);
		}
	}
	//KEYBOARD----------------------------------------------------------------------------------------------
	//Increase Thrust
	if (game->keystate[SDL_SCANCODE_W]){
		if (speed < 1){ speed = speed + 0.01; }
	}

	//Decrease Thrust
	if (game->keystate[SDL_SCANCODE_S]){
		if (speed > 0){ speed = speed - 0.01; }
		if (speed < 0){ speed = 0; }
	}
	playable->move(0, 0, elapsed_time * 100 * speed);
	//Strafe Right
	if (game->keystate[SDL_SCANCODE_D])
	{
		playable->rotate(1 * DEG2RAD, Vector3(0, 1, 0));
	}

	//Strafe Left
	if (game->keystate[SDL_SCANCODE_A])
	{
		playable->rotate(-1 * DEG2RAD, Vector3(0, 1, 0));
	}

	//Tilt Up/Down
	if ((game->mouse_state & SDL_BUTTON_LEFT) || game->mouse_locked)
	{
		//playable->rotate(game->mouse_delta.y * 0.1 * elapsed_time, Vector3(-1, 0, 0));
		playable->rotate(smoothX * 0.1 * elapsed_time, Vector3(-1, 0, 0));
	}

	//Tilt Left/Right
	if ((game->mouse_state & SDL_BUTTON_LEFT) || game->mouse_locked)
	{
		//playable->rotate(game->mouse_delta.x * 0.1 * elapsed_time, Vector3(0, 0, 1));
		playable->rotate(smoothY * 0.1 * elapsed_time, Vector3(0, 0, 1));

		//playable->rotate(smoothY * -0.1 * elapsed_time, Vector3(0, 1, 0));
	}

	if (game->keystate[SDL_SCANCODE_SPACE]){
		Vector3 pos = Vector3(playable->model.m[12], playable->model.m[13], playable->model.m[14]);
		Vector3 vectorRandom; vectorRandom.random(Vector3(.3, .3, .3));
		game->world->bullets.createBullet(pos + vectorRandom, (playable->model).frontVector()*(750 + speed), playable->name);
	}
}

/////////////metodo-utilidad para suavizar el mouse

void Controller::mouseSmooth(double ds, float mX, float mY){
	double d = 1.0 - exp(log(0.5)*softening*ds);

	smoothX += (mX - smoothX)*d;
	smoothY += (mY - smoothY)*d;
}


#include "Pause.h"
#include "game.h"

//states
#include "Menu.h"


Pause::Pause(){};
Pause::~Pause(){};

void Pause::init(){

	Game* game = Game::getInstance();
	game->sfx[1] = BASS_SampleGetChannel(game->select, false);
	BASS_ChannelPlay(game->sfx[1], true);

	_cam = new Camera();
	_cam->setPerspective(70, WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.001, 100000);
	_cam->eye = Vector3(1000, 1000, 1000);
	_cam->up = Vector3(0.0, 1.0, 0.0);
	_cam->center = Vector3(0.0, 0.0, 0.0);

	_cam2d = new Camera();
	_cam2d->setOrthographic(-_cam->aspect, _cam->aspect, 1, -1, -1, 1);


};
void Pause::clean(){
	delete this;
};

void Pause::update(double elapsed_time){

	Game* game = Game::getInstance();

	if (game->dSTART){
		BASS_ChannelPlay(game->sfx[1], true);

		Game::getInstance()->popState();
		return;
	}
	else if (game->dCROSS){
		BASS_ChannelPlay(game->sfx[1], true);
		BASS_ChannelStop(game->sfx[0]);
		State* menu = new Menu();
		Game::getInstance()->changeState(menu);
		return;
	}

	
};

void Pause::render(Camera* cam){
	_cam->set();


	

	//glColor4f(screenColor.x, screenColor.y, screenColor.z, screenColor.a);
	printw(Vector3(0.0,0.0,0), "PAUSE" );
	printw(Vector3(-100.0,-300.0, 0.0), "press X to exit to the main screen"); 
	printw(Vector3(-100.0, -400.0,0.0), "press start to unpause"); 
	

	




	

};

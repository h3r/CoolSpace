#include "HowToPlay.h"
#include "game.h"

//states
#include "Menu.h"


HowToPlay::HowToPlay(){};
HowToPlay::~HowToPlay(){};

void HowToPlay::init(){

	_cam = new Camera();
	_cam->setPerspective(70, WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.001, 100000);
	_cam->eye = Vector3(1.0, 1000, 1.0);
	_cam->up = Vector3(0.0, 1.0, -1.0);
	_cam->center = Vector3(0.0, 0.0, 0.0);

	_cam2d = new Camera();
	_cam2d->setOrthographic(-_cam->aspect, _cam->aspect, 1, -1, -1, 1);

	screenColor = Vector4(1.0, 1.0, 1.0, 0.0);
	panelColor = Vector4(1.0, 1.0, 1.0, 0.0);

	shouldExit = false;
	
	screen = new EntityMesh();
	screen->texture = Texture::LOAD("assets/UI/howtoplay.tga");
	screen->mesh = new Mesh(); screen->mesh->createPlane(100);	screen->mesh->createBillboard(screen->texture->width/2, screen->texture->height/2, Vector4(0.0, 1.0, 0.0, -1.0));
	screen->model.m[12] = 0.00;
	screen->model.m[13] = 0.00;
	screen->model.m[14] = 1.0;
	
};
void HowToPlay::clean(){
	delete screen;
	delete this;
};

void HowToPlay::update(double elapsed_time){

	Game* game = Game::getInstance();

	if (game->dCROSS || game->dSTART){
		game->popState();
		return;
	}


	//underState->update(elapsed_time);
	
};

void HowToPlay::render(Camera* cam){
	Game* game = Game::getInstance();

	//glDisable(GL_CULL_FACE);


	
	//underState->render(_cam);

	_cam->set();

	//glDepthMask(GL_FALSE);
	//glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glColor4f(1.0,1.0,1.0,1.0);

	screen->render();


	


	//glColor4f(screenColor.x, screenColor.y, screenColor.z, screenColor.a);
	


	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
	
	glEnable(GL_CULL_FACE);

};

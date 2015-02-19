#include "gameOver.h"
#include "game.h"

//states
#include "Menu.h"


gameOver::gameOver(){};
gameOver::~gameOver(){};

void gameOver::init(){

	Game* game = Game::getInstance();
	game->sfx[1] = BASS_SampleGetChannel(game->transition, false);
	BASS_ChannelStop(game->sfx[0]);
	game->sfx[0] = BASS_SampleGetChannel(game->over, false);
	BASS_ChannelPlay(game->sfx[0], true);

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
	screen->mesh = new Mesh(); screen->mesh->createPlane(100);	screen->mesh->createBillboard(WINDOW_WIDTH, WINDOW_HEIGHT, Vector4(0.0, 1.0, 0.0, -1.0));
	screen->texture = Texture::LOAD("assets/cutscenes/over.tga");
	
};
void gameOver::clean(){
	delete screen;
	delete this;
};

void gameOver::update(double elapsed_time){

	Game* game = Game::getInstance();


	if (shouldExit){
		screenColor.a -= elapsed_time*0.25;
		panelColor.a = screenColor.a;
		if (screenColor.a <= 0){
				
				game->changeState((State*)new Menu());
				return;
			}
			
	}
	else {
		if (screenColor.a<1.0){screenColor.a += elapsed_time*0.50;
	}
		if (screenColor.a > 0.99){
			panelColor.a += elapsed_time*0.5;
		}
		if (game->dSTART || game->dSELECT || game->dCROSS || game->dCIRCLE || game->dSQUARE || game->dTRIANGLE){
			BASS_ChannelPlay(game->sfx[1], true);
			BASS_ChannelStop(game->sfx[0]);
			shouldExit = true;
		}
	}

	
};

void gameOver::render(Camera* cam){
	_cam->set();

	//glDisable(GL_CULL_FACE);
	glDepthMask(GL_TRUE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	
	glColor4f(screenColor.x, screenColor.y, screenColor.z, screenColor.a);


	screen->render();


	//glColor4f(screenColor.x, screenColor.y, screenColor.z, screenColor.a);
	
	printw(Vector3(0.01, 0.01, 0.01), "LOADING %d", 1);


	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
	
	glEnable(GL_CULL_FACE);

};

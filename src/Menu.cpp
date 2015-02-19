#include "Menu.h"
#include "HowToPlay.h"

Menu::Menu(){};
Menu::~Menu(){};

void Menu::init(){

	screenColor = Vector4(1.0, 1.0, 1.0, 0.0);
	panelColor = Vector4(1.0, 1.0, 1.0, 0.0);

	skybox = new EntityMesh();
	skybox->mesh = Mesh::LOAD("assets/mesh/space_cubemap"); skybox->texture = Texture::LOAD("assets/mesh/space_cubemap.tga");
	//skybox->scale(5.0, 5.0,5.0);
	panelMenu = new EntityMesh();
	panelMenu->mesh = new Mesh(); panelMenu->mesh->createPlane(0.06); panelMenu->texture = Texture::LOAD("assets/UI/menu.tga");
	
	Entity* place0= new Entity();
	//place0->mesh = new Mesh(); place0->mesh->createPlane(0.001); place0->texture = Texture::LOAD("assets/UI/target.tga");
	//place0->move(0.01, -0.015, -0.045);//panelMenu->model*Vector3());
	place0->model.traslateLocal(0.01, 0.01, -0.045);
	panelMenu->addChild(place0);

	place0 = new Entity();
	//place0->mesh = new Mesh(); place0->mesh->createPlane(0.001); place0->texture = Texture::LOAD("assets/UI/target.tga");
	place0->model.traslateLocal(0.01, 0.01, -0.036);
	panelMenu->addChild(place0);

	place0 = new Entity();
	//place0->mesh = new Mesh(); place0->mesh->createPlane(0.001); place0->texture = Texture::LOAD("assets/UI/target.tga");
	place0->model.traslateLocal(0.01, 0.01, -0.027);
	panelMenu->addChild(place0);

	place0 = new Entity();
	//place0->mesh = new Mesh(); place0->mesh->createPlane(0.001); place0->texture = Texture::LOAD("assets/UI/target.tga");
	place0->model.traslateLocal(0.01, 0.01, -0.018);
	panelMenu->addChild(place0);

	place0 = new Entity();
	//place0->mesh = new Mesh(); place0->mesh->createPlane(0.001); place0->texture = Texture::LOAD("assets/UI/target.tga");
	place0->model.traslateLocal(0.0397, 0.01, -0.055);
	panelMenu->addChild(place0);

	EntityMesh* flecha = new EntityMesh();
	flecha->mesh = new Mesh(); flecha->mesh->createPlane(0.0006); flecha->texture =  NULL ; Texture::LOAD("assets/UI/target.tga");
	panelMenu->addChild(flecha);

	currentOption = 3;
	difficulty = 0;
	level = 0;

	_cam = new Camera();
	_cam->setPerspective(70, WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.001, 100000);
	_cam->eye = Vector3(1000, 1000, 1000);
	_cam->up = Vector3(0.0, 1.0, 0.0);
	_cam->center = Vector3(0.0, 0.0, 0.0);

	_cam2d = new Camera();
	_cam2d->setOrthographic(-_cam->aspect, _cam->aspect, 1, -1, -1, 1);
	_cam2d->left = 0;
	_cam2d->right = 0;
	game = Game::getInstance();
	game->sfx[0]= BASS_SampleGetChannel(game->menuMusic, false);
	game->sfx[1]= BASS_SampleGetChannel(game->select, false);
	game->sfx[2] = BASS_SampleGetChannel(game->transition, false);
	
	BASS_ChannelPlay(game->sfx[0], true);
	


	shouldExit = false;



};
void Menu::clean(){
	delete skybox;
	//delete flecha;
	delete panelMenu;
	

	delete this; };

void Menu::update(double elapsed_time){

	if (_cam){
		//Posicionar la camara
		Vector3 rotation = _cam->eye - _cam->center;rotation = (rotation).rotateInAxis(elapsed_time * 0.01 * M_PI, Vector3(0.0, 1.0, 0.0));
		Vector3 unitDir = rotation.normalize();
		double distance = rotation.length();
		
		Vector3 eye = _cam->center + (unitDir *  distance);
		_cam->lookAt(eye, _cam->center, _cam->up);

		//Posicionar el skybox respecto al eye
		skybox->model.setTranslation(_cam->eye.x, _cam->eye.y, _cam->eye.z);
		
		//Posicionar el menu respecto a la camara
		Vector3 neareye = _cam->center + (unitDir *  (distance - 0.11));

		panelMenu->rotateToVecAngle(unitDir,0.0);
		panelMenu->model.traslate(neareye.x, neareye.y, neareye.z);
		
	}

	//skybox->rotate(elapsed_time*0.1, elapsed_time*0.2, elapsed_time*0.2);
	
	if (shouldExit){
		screenColor.a -= elapsed_time*0.25;
		panelColor.a = screenColor.a;
		if (screenColor.a <= 0){
			if (currentOption == 0){
				game->quit();
			}
			else if(currentOption==3){
				Level1* levelState = new Level1();
				//difficulty
				levelState->s_difficulty = difficulty;
				levelState->s_level = level;
				game->changeState((State*)levelState);
				
				return;
				//set up gameState using level and difficulty and switch
			}
		}
	}
	else if (game->states.back()==this){

		if (screenColor.a < 1.0){
			screenColor.a+=elapsed_time;
		}
		else{
			panelColor.a+=elapsed_time;
		}

		//control del menu
		if (currentOption == 3){

			if (game->dRIGHT){
				BASS_ChannelPlay(game->sfx[1], true);
				level++;
				if (level > 2){
					level = 0;
				}
			}
			else if (game->dLEFT){
				BASS_ChannelPlay(game->sfx[1], true);
				level--;
				if (level < 0){
					level = 2;
				}
			}

		}

		else if (currentOption == 2){

			if (game->dRIGHT){
				BASS_ChannelPlay(game->sfx[1], true);
				difficulty++;
				if (difficulty > 2){
					difficulty = 0;
				}
			}
			else if (game->dLEFT){
				BASS_ChannelPlay(game->sfx[1], true);
				difficulty--;
				if (difficulty < 0){
					difficulty = 2;
				}
			}
		}

		if (game->dUP){
			BASS_ChannelPlay(game->sfx[1], true);
			currentOption++;
			if (currentOption > 3){
				currentOption = 0;
			}
		}
		if (game->dDOWN){
			BASS_ChannelPlay(game->sfx[1], true);
			currentOption--;
			if (currentOption < 0){
				currentOption = 3;
			}
		}

		if (game->dCROSS || game->dSTART){
			if (currentOption == 0 || currentOption==3){
				shouldExit = true;
				BASS_ChannelStop(game->sfx[0]);
				BASS_ChannelPlay(game->sfx[2], true);

			}
			if (currentOption == 1){

				BASS_ChannelPlay(game->sfx[1], true);

				HowToPlay* howTo = new HowToPlay();
				howTo->underState = this;
				game->pushState(howTo);
				return;
			}
		}
	}
	//std::cout << currentOption << std::endl;

	//std::cout << panelMenu->children.size() << std::endl;
	//updateamos flecha
	panelMenu->children[5]->model = panelMenu->children[currentOption]->model;
	panelMenu->children[5]->model.m[12]+=0.0025;
	panelMenu->children[5]->model.m[14]+= 0.0005125;
		
};


void Menu::render(Camera* cam){
	_cam->set();
	//_cam = cam;

	glDepthMask(GL_TRUE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//AQUI SE RENDERISA EL FONDO
	glColor4f(screenColor.x, screenColor.y, screenColor.z, screenColor.a);

	glDisable(GL_DEPTH_TEST);
	skybox->render();
	glEnable(GL_DEPTH_TEST);

	glColor4f(panelColor.x, panelColor.y, panelColor.z, panelColor.a);

	
	//A PARTIR DE AQUI SE RENDERIzA EL MENU
	//_cam2d->set();
	
	panelMenu->render();
	
	std::string dificultad;
	if (difficulty == 0){
		dificultad = "Newbie";
	}
	else if (difficulty == 1){
		dificultad = "Intermedium";
	}
	else if (difficulty == 2){
		dificultad = "Extra Crispy";
	}
	printw(panelMenu->children[3]->getGlobalMatrix()*Vector3(), "< Level: %d >", level+1);
	printw(panelMenu->children[2]->getGlobalMatrix()*Vector3(), "< DIFFICULTY: %s >", &dificultad);
	printw(panelMenu->children[1]->getGlobalMatrix()*Vector3(), "HOW TO PLAY");
	printw(panelMenu->children[0]->getGlobalMatrix()*Vector3(), "EXIT" );

	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
};

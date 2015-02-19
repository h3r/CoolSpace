#pragma once

#include "game.h"

#include "UIManager.h"
#include "Particle.h"
#include "Emitter.h"

//Singleton init
Game* Game::instance = NULL;

//some globals
Shader* shader = NULL;
int last = 0;

EntityMovile* player= NULL;
EntityMesh* skybox = NULL;

std::vector<std::string> cameras;

//Joystick

JoystickState getJoystickState(SDL_Joystick* joystick)
{
	JoystickState state;
	memset(&state, 0, sizeof(JoystickState)); //clear

	if (joystick == NULL)
	{
		//std::cout << "Error: Joystick not opened" << std::endl;
		return state;
	}

	state.num_axis = SDL_JoystickNumAxes((::SDL_Joystick*) joystick);
	state.num_buttons = SDL_JoystickNumButtons((::SDL_Joystick*)joystick);

	if (state.num_axis > 8) state.num_axis = 8;
	if (state.num_buttons > 16) state.num_buttons = 16;

	for (int i = 0; i < state.num_axis; ++i) //axis
		state.axis[i] = SDL_JoystickGetAxis((::SDL_Joystick*) joystick, i) / 32768.0f; //range -32768 to 32768
	for (int i = 0; i < state.num_buttons; ++i) //buttons
		state.button[i] = SDL_JoystickGetButton((::SDL_Joystick*) joystick, i);
	state.hat = (HATState)(SDL_JoystickGetHat((::SDL_Joystick*) joystick, 0) - SDL_HAT_CENTERED); //one hat is enough

	return state;
}
SDL_Joystick* openJoystick(int num_joystick)
{
	// Initialize the joystick subsystem
	SDL_InitSubSystem(SDL_INIT_JOYSTICK);
	

	// Check for number of joysticks available
	if (SDL_NumJoysticks() <= num_joystick)
		return NULL;

	// Open joystick and return it
	return SDL_JoystickOpen(num_joystick);
}

Game::Game()
{
	// initialize attributes
	// Warning: DO NOT CREATE STUFF HERE, USE THE INIT 
	// things create here cannot access opengl
	window_width = WINDOW_WIDTH;
	window_height = WINDOW_HEIGHT;

	keystate = NULL;
	mouse_locked = true;
	gamestate = 0;

	sfx.clear();
	current = 0;
	enforcedMapView = false;
	LockOn = false;
}
void Game::setWindow(SDL_Window* awindow){
	window = awindow;
}

float erand48(unsigned short xsubi[3]){
	return (float)((float)rand() / (float)RAND_MAX);
}
//Here we have already GL working, so we can create meshes and textures
void Game::init(void){

	//basic stuff that all states will use should be init here
	//like cameras
	camera = new Camera();
	camera->setPerspective(70, WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.01, 100000);
	camera->width = WINDOW_WIDTH;
	camera->height = WINDOW_HEIGHT;

	cameraMiniMap = new Camera();
	camera->setPerspective(70, WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.01, 100000);
	camera->width = WINDOW_WIDTH;
	camera->height = WINDOW_HEIGHT;

	//or audio channels,samples
	BASS_Init(1, 44100, 0, 0, NULL);
	sfx.push_back(HCHANNEL());
	sfx.push_back(HCHANNEL());
	sfx.push_back(HCHANNEL());
	sfx.push_back(HCHANNEL());
	sfx.push_back(HCHANNEL());
	sfx.push_back(HCHANNEL());
	sfx.push_back(HCHANNEL());
	sfx.push_back(HCHANNEL());
	sfx.push_back(HCHANNEL());
	sfx.push_back(HCHANNEL());

	//and control things
	gUP = false;
	gDOWN = false;
	gLEFT = false;
	gRIGHT = false;
	gSTART = false;
	gSELECT = false;
	dUP = false;
	dDOWN = false;
	dLEFT = false;
	dRIGHT = false;
	dSTART = false;
	dSELECT = false;

	std::cout << "initiating game..." << std::endl;
	pushState(new Loader());
	
	map = menu = false;
	gui = true;
	
	/*
	//OpenGL flags
	glEnable( GL_CULL_FACE ); //render both sides of every triangle
	glEnable( GL_DEPTH_TEST ); //check the occlusions using the Z buffer
	
	world = new World();

	//create our camera
	camera = new Camera();
	camera->setPerspective(70,WINDOW_WIDTH/(float)WINDOW_HEIGHT,0.01,100000); //set the projection, we want to be perspective
	distanceToCamera = 1.0;
	
	//create our 2d camera
	camera2D = new Camera();
	camera2D->setOrthographic(-1, 1, 1, -1, -1, 1);

	cameraMAP = new Camera(); //our big MAP camera
	cameraMAP->setPerspective(70, WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.01, 100000); //set the projection, we want to be perspective

	cameraMiniMap = new Camera(); //our minimap camera
	cameraMiniMap->setPerspective(70, WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.01, 100000); //set the projection, we want to be perspective

	//Setting up the ui
	world->ui =new UIManager(300, camera);
	world->ui->loadTexture("Mapa");
	world->ui->loadTexture("marker");
	world->ui->loadTexture("minimap");
	world->ui->loadTexture("minimap2");
	world->ui->loadTexture("target");
	world->ui->loadTexture("panel");

	EntityMesh* panel = new EntityMesh();
		panel->texture = Texture::LOAD("assets/UI/panel.tga");
		panel->mesh = new Mesh();panel->mesh->createPlane(100);
		panel->name = "panel";
		panel->model.setTranslation(0.0, 0.0, 0.0);

		EntityMesh* Shield = new EntityMesh();
		Shield->texture = Texture::LOAD("assets/UI/shield.tga");
		Shield->mesh = new Mesh(); Shield->mesh->createPlane(100);
		Shield->name = "shield";
		Shield->model.setTranslation(0.0, 5, 0.0);
		panel->addChild(Shield);

		EntityMesh* Hull = new EntityMesh();
		Hull->texture = Texture::LOAD("assets/UI/hull.tga");
		Hull->mesh = new Mesh(); Hull->mesh->createPlane(100);
		Hull->name = "hull";
		Hull->model.setTranslation(0.0, 3, 0.0);
		panel->addChild(Hull);

		EntityMesh* Fuel = new EntityMesh();
		Fuel->texture = Texture::LOAD("assets/UI/fuel.tga");
		Fuel->mesh = new Mesh(); Fuel->mesh->createPlane(100);
		Fuel->name = "fuel";
		Fuel->model.setTranslation(0.0, 1, 0.0);
		panel->addChild(Fuel);

	world->addChild(panel, "UI");


//	world->particles = Texture::LOAD("assets/particles/mySpaceAtlas.tga");
//	Atlas* atlas = new Atlas("assets/particles/mySpaceAtlas.txt", world->particles);
//	world->pManager = new ParticleManager(20000, atlas, camera);

	//hide the cursor
	SDL_ShowCursor(!mouse_locked); //hide or show the mouse
	
	player = new EntityMovile();
	player->mesh = Mesh::LOAD("assets/mesh/LightSpaceShip"); player->texture = Texture::LOAD("assets/mesh/LightSpaceShip.tga");
	player->model.setTranslation(0, 0, 0);
	player->name = "player1";

	EntityMovile * CarrierAlied  = new EntityMovile();
		CarrierAlied->mesh = Mesh::LOAD("assets/mesh/frigate"); CarrierAlied->texture = Texture::LOAD("assets/mesh/frigate.tga");
		CarrierAlied->model.setTranslation(-750, -200, -750);
		CarrierAlied->name = "CarrierAlied";
	
	EntityMovile * CarrierEnemy = new EntityMovile();
		CarrierEnemy->mesh = Mesh::LOAD("assets/mesh/frigate"); CarrierEnemy->texture = Texture::LOAD("assets/mesh/frigate.tga");
		CarrierEnemy->model.setTranslation(750, 200, 750);
		CarrierEnemy->name = "CarrierEnemy";
		

	skybox = new EntityMesh();
		skybox->mesh = Mesh::LOAD("assets/mesh/space_cubemap");		skybox->texture = Texture::LOAD("assets/mesh/space_cubemap.tga");
		skybox->name = "skybox";
		skybox->model.setTranslation(0, 0, 0);
	
	
	world->addChild(CarrierAlied);
	world->addChild(CarrierEnemy);
	world->addChild(player, "player");
	world->addChild(skybox, "skybox");
	world->controllers.getControlled(player->name)->doing = 0;
	world->controllers.getControlled(player->name)->target = CarrierAlied;
	world->controllers.getControlled(player->name)->pos = CarrierAlied->model*Vector3();
	world->controllers.getControlled(CarrierAlied->name)->doing = 2;
	world->controllers.getControlled(CarrierAlied->name)->target = player;
	
	//Some asteroids
	for (int i = 0; i < 10; i++){
		EntityMovile* asteroid = new EntityMovile();
		
		asteroid->mesh = Mesh::LOAD("assets/mesh/asteroides");				asteroid->texture = Texture::LOAD("assets/mesh/asteroide.tga");
		asteroid->mesh_low = Mesh::LOAD("assets/mesh/asteroide1");	asteroid->texture_low = Texture::LOAD("assets/mesh/asteroide.tga");

			Vector3 vectorRandom; vectorRandom.random(Vector3(1, 1, 1)); vectorRandom = vectorRandom + Vector3(1.0, 1.0, 1.0); //Rango entre 0 y 2
			float azimuth, altitude;
			azimuth = vectorRandom.x * M_PI;
			altitude = vectorRandom.y * M_PI;
			float x, y, z, r;
			r = 5000;
			x = r * cos(azimuth);
			y = r * sin(azimuth);
			z = r * cos(altitude);

		asteroid->model.setTranslation((x-1), vectorRandom.y, (z-1));
		world->addChild(asteroid);
	}

	//Some Alied
	for (int i = 0; i < 1000; i++){
		EntityMovile* alied = new EntityMovile();

		alied->mesh = Mesh::LOAD("assets/mesh/LightSpaceShip");			alied->texture = Texture::LOAD("assets/mesh/LightSpaceShip.tga");
		//alied->mesh_low = Mesh::LOAD("assets/mesh/asteroide1");			alied->texture_low = Texture::LOAD("assets/mesh/asteroide.tga");

		Vector3 vectorRandom; vectorRandom.random(Vector3(2, 2, 2));
		Vector3 AliedPos = CarrierAlied->model*Vector3();
		double x = (vectorRandom.x < 1.5) ? (vectorRandom.x > 0.5) ? 0.5 : vectorRandom.x : 1.5;
		double z = (vectorRandom.z < 1.5) ? (vectorRandom.z > 0.5) ? 0.5 : vectorRandom.z : 1.5;
		alied->model.setTranslation(AliedPos.x + (x - 1) * 100, AliedPos.y + (vectorRandom.y - 1) * 50, AliedPos.z + (z - 1) * 100);

		
		///*UIComponent* target = new UIComponent();
		//target->uiElement = NULL;
		//target->ui = world->ui;
		//alied->addChild(target);
		//
		world->addChild(alied);
		
		world->controllers.getControlled(alied->name)->doing = 9;
		world->controllers.getControlled(alied->name)->target = player;
		
		//agregamos trail
		Emitter* emit = new Emitter();
		emit->manager = world->pManager;
		emit->dies = false;
		emit->encendido = true;
		emit->ttl = 1.0;
		emit->life_time = 1.0;
		emit->type = 0;
		alied->addChild(emit);
		///

	}

	//Some Enemy
	for (int i = 0; i < 1000; i++){
		EntityMovile* enemy = new EntityMovile();
		enemy->mesh = Mesh::LOAD("assets/mesh/LightSpaceShip");			enemy->texture = Texture::LOAD("assets/mesh/LightSpaceShip.tga");
		//enemy->mesh_low = Mesh::LOAD("assets/mesh/asteroide1");		enemy->texture_low = Texture::LOAD("assets/mesh/asteroide.tga");

		Vector3 vectorRandom; vectorRandom.random(Vector3(2, 2, 2));
		Vector3 enemyPos = CarrierEnemy->model*Vector3();
		double x = (vectorRandom.x < 1.5) ? (vectorRandom.x > 0.5) ? 0.5 : vectorRandom.x : 1.5;
		double z = (vectorRandom.z < 1.5) ? (vectorRandom.z > 0.5) ? 0.5 : vectorRandom.z : 1.5;
		enemy->model.setTranslation(enemyPos.x + (x - 1) * 100, enemyPos.y + (vectorRandom.y - 1) * 50, enemyPos.z + (z - 1) * 100);
		world->addChild(enemy);
		world->controllers.getControlled(enemy->name)->doing = 4;
		world->controllers.getControlled(enemy->name)->target = CarrierAlied;



	}
	

	for (int i = 0; i < 0; i++){
		Vector3 vectorRandom; vectorRandom.random(Vector3(1, 1, 1)); vectorRandom = vectorRandom + Vector3(1.0, 1.0, 1.0); //Rango entre 0 y 2
		float azimuth, altitude;
		azimuth = vectorRandom.x * M_PI;
		altitude = vectorRandom.y * M_PI;
		float x, y, z, r;
		r = 100;
		x = r * sin(altitude) * cos(azimuth);
		y = r * sin(altitude) * sin(azimuth);
		z = r * cos(altitude);

		Particle* particle = world->pManager->getParticle();
		particle->pos = Vector3(x,y,z);
		particle->first_update = false;
		particle->cameraType = 0;
		particle->type = 1;
		particle->kind = "FLARE";
		particle->frame = 0;
		particle->life_time = 10.0; particle->ttl = 10.0;
		particle->start_Color = Vector4(1.0, 1.0, 1.0, 1.0);
		particle->end_color = Vector4(0.1, 0.1, 1.0, 1.0);
		particle->start_size.random(Vector3(1.0, 0.0,1.0));
		particle->end_size.x =1.0;
		particle->end_size.y = 0.0;
	}
	*/
	std::cout << "game startup complete" << std::endl;

	
}
//what to do when the image has to be draw
void Game::render(void)
{
	//set the clear color (the background color)
	glClearColor(0.0, 0.0, 0.0, 1.0);
	// Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	
	
	states.back()->render(camera);
	
	/*
	//Put the camera matrices on the stack of OpenGL (only for fixed rendering)
	//pedir ayuda a hermanpara mover la camara adentro del world en cuestion cuando este vivo
	camera->set();
	world->render(camera);
	*/
	//if (map/*Map Toogled*/) world->renderMap(camera);
	
	//if (menu/*First Person Cam Toogled*/) world->renderMenu(camera, 0);
	//if (gui/*First Person Cam Toogled*/){ (current == 3) ? world->renderGUI(camera, 1) : world->renderGUI(camera, 0); }
	//swap between front buffer and back buffer
	
	
	SDL_GL_SwapWindow(this->window);
}

void Game::update(double seconds_elapsed)
{
	getJoyStickEvents();
	
	//updateamos todos los objetos, fisicas, estihjkooados etc.
	states.back()->update(seconds_elapsed);

	//world->update(seconds_elapsed);

	//to navigate with the mouse fixed in the middle
	if (mouse_locked){
		SDL_WarpMouseInWindow(this->window, window_width*0.5, window_height*0.5); //put the mouse back in the middle of the screen
		this->mouse_position.x = window_width*0.5;
		this->mouse_position.y = window_height*0.5;
	}
	//Actualizar camaras
	
	//updateCameras();
}
void Game::getJoyStickEvents(){

	dUP = dDOWN = dLEFT = dRIGHT = dSELECT = dSTART =
	dCROSS = dSQUARE = dCIRCLE = dTRIANGLE =  dR1 = dR2  = dL1 = dL2 =  false ;

	try{//Try to open joystick first joystick
		myJoystick1 = openJoystick(0);
		try{//Try to get joystickparameters
			joystickstate = getJoystickState(myJoystick1);
			if (joystickstate.button[SELECT_BUTTON]){
				gSELECT = true;
			}
			else if (gSELECT){
				gSELECT = false;
				dSELECT = true;
				map = false;
				current++;
				if (current > 2){
					current = 0;
				}
				if (current == 2){
					mapangle = 0.0f;
					map = true;
				}
			}

			if (joystickstate.button[START_BUTTON]){
				joystickstate.press = true;
				gSTART = true;
			}
			else if (gSTART){
				gSTART = false;
				dSTART = true;
			}


			if (joystickstate.hat == HAT_UP){
				gUP = true;
			}
			else if (gUP){
				gUP = false;
				dUP = true;
			}
			if (joystickstate.hat == HAT_DOWN){
				gDOWN = true;
			}
			else if (gDOWN){
				gDOWN = false;
				dDOWN = true;
			}
			if (joystickstate.hat == HAT_LEFT){
				gLEFT = true;
			}
			else if (gLEFT){
				gLEFT = false;
				dLEFT = true;
			}
			if (joystickstate.hat == HAT_RIGHT){
				gRIGHT = true;
			}
			else if (gRIGHT){
				gRIGHT = false;
				dRIGHT = true;
			}

			if (joystickstate.button[CROSS_BUTTON]){
				gCROSS = true;
			}
			else if (gCROSS){
				gCROSS = false;
				dCROSS = true;
			}
			if (joystickstate.button[TRIANGLE_BUTTON]){
				gTRIANGLE = true;
			}
			else if (gTRIANGLE){
				gTRIANGLE = false;
				dTRIANGLE = true;

			}

			if (joystickstate.button[SQUARE_BUTTON]){
				gSQUARE = true;
			}
			else if (gSQUARE){
				gSQUARE = false;
				dSQUARE= true;
			}
			if (joystickstate.button[CIRCLE_BUTTON]){
				gCIRCLE = true;
			}
			else if (gCIRCLE){
				gCIRCLE = false;
				dCIRCLE = true;

			}
			if (joystickstate.button[L1_BUTTON]){
				gL1 = true;
			}
			else if (gL1){
				gL1 = false;
				dL1 = true;
			}

			
		}
		catch (int){}
	}
	catch (int){}


	if (dL1){ (LockOn) ? LockOn = false : LockOn = true; }


}
//Keyboard event handler (sync input)
void Game::onKeyPressed( SDL_KeyboardEvent event )
{
	switch(event.keysym.sym)
	{
		case SDLK_ESCAPE: exit(0); //ESC key, kill the app
		case SDLK_m:{	map = (current == 2) ? false : true;
						current = (current == 2)? 0:  2; break; }
		case SDLK_UP:case SDLK_w:{gUP = true;break; }
		case SDLK_LEFT:case SDLK_a:{gLEFT = true; break; }
		case SDLK_RIGHT: case SDLK_d:{gRIGHT = true; break; }
		case SDLK_DOWN: case SDLK_s:{gDOWN = true; break; }
		case SDLK_SPACE: case SDLK_RETURN:{ gSTART = true; break; }

		default:
			if (gUP){gUP = false;dUP = true;}
			if (gLEFT){ gLEFT = false; dLEFT = true; }
			if (gRIGHT){ gRIGHT = false; dRIGHT = true; }
			if (gDOWN){ gDOWN = false; dDOWN = true; }
			if (gSTART){ gSTART = false; dSTART = true; }
			if (gSTART){ gSTART = false; dSTART = true; }
	}
}

void Game::onMouseButton( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_MIDDLE) //middle mouse
	{
		mouse_locked = !mouse_locked;
		SDL_ShowCursor(!mouse_locked);
	}
}
void Game::onMouseWheel(SDL_MouseWheelEvent event){	
}


void Game::setWindowSize(int width, int height){
	glViewport( 0,0, width, height );
	//camera->aspect =  width / (float)height;
	states.back()->_cam->aspect = width / (float)height;
	states.back()->_cam->width = width;
	states.back()->_cam->height = height;

	
	states.back()->_cam2d->left = -width / (float)height;
	states.back()->_cam2d->right = width / (float)height;

	window_width = width;
	window_height = height;
}

void Game::updateCameras(EntityMovile* _player,float elapsed_time){
	//Camera view Settings
	camera = states.back()->_cam;
	player = _player;
	switch (current){
		case 0:	{//Third Person
					Matrix44 copia = player->model;
					Vector3 up = copia.topVector();
					Vector3 eye = copia  * Vector3(0, 6, -30);
					Vector3 center = copia  * Vector3(0, 10, 10);
					camera->non_interpolative_up = up;
					camera->non_interpolative_eye = eye;
					camera->non_interpolative_center = center;
					camera->lookAt(eye * 0.1 + camera->eye * 0.9, center, up * 0.5 + camera->up * 0.5);
					break;
		}
		case 1:	{//First Person
					Matrix44 copia = player->model;
					Vector3 up = copia.topVector();
					Vector3 eye = copia  * Vector3(0, 1.2, 0.3);
					Vector3 center = copia  * Vector3(0, 1.2, 1);
					camera->non_interpolative_up = up;
					camera->non_interpolative_eye = eye;
					camera->non_interpolative_center = center;
					camera->lookAt(eye, center, up * 0.5 + camera->up * 0.5);
					break;
		}
		case 2:	{//Map
					Matrix44 copia = player->model;
					Vector3 up = Vector3(0.0,1.0,0.0);
					mapangle += joystickstate.axis[R_X]*DEG2RAD*elapsed_time*50;
					Vector3 eye = copia*Vector3() + Vector3(2000, 2500, 2000).rotateInAxis(mapangle, Vector3(0.0, 1.0, 0.0));
					Vector3 center = copia*Vector3()+ Vector3(0, 200, 0);
					camera->non_interpolative_up = up;
					camera->non_interpolative_eye = eye;
					camera->non_interpolative_center = center;
					camera->lookAt(eye * 0.1 + camera->eye * 0.9, center, up * 0.5 + camera->up * 0.5);
					break;
		}
		default:{
					Matrix44 copia = player->model;
					Vector3 up = copia.topVector();
					Vector3 eye = copia  * Vector3(0, 6, -30);
					Vector3 center = copia  * Vector3(0, 5, 10);
					camera->non_interpolative_up = up;
					camera->non_interpolative_eye = eye;
					camera->non_interpolative_center = center;
					camera->lookAt(eye * 0.1 + camera->eye * 0.9, center, up * 0.5 + camera->up * 0.5);
					break;
		}
	}
	Matrix44 copia = player->model;
	Vector3 up = copia.topVector();
	Vector3 eye = copia  * Vector3(0, 100, -100);
	Vector3 center = copia  * Vector3(0, 0, 0);
	cameraMiniMap->lookAt(up,eye,center);
	states.back()->_camMini->lookAt(up, eye, center);
	//camera2D->lookAt(camera->eye, camera->center, camera->up);
	//Skybox respecto al player

}



void Game::clear(){
	while (!states.empty()){
		states.back()->clean();
		states.pop_back();
	}
};

void Game::changeState(State* state){
	//borramos el estado actual
	/*while (!states.empty()){
		states.back()->clean();
		states.pop_back();
	}*/
	clear();

	//insertamos el nuevo
	states.push_back(state);
	states.back()->init();

};

void Game::pushState(State* state){
//util para hacer pausas
	if (!states.empty()){
		states.back()->pause();
	}

	states.push_back(state);
	states.back()->init();

};
void Game::popState(){
	if (!states.empty()){
		states.back()->clean();
		states.pop_back();
	}

	if (!states.empty()){
		states.back()->resume();
	}
};

void Game::quit(){
	BASS_Free();
	exit(0);
};



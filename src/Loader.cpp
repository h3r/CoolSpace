#include "Loader.h"
#include "game.h"

//states
#include "Menu.h"
#include "Level1.h"

Loader::Loader(){};
Loader::~Loader(){};

void Loader::init(){

	screenColor = Vector4(1.0, 1.0, 1.0, 0.0);
	panelColor = Vector4(1.0, 1.0, 1.0, 0.0);

	disk = new EntityMesh();
	disk->mesh = new Mesh();
	disk->mesh->createPlane(100);
	disk->texture = Texture::LOAD("assets/UI/loaderScreen.tga");

	//ponemos en cola las texturas

	//particles
	textures.push_back("assets/particles/mySpaceAtlas.tga");
	//UI
	textures.push_back("assets/UI/fuel.tga");
	textures.push_back("assets/UI/hull.tga");
	textures.push_back("assets/UI/Mapa.tga");
	textures.push_back("assets/UI/marker.tga");
	textures.push_back("assets/UI/Menu.tga");
	textures.push_back("assets/UI/minimap.tga");
	textures.push_back("assets/UI/minimap2.tga");
	textures.push_back("assets/UI/panel.tga");
	textures.push_back("assets/UI/shield.tga");
	textures.push_back("assets/UI/target.tga");
	textures.push_back("assets/UI/howtoplay.tga");


	//Texturas de meshes
	//textures.push_back("assets/mesh/....tga");
	textures.push_back("assets/mesh/asteroide.tga");
	textures.push_back("assets/mesh/frigate.tga");
	textures.push_back("assets/mesh/LightSpaceShip.tga");
	textures.push_back("assets/mesh/oboid.tga");
	textures.push_back("assets/mesh/purplenebula.tga");

	textures.push_back("assets/mesh/NEWCARRIERS/CARRIER01.tga");
	textures.push_back("assets/mesh/NEWCARRIERS/CARRIER02.tga");
	textures.push_back("assets/mesh/NEWCARRIERS/CARRIER03.tga");
	textures.push_back("assets/mesh/NEWASTEROIDS/hdRoid.tga");
	//hay que rellenarlo

	//y ponemos en cola las meshes
	meshes.push_back("assets/mesh/frigate");
	meshes.push_back("assets/mesh/LightSpaceShip");
	meshes.push_back("assets/mesh/oboiddrone");
	meshes.push_back("assets/mesh/asteroides");
	meshes.push_back("assets/mesh/space_cubemap");
	meshes.push_back("assets/mesh/NEWCARRIERS/carrier1");
	meshes.push_back("assets/mesh/NEWCARRIERS/carrier2");
	meshes.push_back("assets/mesh/NEWCARRIERS/carrier3");
	meshes.push_back("assets/mesh/NEWASTEROIDS/hdRoid");
	meshes.push_back("assets/mesh/NEWASTEROIDS/hdRoidMid");
	meshes.push_back("assets/mesh/NEWASTEROIDS/lqRoid1");
	meshes.push_back("assets/mesh/NEWASTEROIDS/lqRoid2");
	meshes.push_back("assets/mesh/NEWASTEROIDS/lqRoid3");
	meshes.push_back("assets/mesh/NEWASTEROIDS/lqRoid4");
	
	//hay que rellenarlo

	///////////////
	totalText = textures.size();
	totalMesh = meshes.size();

	//sounds
	sounds = false;

	_cam = new Camera();
	_cam->setPerspective(70, WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.001, 100000);
	_cam->eye = Vector3(1000, 1000, 1000);
	_cam->up = Vector3(0.0, 1.0, 0.0);
	_cam->center = Vector3(0.0, 0.0, 0.0);

	_cam2d = new Camera();
	_cam2d->setOrthographic(-_cam->aspect, _cam->aspect, 1, -1, -1, 1);

	shouldExit = false;
};
void Loader::clean(){ 
	delete disk;
	delete this; };

void Loader::update(double elapsed_time){

	if (shouldExit){
		screenColor.a -= elapsed_time;
		panelColor.a = screenColor.a;
		if (screenColor.a <= 0){
			State* menu = new Menu();
			//State* menu = new gameOver();
			Game::getInstance()->changeState(menu);
		}
	}
	else{

		if (screenColor.a < 1.0){
			screenColor.a += elapsed_time*2;
			return;
		}

	std::cout << textures.size() << std::endl;
	if (!meshes.empty()){
		Mesh::LOAD(meshes.back());
		meshes.pop_back();
	} else if (!textures.empty()){
		Texture::LOAD(textures.back());
		textures.pop_back();
	}
	else if (!sounds){
		sounds = true;
		
		Game::getInstance()->menuMusic = BASS_SampleLoad(false, "assets/music/[menu]LJ_Kruzer_-_01_-_Chantiers_Navals_412.wav", 0, 0, 3, BASS_SAMPLE_LOOP);
		Game::getInstance()->battle1Music = BASS_SampleLoad(false, "assets/music/[level1]All This.mp3", 0, 0, 3, BASS_SAMPLE_LOOP);
		Game::getInstance()->battle2Music = BASS_SampleLoad(false, "assets/music/[level2]Mistake the Getaway.mp3", 0, 0, 3, BASS_SAMPLE_LOOP);
		Game::getInstance()->battle3Music = BASS_SampleLoad(false, "assets/music/[level3]Agnus Dei X.mp3", 0, 0, 3, BASS_SAMPLE_LOOP);
		Game::getInstance()->over = BASS_SampleLoad(false, "assets/music/[over]Edward_Shallow_-_07_-_We_Are_Carbon.wav", 0, 0, 3, BASS_SAMPLE_LOOP);
		Game::getInstance()->win = BASS_SampleLoad(false, "assets/music/[win]Chris_Zabriskie_-_05_-_Air_Hockey_Saloon.wav", 0, 0, 3, BASS_SAMPLE_LOOP);

		Game::getInstance()->pew = BASS_SampleLoad(false, "assets/sfx/single-fusion-shot.wav", 0, 0, 3, 0);
		Game::getInstance()->wrum = BASS_SampleLoad(false, "assets/sfx/UFO_Takeoff-Sonidor-1604321570.wav", 0, 0, 3, 0);
		Game::getInstance()->boom1 = BASS_SampleLoad(false, "assets/sfx/Explosion_2-SoundBible.wav", 0, 0, 3, 0);
		Game::getInstance()->boom2 = BASS_SampleLoad(false, "assets/sfx/Explosion_3-SoundBible.wav", 0, 0, 3, 0);
		Game::getInstance()->boom3 = BASS_SampleLoad(false, "assets/sfx/Explosion_4-SoundBible.wav", 0, 0, 3, 0);
		Game::getInstance()->boom4 = BASS_SampleLoad(false, "assets/sfx/Explosion-SoundBible.wav", 0, 0, 3, 0);
		Game::getInstance()->bla = BASS_SampleLoad(false, "assets/sfx/bla.wav", 0, 0, 3, 0);
		Game::getInstance()->transition = BASS_SampleLoad(false, "assets/sfx/165191__robinhood76__03700-thor-transitions.wav", 0, 0, 3, 0);
		Game::getInstance()->select = BASS_SampleLoad(false, "assets/sfx/Pickup_Coin3.wav", 0, 0, 3, 0);

	}
	else{
		shouldExit = true;
		}
	}

};

void Loader::render(Camera* cam){
	_cam->set();

	std::cout << textures.size() << std::endl;
	//aqui hermann imprime LoaderScreen.tga y dibuja encima  el siguiente texto

	glDepthMask(GL_TRUE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//glColor4f(1.0, 1.0, 1.0, 1.0);
	glColor4f(screenColor.x, screenColor.y, screenColor.z, screenColor.a);


	disk->render();

	std::string ok = " ";
	if (textures.size() == 0){
		ok = "O";
	}
    if (meshes.size()==0){
		ok = "OO";
	}
	if (shouldExit){
		ok = "OOO";
	}


	printw(Vector3(0.0,0.0,500.0), "LOADING %s" ,&ok);

	

	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);

};

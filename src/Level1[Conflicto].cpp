#include "Level1.h"


#define CERTAIN_BULLET_HIT_DISTANCE 200
#define CERTAIN_M2M_DISTANCE 400
Mesh * test;
Mesh * test3;
Level1::Level1(void){
	game_status = 0;
	game = Game::getInstance();
	game->sfx[1] = BASS_SampleGetChannel(game->select, false);
	game->sfx[2] = BASS_SampleGetChannel(game->transition, false);
	
	_cam = NULL;
	_cam2d = NULL;

	fustrum = Clipper();
	positionsAlly= new Mesh();
	positionsAlly->primitive = GL_POINTS;
	positionsEnemy = new Mesh();
	positionsEnemy->primitive = GL_POINTS;
	test2 = new Mesh();
	test2->primitive = GL_LINES;

	mapGrid = new EntityMesh();
	mapGrid->mesh = new Mesh();
	mapGrid->mesh->createPlane(3000);
	mapGrid->texture = Texture::LOAD("assets/UI/Mapa.tga");
		//CarrierAlied->mesh = Mesh::LOAD("assets/mesh/frigate"); CarrierAlied->texture = Texture::LOAD("assets/mesh/frigate.tga");
	test = new Mesh();
	test->primitive = GL_TRIANGLES;
	test3 = new Mesh();
	test3->primitive = GL_TRIANGLES;
	
	screenColor = Vector4(1.0, 1.0, 1.0, 0.0);
	panelColor = Vector4(1.0, 1.0, 1.0, 0.0);

	entering = true;


}

Level1::~Level1(void){
	/*
	delete _cam;
	delete _cam2d;

	delete positionsAlly;
	delete positionsEnemy;
	delete test2;
	delete mapGrid;

	delete skybox;
	delete player; //modelo al que se  le asigna el foco de las camaras y se asume que es el control del jugador
	delete CarrierAllied;
	delete CarrierEnemy;

	for each (Entity* ent in childrens)
	{
		delete ent;
	}
	childrens.clear();
	children.clear();
	childrensByID.clear();

	

	positionsAlly->vertices.clear();
	positionsEnemy->vertices.clear(); 
	test2->vertices.clear();

	*/
}

void Level1::init(){
	auxLevel = 1;
	std::cout << "me llegue a iniciar?";

	s_gameTime =0;
	s_alliedShipsDestroyed =0;
	s_enemyShipsDestroyed =0;
	s_playerShipsDestroyed =0;
	s_recursosRecolectados =0;
	change_timer = 0;

	game = Game::getInstance();
	float aspect = WINDOW_WIDTH / (float)WINDOW_HEIGHT;
	_cam = new Camera();
	_cam->setPerspective(70, aspect, 0.01, 100000); //set the projection, we want to be perspective
	_cam->eye = Vector3(0.0, 0.0, 0.0);
	_cam2d = new Camera();
	//_cam2d->setOrthographic(-1,1, 1, -1, -1, 1);
	_cam2d->setOrthographic(-aspect, aspect, 1, -1, -1, 1);

	Texture* tTexture = Texture::LOAD("assets/particles/mySpaceAtlas.tga");
	Atlas* atlas = new Atlas("assets/particles/mySpaceAtlas.txt", tTexture);
	particles = ParticleManager::getInstance();
	particles->setup(10000, atlas, _cam);
	particles->texture = tTexture;
	bullets = BulletManager::getInstance();
	bullets->setup();
	///BACKGROUND
	skybox = new EntityMesh();
	switch (s_level){
	case 0:
		skybox->mesh = Mesh::LOAD("assets/mesh/space_cubemap");	skybox->texture = Texture::LOAD("assets/mesh/space_cubemap.tga");
		game->sfx[0] = BASS_SampleGetChannel(game->battle1Music, false);
		BASS_ChannelPlay(game->sfx[0], true);

		break;
	case 1:
		skybox->mesh = Mesh::LOAD("assets/mesh/space_cubemap");	skybox->texture = Texture::LOAD("assets/mesh/purplenebula4.tga");
		game->sfx[0] = BASS_SampleGetChannel(game->battle2Music, false);
		BASS_ChannelPlay(game->sfx[0], true);

		break;
	case 2:
		skybox->mesh = Mesh::LOAD("assets/mesh/space_cubemap");	skybox->texture = Texture::LOAD("assets/mesh/hell_cubemap.tga");
		game->sfx[0] = BASS_SampleGetChannel(game->battle3Music, false);
		BASS_ChannelPlay(game->sfx[0], true);

		break;
	}
	
	skybox->name = "skybox";
	skybox->model.setTranslation(0, 0, 0);
	addChild(skybox, "skybox");
	
	//RESOURCES
	//first Asteroid Group
	createAsteroids(Vector3(), 100);
	
	
	//Carriers, these are needed to create drones
	CarrierAllied = new EntityMovile();
		CarrierAllied->mesh = Mesh::LOAD("assets/mesh/frigate"); 
		CarrierAllied->texture = Texture::LOAD("assets/mesh/frigate.tga");
		CarrierAllied->model.setTranslation(-1000, -200, -1000);
		CarrierAllied->name = "CarrierAllied";
		CarrierAllied->maxHull = 500;
		CarrierAllied->maxShield = 1000;
		CarrierAllied->maxEnergy = 500;
		CarrierAllied->hull = CarrierAllied->maxHull;
		CarrierAllied->shield = CarrierAllied->maxShield;
		CarrierAllied->energy = CarrierAllied->maxEnergy;
		addChild(CarrierAllied, "alliedTeam");

		CarrierEnemy = new EntityMovile();
		CarrierEnemy->mesh = Mesh::LOAD("assets/mesh/frigate");
		CarrierEnemy->texture = Texture::LOAD("assets/mesh/frigate.tga");
		
		CarrierEnemy->model.setTranslation(1000, 200, 1000);
		CarrierEnemy->name = "CarrierEnemy";
		CarrierEnemy->maxHull = 1000;
		CarrierEnemy->maxShield = 500;
		CarrierEnemy->maxEnergy = 500;
		CarrierEnemy->hull = CarrierEnemy->maxHull;
		CarrierEnemy->shield = CarrierEnemy->maxShield;
		CarrierEnemy->energy = CarrierEnemy->maxEnergy;
		addChild(CarrierEnemy, "enemyTeam");

		//barra de vida del carrier enemigo
		/*EntityBillBoard* healthbar= new EntityBillBoard;
		healthbar->_cam = _cam;
		healthbar->color = Vector4(0.2, 0.9, 0.2, 1.0);
		healthbar->size = Vector2(100, 10);
		healthbar->setup();
		CarrierEnemy->addChild(healthbar);
		*/

	//crear  drones para ambos equipos, el vector en 0.0,0.0,0.0 indica que deben crearse  en sus carriers  determinados
		for (int i = 0; i < 50; i++){
			//createDrone(Vector3(0.0, 0.0, 0.0), "alliedTeam");
			Entity* drone = createDrone(Vector3(0.0, 0.0, 0.0), "alliedTeam"); 
			controllers.getControlled(drone->name)->doing = 4;//Atacar al carrier enemigo
			controllers.getControlled(drone->name)->target = CarrierEnemy;
		}
		for (int i = 0; i < 200; i++){
			Entity* drone = createDrone(Vector3(0.0, 0.0, 0.0), "alliedTeam");
			controllers.getControlled(drone->name)->doing = 5;//Defender tu carrier
			controllers.getControlled(drone->name)->target = CarrierAllied;
		}
		for (int i = 0; i < 50; i++){
			//createDrone(Vector3(0.0, 0.0, 0.0), "alliedTeam");
			Entity* drone = createDrone(Vector3(0.0, 0.0, 0.0), "enemyTeam");
			controllers.getControlled(drone->name)->doing = 4;//Atacar al carrier enemigo
			controllers.getControlled(drone->name)->target = CarrierAllied;
		}
		for (int i = 0; i < 200; i++){
			Entity* drone = createDrone(Vector3(0.0, 0.0, 0.0), "enemyTeam");
			controllers.getControlled(drone->name)->doing = 5;//Atacar al carrier enemigo
			controllers.getControlled(drone->name)->target = CarrierEnemy;
		}

		//aqui creariamos el modelo especial del jugador si tuvieramos tiempo,por ahora solo metemos un drone a mano;
		player = createDrone(Vector3(0.0, 0.0, 0.0), "alliedTeam", "player1");
		player->model.setTranslation(0, 0, 0);
		controllers.getControlled(player->name)->doing = 0;
		controllers.getControlled(player->name)->target = CarrierAllied;
		
};
void Level1::clean(){
	
	
	delete this; };

void Level1::render(Camera * camera){

	_cam->set();
	Vector3 cameraEye = _cam->eye;

	std::vector<Entity*> * toRender;
	fustrum.ExtractFrustum();
	
	glDisable(GL_DEPTH_TEST);
		toRender = &children["skybox"];
		for (unsigned int i = 0; i< toRender->size(); i++){
			toRender->at(i)->render();

		}
	glEnable(GL_DEPTH_TEST);
	
	toRender = &children["alliedTeam"];
	for (unsigned int i = 0; i< toRender->size(); i++){
		Entity* renderItem = toRender->at(i);
		if (renderItem->alive){
			if (fustrum.SphereInFrustum(renderItem->getCenter().x, renderItem->getCenter().y, renderItem->getCenter().z, renderItem->getHalfSize())){
				renderItem->render((renderItem->getCenter() - cameraEye).length()); //El parametro que le pasamos es la distancia del  objeto al eye, lo utilizamos para utilizar una mesh low
				
			}
		}
	}
	toRender = &children["enemyTeam"];
	for (unsigned int i = 0; i< toRender->size(); i++){
		Entity* renderItem = (toRender->at(i));
		if (renderItem->alive){
			if (fustrum.SphereInFrustum(renderItem->getCenter().x, renderItem->getCenter().y, renderItem->getCenter().z, renderItem->getHalfSize())){
				renderItem->render((renderItem->getCenter() - cameraEye).length()); //El parametro que le pasamos es la distancia del  objeto al eye, lo utilizamos para utilizar una mesh low
			}
		}
	}
	
	toRender = &children["asteroids"];
	for (unsigned int i = 0; i< toRender->size(); i++){
		Entity* renderItem = (toRender->at(i));
		if (renderItem->alive){
			if (fustrum.SphereInFrustum(renderItem->getCenter().x, renderItem->getCenter().y, renderItem->getCenter().z, renderItem->getHalfSize())){
				renderItem->render((renderItem->getCenter() - cameraEye).length()); //El parametro que le pasamos es la distancia del  objeto al eye, lo utilizamos para utilizar una mesh low
			}
		}
	}
	//Renderizado de aviones y demás no especiales
	toRender = &children["none"];
	for (unsigned int i = 0; i< toRender->size(); i++){
		Entity* renderItem = toRender->at(i);
		if (renderItem->alive){
			//test->vertices.push_back(renderItem->model * Vector3());
			if (fustrum.SphereInFrustum(renderItem->getCenter().x, renderItem->getCenter().y, renderItem->getCenter().z, renderItem->getHalfSize())){
				renderItem->render((renderItem->getCenter() - cameraEye).length()); //El parametro que le pasamos es la distancia del  objeto al eye, lo utilizamos para utilizar una mesh low
			}
		}
	}
	toRender = &children["player"];
	for (unsigned int i = 0; i< toRender->size(); i++){
		Entity* renderItem = toRender->at(i);
		if (renderItem->alive){ renderItem->render(); }
	}

	bullets->render();

	if (game->map){
		renderMap(_cam);
	}
	particles->render();
	
	_cam2d->set();
	renderGUI(_cam, 0);

	
}
void Level1::renderMap(Camera * camera){

	
	positionsAlly->vertices.clear();
	positionsEnemy->vertices.clear();
	test2->vertices.clear();
	fustrum.ExtractFrustum();

	std::vector<Entity*> * toRender;
	toRender = &children["enemyTeam"];
	for (unsigned int i = 0; i< toRender->size(); i++){
		Entity* renderItem = (toRender->at(i));
		if (renderItem->alive){
			if (fustrum.SphereInFrustum(renderItem->getCenter().x, renderItem->getCenter().y, renderItem->getCenter().z, renderItem->getHalfSize())){
				Vector3 position = renderItem->model * Vector3();
				Vector3 positionInPlane = position;
				positionInPlane.y = 0.0;

				positionsEnemy->vertices.push_back(position);
				test2->vertices.push_back(Vector3(position.x,position.y-10,position.z));
				test2->vertices.push_back(positionInPlane);
			}
		}
	}

	toRender = &children["alliedTeam"];
	for (unsigned int i = 0; i< toRender->size(); i++){
		Entity* renderItem = (toRender->at(i));
		if (renderItem->alive){
			if (fustrum.SphereInFrustum(renderItem->getCenter().x, renderItem->getCenter().y, renderItem->getCenter().z, renderItem->getHalfSize())){
				Vector3 position = renderItem->model * Vector3();
				Vector3 positionInPlane = position;
				positionInPlane.y = 0.0;

				positionsAlly->vertices.push_back(position);
				test2->vertices.push_back(Vector3(position.x, position.y - 10, position.z));
				test2->vertices.push_back(positionInPlane);
			}
		}
	}


	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glDepthMask(GL_FALSE);
	glDisable(GL_CULL_FACE);
		mapGrid->render();
	glEnable(GL_CULL_FACE);
	glDepthMask(GL_TRUE);
	glColor4f(0.36, 0.56, 0.78, 0.5);//Blue
	test2->render(); //Para ver el punto donde colisiona
	

	glPointSize(5);
	glColor4f(0.62, 0.21, 0.21, 0.5);//Red
	positionsEnemy->render();
	glColor4f(0.26, 0.67, 0.32, 0.5);//Green
	positionsAlly->render(); //Para ver el punto donde colisiona

	glColor4f(1.0, 1.0, 1.0, 1.0);

	glDisable(GL_BLEND);
}
void Level1::renderMenu(Camera * camera, int state){}
void Level1::renderGUI(Camera * camera, int state){
	test->vertices.clear();
	test3->vertices.clear();

	//Renderizar barras de vida
	Entity * entity = getChildren("CarrierEnemy");// childrensByID["CarrierEnemy"];
	Vector3 ent_position = entity->model* Vector3();
	Vector3 position = ent_position + (camera->up * entity->getHalfSize());
	ent_position = camera->viewprojection_matrix * ent_position;
	position = camera->viewprojection_matrix * position;
	
	
	//Esta dentro de la camara
	if (position.z > 0){
		position.x /= position.z;
		position.y /= position.z;
		position.z = 0;
		ent_position.x /= ent_position.z;
		ent_position.y /= ent_position.z;
		ent_position.z = 0;
		float dist = (ent_position - position).length();

		Vector3 tVectorP = position;
		float Hull = ((EntityMovile*)entity)->hull / ((EntityMovile*)entity)->maxHull;//   1.0;//Pon aqui lo que tu quieras
		float Shield = ((EntityMovile*)entity)->shield / ((EntityMovile*)entity)->maxShield;//   1.0;//Pon aqui lo que tu quieras
		Vector3 tVectorS = Vector3(0.01, 0.0, dist*0.5*Hull);
		Vector3 up = Vector3(0.0, 1.0, 0.0);;
		Vector3 right = Vector3(0.75, 0.0, 0.0);

		Vector3 vertexpos0 = Vector3(tVectorP + (up * tVectorS.x) + (right * tVectorS.z));
		Vector3 vertexpos1 = Vector3(tVectorP - (up * tVectorS.x) + (right * tVectorS.z));
		Vector3 vertexpos2 = Vector3(tVectorP - (up * tVectorS.x) - (right * tVectorS.z));
		Vector3 vertexpos3 = Vector3(tVectorP + (up * tVectorS.x) - (right * tVectorS.z));
		Vector3 vertexpos4 = vertexpos0;
		Vector3 vertexpos5 = vertexpos2;

		test->vertices.push_back(vertexpos0);
		test->vertices.push_back(vertexpos1);
		test->vertices.push_back(vertexpos2);
		test->vertices.push_back(vertexpos3);
		test->vertices.push_back(vertexpos4);
		test->vertices.push_back(vertexpos5);

		tVectorP = position + Vector3(0.0, 0.025, 0.0);
		tVectorS = Vector3(0.01, 0.0, dist*0.5*Shield);

		vertexpos0 = Vector3(tVectorP + (up * tVectorS.x) + (right * tVectorS.z));
		vertexpos1 = Vector3(tVectorP - (up * tVectorS.x) + (right * tVectorS.z));
		vertexpos2 = Vector3(tVectorP - (up * tVectorS.x) - (right * tVectorS.z));
		vertexpos3 = Vector3(tVectorP + (up * tVectorS.x) - (right * tVectorS.z));
		vertexpos4 = vertexpos0;
		vertexpos5 = vertexpos2;

		test3->vertices.push_back(vertexpos0);
		test3->vertices.push_back(vertexpos1);
		test3->vertices.push_back(vertexpos2);
		test3->vertices.push_back(vertexpos3);
		test3->vertices.push_back(vertexpos4);
		test3->vertices.push_back(vertexpos5);

	}
	entity = getChildren("CarrierAllied");// childrensByID["CarrierEnemy"];
	ent_position = entity->model* Vector3();
	position = ent_position + (camera->up * entity->getHalfSize());
	ent_position = camera->viewprojection_matrix * ent_position;
	position = camera->viewprojection_matrix * position;


	//Esta dentro de la camara
	if (position.z > 0){
		position.x /= position.z;
		position.y /= position.z;
		position.z = 0;
		ent_position.x /= ent_position.z;
		ent_position.y /= ent_position.z;
		ent_position.z = 0;
		float dist = (ent_position - position).length();

		Vector3 tVectorP = position;
		float Hull = ((EntityMovile*)entity)->hull / ((EntityMovile*)entity)->maxHull;//   1.0;//Pon aqui lo que tu quieras
		float Shield = ((EntityMovile*)entity)->shield / ((EntityMovile*)entity)->maxShield;//   1.0;//Pon aqui lo que tu quieras
		Vector3 tVectorS = Vector3(0.01, 0.0, dist*0.5*Hull);
		Vector3 up = Vector3(0.0, 1.0, 0.0);
		Vector3 right = Vector3(0.75, 0.0, 0.0);

		Vector3 vertexpos0 = Vector3(tVectorP + (up * tVectorS.x) + (right * tVectorS.z));
		Vector3 vertexpos1 = Vector3(tVectorP - (up * tVectorS.x) + (right * tVectorS.z));
		Vector3 vertexpos2 = Vector3(tVectorP - (up * tVectorS.x) - (right * tVectorS.z));
		Vector3 vertexpos3 = Vector3(tVectorP + (up * tVectorS.x) - (right * tVectorS.z));
		Vector3 vertexpos4 = vertexpos0;
		Vector3 vertexpos5 = vertexpos2;

		test->vertices.push_back(vertexpos0);
		test->vertices.push_back(vertexpos1);
		test->vertices.push_back(vertexpos2);
		test->vertices.push_back(vertexpos3);
		test->vertices.push_back(vertexpos4);
		test->vertices.push_back(vertexpos5);
		
		tVectorP = position + Vector3(0.0,0.025,0.0);
		tVectorS = Vector3(0.01, 0.0, dist*0.5*Shield);

		vertexpos0 = Vector3(tVectorP + (up * tVectorS.x) + (right * tVectorS.z));
		vertexpos1 = Vector3(tVectorP - (up * tVectorS.x) + (right * tVectorS.z));
		vertexpos2 = Vector3(tVectorP - (up * tVectorS.x) - (right * tVectorS.z));
		vertexpos3 = Vector3(tVectorP + (up * tVectorS.x) - (right * tVectorS.z));
		vertexpos4 = vertexpos0;
		vertexpos5 = vertexpos2;

		test3->vertices.push_back(vertexpos0);
		test3->vertices.push_back(vertexpos1);
		test3->vertices.push_back(vertexpos2);
		test3->vertices.push_back(vertexpos3);
		test3->vertices.push_back(vertexpos4);
		test3->vertices.push_back(vertexpos5);

	}
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(GL_FALSE);

	glDisable(GL_CULL_FACE);


	entity = getChildren("player1");// childrensByID["CarrierEnemy"];
	float Shield = ((EntityMovile*)entity)->shield / ((EntityMovile*)entity)->maxShield;//   1.0;//Pon aqui lo que tu quieras
	float Hull = ((EntityMovile*)entity)->hull / ((EntityMovile*)entity)->maxHull;//   1.0;//Pon aqui lo que tu quieras
	float Energy = ((EntityMovile*)entity)->energy / ((EntityMovile*)entity)->maxEnergy;//   1.0;//Pon aqui lo que tu quieras

	//Render GUI : Shield
	Texture * guiTex = Texture::LOAD("assets/UI/shield2.tga");
	Mesh * guiMesh = new Mesh();
	guiMesh->primitive = GL_TRIANGLES;
	Vector3 up = Vector3(0.0, 1.0, 0.0);
	up = up.rotateInAxis((-1+Shield)*M_PI, Vector3(0.0, 0.0, 1.0));
	Vector3 right = Vector3(0.0, 0.0, 1.0).cross(up);// Vector3(-1.0, 0.0, 0.0);
	Vector3 tVectorP = Vector3(0.0, -0.775, 0.0);
	Vector3 tVectorS = Vector3(1.00, 0.5, 1.00)*1.475;
	Vector3 vertexpos0 = Vector3(tVectorP + (up * tVectorS.x) + (right * tVectorS.z));
	Vector3 vertexpos1 = Vector3(tVectorP - (up * tVectorS.x) + (right * tVectorS.z));
	Vector3 vertexpos2 = Vector3(tVectorP - (up * tVectorS.x) - (right * tVectorS.z));
	Vector3 vertexpos3 = Vector3(tVectorP + (up * tVectorS.x) - (right * tVectorS.z));
	Vector3 vertexpos4 = vertexpos0;
	Vector3 vertexpos5 = vertexpos2;

	guiMesh->vertices.push_back(vertexpos0);
	guiMesh->vertices.push_back(vertexpos1);
	guiMesh->vertices.push_back(vertexpos2);
	guiMesh->vertices.push_back(vertexpos3);
	guiMesh->vertices.push_back(vertexpos4);
	guiMesh->vertices.push_back(vertexpos5);

	//all of them have the same normal
	guiMesh->normals.push_back(Vector3(0, 1, 0));
	guiMesh->normals.push_back(Vector3(0, 1, 0));
	guiMesh->normals.push_back(Vector3(0, 1, 0));
	guiMesh->normals.push_back(Vector3(0, 1, 0));
	guiMesh->normals.push_back(Vector3(0, 1, 0));
	guiMesh->normals.push_back(Vector3(0, 1, 0));

	//texture coordinates
	guiMesh->uvs.push_back(Vector2(0, 1));
	guiMesh->uvs.push_back(Vector2(0, 0));
	guiMesh->uvs.push_back(Vector2(1, 0));
	guiMesh->uvs.push_back(Vector2(1, 1));
	guiMesh->uvs.push_back(Vector2(0, 1));
	guiMesh->uvs.push_back(Vector2(1, 0));

	guiTex->bind();
	guiMesh->render();
	guiTex->unbind();

	//Energy bar
	guiTex = Texture::LOAD("assets/UI/fuel2.tga");
	guiMesh = new Mesh();
	guiMesh->primitive = GL_TRIANGLES;
	up = Vector3(0.0, 1.0, 0.0);
	up = up.rotateInAxis((-1 + Energy)*M_PI, Vector3(0.0, 0.0, 1.0));
	right = Vector3(0.0, 0.0, 1.0).cross(up);// Vector3(-1.0, 0.0, 0.0);
	tVectorP = Vector3(0.0, -0.775, 0.0);
	vertexpos0 = Vector3(tVectorP + (up * tVectorS.x) + (right * tVectorS.z));
	vertexpos1 = Vector3(tVectorP - (up * tVectorS.x) + (right * tVectorS.z));
	vertexpos2 = Vector3(tVectorP - (up * tVectorS.x) - (right * tVectorS.z));
	vertexpos3 = Vector3(tVectorP + (up * tVectorS.x) - (right * tVectorS.z));
	vertexpos4 = vertexpos0;
	vertexpos5 = vertexpos2;

	guiMesh->vertices.push_back(vertexpos0);
	guiMesh->vertices.push_back(vertexpos1);
	guiMesh->vertices.push_back(vertexpos2);
	guiMesh->vertices.push_back(vertexpos3);
	guiMesh->vertices.push_back(vertexpos4);
	guiMesh->vertices.push_back(vertexpos5);

	//all of them have the same normal
	guiMesh->normals.push_back(Vector3(0, 1, 0));
	guiMesh->normals.push_back(Vector3(0, 1, 0));
	guiMesh->normals.push_back(Vector3(0, 1, 0));
	guiMesh->normals.push_back(Vector3(0, 1, 0));
	guiMesh->normals.push_back(Vector3(0, 1, 0));
	guiMesh->normals.push_back(Vector3(0, 1, 0));

	//texture coordinates
	guiMesh->uvs.push_back(Vector2(0, 1));
	guiMesh->uvs.push_back(Vector2(0, 0));
	guiMesh->uvs.push_back(Vector2(1, 0));
	guiMesh->uvs.push_back(Vector2(1, 1));
	guiMesh->uvs.push_back(Vector2(0, 1));
	guiMesh->uvs.push_back(Vector2(1, 0));

	guiTex->bind();
	guiMesh->render();
	guiTex->unbind();

	//Render GUI: Hull bar
	guiTex = Texture::LOAD("assets/UI/hull2.tga");
	guiMesh = new Mesh();
	guiMesh->primitive = GL_TRIANGLES;
	up = Vector3(0.0, 1.0, 0.0);
	up = up.rotateInAxis((-1 + Hull)*M_PI, Vector3(0.0, 0.0, 1.0));
	right = Vector3(0.0, 0.0, 1.0).cross(up);// Vector3(-1.0, 0.0, 0.0);
	tVectorP = Vector3(0.0, -0.775, 0.0);
	vertexpos0 = Vector3(tVectorP + (up * tVectorS.x) + (right * tVectorS.z));
	vertexpos1 = Vector3(tVectorP - (up * tVectorS.x) + (right * tVectorS.z));
	vertexpos2 = Vector3(tVectorP - (up * tVectorS.x) - (right * tVectorS.z));
	vertexpos3 = Vector3(tVectorP + (up * tVectorS.x) - (right * tVectorS.z));
	vertexpos4 = vertexpos0;
	vertexpos5 = vertexpos2;

	guiMesh->vertices.push_back(vertexpos0);
	guiMesh->vertices.push_back(vertexpos1);
	guiMesh->vertices.push_back(vertexpos2);
	guiMesh->vertices.push_back(vertexpos3);
	guiMesh->vertices.push_back(vertexpos4);
	guiMesh->vertices.push_back(vertexpos5);

	//all of them have the same normal
	guiMesh->normals.push_back(Vector3(0, 1, 0));
	guiMesh->normals.push_back(Vector3(0, 1, 0));
	guiMesh->normals.push_back(Vector3(0, 1, 0));
	guiMesh->normals.push_back(Vector3(0, 1, 0));
	guiMesh->normals.push_back(Vector3(0, 1, 0));
	guiMesh->normals.push_back(Vector3(0, 1, 0));

	//texture coordinates
	guiMesh->uvs.push_back(Vector2(0, 1));
	guiMesh->uvs.push_back(Vector2(0, 0));
	guiMesh->uvs.push_back(Vector2(1, 0));
	guiMesh->uvs.push_back(Vector2(1, 1));
	guiMesh->uvs.push_back(Vector2(0, 1));
	guiMesh->uvs.push_back(Vector2(1, 0));

	guiTex->bind();
	guiMesh->render();
	guiTex->unbind();


	//Render GUI: Control Panel
	guiTex = Texture::LOAD("assets/UI/panel5.tga");
	guiMesh = new Mesh();
	guiMesh->primitive = GL_TRIANGLES;
	up = Vector3(0.0, 1.0, 0.0);

	right = Vector3(-1.0, 0.0, 0.0);
	tVectorP = Vector3(0.0, -0.775, 0.0);
	tVectorS = Vector3(1.125, 0.5, 1.777)*0.85;
	vertexpos0 = Vector3(tVectorP + (up * tVectorS.x) + (right * tVectorS.z));
	vertexpos1 = Vector3(tVectorP - (up * tVectorS.x) + (right * tVectorS.z));
	vertexpos2 = Vector3(tVectorP - (up * tVectorS.x) - (right * tVectorS.z));
	vertexpos3 = Vector3(tVectorP + (up * tVectorS.x) - (right * tVectorS.z));
	vertexpos4 = vertexpos0;
	vertexpos5 = vertexpos2;

	guiMesh->vertices.push_back(vertexpos0);
	guiMesh->vertices.push_back(vertexpos1);
	guiMesh->vertices.push_back(vertexpos2);
	guiMesh->vertices.push_back(vertexpos3);
	guiMesh->vertices.push_back(vertexpos4);
	guiMesh->vertices.push_back(vertexpos5);

	//all of them have the same normal
	guiMesh->normals.push_back(Vector3(0, 1, 0));
	guiMesh->normals.push_back(Vector3(0, 1, 0));
	guiMesh->normals.push_back(Vector3(0, 1, 0));
	guiMesh->normals.push_back(Vector3(0, 1, 0));
	guiMesh->normals.push_back(Vector3(0, 1, 0));
	guiMesh->normals.push_back(Vector3(0, 1, 0));

	//texture coordinates
	guiMesh->uvs.push_back(Vector2(0, 1));
	guiMesh->uvs.push_back(Vector2(0, 0));
	guiMesh->uvs.push_back(Vector2(1, 0));
	guiMesh->uvs.push_back(Vector2(1, 1));
	guiMesh->uvs.push_back(Vector2(0, 1));
	guiMesh->uvs.push_back(Vector2(1, 0));

	guiTex->bind();
	guiMesh->render();
	guiTex->unbind();

	glColor4f(0.26, 0.67, 0.32, 0.5);//Green
	test->render();
	glColor4f(0.36, 0.56, 0.78, 0.5);//Blue
	test3->render();

	glEnable(GL_CULL_FACE);
	glDepthMask(GL_TRUE);
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glDisable(GL_BLEND);


	/*Entity* renderItem;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDepthMask(GL_FALSE);
	
	std::vector<Entity*> * toRender = &children["UI"];
	renderItem = getChildren("panel");
	Matrix44 copia = renderItem->model;
	copia.inverse();
	Vector3 axis = copia.rotateVector(renderItem->model.topVector());

	for (int i = 0; i < renderItem->children.size(); i++){
		renderItem->children[i]->model.setIdentity();
		renderItem->children[i]->rotate(M_PI * -0.33 * (i+1), axis);
	}
	renderItem->render();
	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);*/
}

void Level1::update(double elapsed_time){	
	s_gameTime += elapsed_time;
	if (game_status == 0){
		//si la partida sigue en curso
		if (CarrierAllied->hull <= 0){
			//pasamos a derrota
			game_status = 1;
			
			Emitter* emit = new Emitter();
			
			emit->dies = true;
			emit->aux = 3;
			emit->encendido = true;
			emit->type =  3;
			emit->ttl = 7.0;
			emit->life_time = 7.0;
			emit->model.setTranslation(CarrierAllied->model.m[12], CarrierAllied->model.m[13], CarrierAllied->model.m[14]);
			addChild(emit, "EMITTER");
		}
		else if (CarrierEnemy->hull <= 0){
			game_status = 2;

			Emitter* emit = new Emitter();
			emit->dies = true;
			emit->encendido = true;
			emit->aux = 3;
			emit->type = 3;
			emit->ttl = 7.0;
			emit->life_time = 7.0;
			emit->model.setTranslation(CarrierEnemy->model.m[12], CarrierEnemy->model.m[13], CarrierEnemy->model.m[14]);
			addChild(emit, "EMITTER");
		}
		else if (game->dSTART){

			game->pushState(new Pause());
			//game->pushState(new gameOver());
			return;
		}
	}
	else if (game_status == 1){
		if (change_timer < 7.5){
			change_timer += elapsed_time;
		}
		else{
			gameOver* over = new gameOver();
			over->s_level = s_level;
			over->s_difficulty=s_difficulty;
			over->s_gameTime=s_gameTime;
			over->s_alliedShipsDestroyed = s_alliedShipsDestroyed;
			over->s_enemyShipsDestroyed = s_enemyShipsDestroyed;
			over->s_playerShipsDestroyed = s_playerShipsDestroyed;
			over->s_recursosRecolectados = s_recursosRecolectados;
			game->changeState(over);
			return;
		}
		
	}
	else if (game_status == 2){
		if (change_timer < 7.5){
			change_timer += elapsed_time;
		}
		else{
			gameWon* over = new gameWon();
			over->s_level = s_level;
			over->s_difficulty = s_difficulty;
			over->s_gameTime = s_gameTime;
			over->s_alliedShipsDestroyed = s_alliedShipsDestroyed;
			over->s_enemyShipsDestroyed = s_enemyShipsDestroyed;
			over->s_playerShipsDestroyed = s_playerShipsDestroyed;
			over->s_recursosRecolectados = s_recursosRecolectados;
			game->changeState(over);
			//game->changeState(over);
			return;
		}
	}

	//Actualizamos la logica del juego (define como van a tener que moverse/actuar )
	controllers.update(elapsed_time);

	//regular updates
	for (std::map<std::string, std::vector<Entity *>>::const_iterator it = children.begin(); it != children.end(); ++it){	
		std::vector<Entity*> objects = children[it->first];
		for (unsigned int j = 0; j < objects.size(); j++){
			if(objects.at(j)->alive)objects.at(j)->update(elapsed_time);
		}
	}


	bullets->update(elapsed_time);

	checkCollisions();

	//Actualizamos la posicion y condicion de las balas
	particles->update(elapsed_time);


	//ui->update(elapsed_time);
	
	//movemos camaras;
	game->updateCameras(player,elapsed_time);
	skybox->model.setTranslation(_cam->eye);

}
void Level1::checkCollisions(){
	std::vector<Entity*> toCheck = children["alliedTeam"];	
	//Comprobamos colisiones entre Entidad-Entidad Bala-Entidad
	
	for (unsigned int i = 0; i < bullets->first_inactive_position; i++)
	{
		Bullet* tBullet = bullets->bullets_vector[i];

		Vector3 collisionPoint;
		
		Entity* from = getChildren(tBullet->author);
		Vector3 from_center = from->model*Vector3();

		for (unsigned int j = 0; j < toCheck.size(); j++)
		{
			Entity* to = toCheck[j];
			if (!to->alive || from->Material == "alliedTeam"){ continue; }
			Vector3 to_pos = to->model*Vector3();// getCenter();
			float to_radius = to->getHalfSize();

			Vector3 dir = tBullet->position - to_pos;
			float dist = dir.length();
			if (to->name != from->name && (dist < 25|| to_radius > 100)){
				if (to->testRayCollision(tBullet->last_position, tBullet->position - tBullet->last_position, collisionPoint, true)){
					tBullet->ttl = -1.0; //Deactivate the bullet
					//from.score(to.colision(collisionPoint));
					
					Emitter* emit = new Emitter();
					emit->dies = true;
					emit->aux = to_radius;
					emit->encendido = true;
					emit->ttl = 2.0;
					emit->life_time = 2.0;
					emit->type = 2; 
					emit->model.setTranslation(collisionPoint.x, collisionPoint.y, collisionPoint.z);
					addChild(emit, "EMITTER");

					//to->destroy();
				}
			}
		}
	}
	toCheck = children["enemyTeam"];
	//Comprobamos colisiones entre Entidad-Entidad Bala-Entidad
	for (unsigned int i = 0; i < bullets->first_inactive_position; i++)
	{
		Bullet* tBullet = bullets->bullets_vector[i];
		Vector3 collisionPoint;

		Entity* from = getChildren(tBullet->author);
		Vector3 from_center = from->getCenter();

		for (unsigned int j = 0; j < toCheck.size(); j++)
		{
			Entity* to = toCheck[j];
			if (!to->alive || from->Material == "enemyTeam"){ continue; }
			Vector3 to_pos = to->model*Vector3();// getCenter();
			float to_radius = to->getHalfSize();

			Vector3 dir = tBullet->position - to_pos;
			float dist = dir.length();
			if (to->name != from->name && (dist < 200 || to_radius > 100)){
				if (to->testRayCollision(tBullet->last_position, tBullet->position - tBullet->last_position, collisionPoint, true)){
					tBullet->ttl = -1.0; //Deactivate the bullet
					//from.score(to.colision(collisionPoint));
					
				}
			}
		}
	}
}

void Level1::addChild(Entity* ent){
	addChild(ent, "none");
}

void Level1::addChild(Entity* ent, std::string Material){
	ent->alive = true;
	ent->Material = Material;
	childrensByID[ent->name] = ent;
	children[Material].push_back(ent);
	controllers.addtoControl(ent);
}

Entity* Level1::getChildren(std::string ID){
	return childrensByID[ID];
}

//Podriamos añadir un vector donde meter los objetos que se han de destruir y el tiempo para ello por si han de hacer alguna animacion o algo
void Level1::removeChild(Entity* ent){
	std::vector<Entity *> * aux = &children[ent->Material];
	ent->alive = false;

	//Remove it from controllers
	controllers.removeControlled(ent->name);

	//Search and remove it from the Level1
	for (unsigned int i = 0; i< aux->size(); i++){
		if(aux->at(i) == ent){
			aux->erase(aux->begin() + i);
			//ent->removeAllChild();			implementar para borrar todos los hijos anidados
		return; //by this point ent is parentless and is not inside any entity now Level1. his children are still inside him however.
		}
	}
}

void Level1::createAsteroids(Vector3 pos, int qty){
	
	for (int i = 0; i < qty; i++){
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

		asteroid->model.setTranslation((x - 1), vectorRandom.y, (z - 1));
		asteroid->model.scale(0.25 + (rand() % 1)*0.75, 0.25 + (rand() % 1)*0.75, 0.25 + (rand() % 1)*0.75);

		addChild(asteroid, "asteroids");
		controllers.addtoControl(asteroid);
		controllers.getControlled(asteroid->name)->doing = 10;
		controllers.getControlled(asteroid->name)->pos =  Vector3(1000,0.0,1000.0);// CarrierEnemy;
		
	}
}

EntityMovile* Level1::createDrone(Vector3 pos, std::string team, std::string name){

	//usamos team para determinar las esquematicas de el drone

	//creamos el trail
	EntityMovile* drone = new EntityMovile();

	Emitter* emit = new Emitter();
	emit->dies = false;
	emit->encendido = true;
	emit->ttl = 1.0;
	emit->life_time = 1.0;
	emit->move(Vector3(0.0, 0.0, -3.5));
	//drone->addChild(emit);

		Vector3 vectorRandom; vectorRandom.random(Vector3(1, 1, 1)); vectorRandom = vectorRandom + Vector3(1.0, 1.0, 1.0); //Rango entre 0 y 2
		Vector3 dronePos;
		float minimal_Radius;
		//si es un aliado
		if (!team.compare("alliedTeam")){
			drone->mesh = Mesh::LOAD("assets/mesh/LightSpaceShip");			drone->texture = Texture::LOAD("assets/mesh/LightSpaceShip.tga");
			if (pos.x == 0.0 && pos.y == 0.0 && pos.x == 0.0){ dronePos = CarrierAllied->model*Vector3(); minimal_Radius = CarrierAllied->getHalfSize(); }
			 else{ dronePos = pos; }
		}
		else{
		//si es  un enemigo
			//drone->mesh = Mesh::LOAD("assets/mesh/oboidDrone");			drone->texture = Texture::LOAD("assets/mesh/oboid.tga");
			drone->mesh = Mesh::LOAD("assets/mesh/LightSpaceShip");			drone->texture = Texture::LOAD("assets/mesh/LightSpaceShip.tga");
			if (pos.x == 0.0 && pos.y == 0.0 && pos.x == 0.0){ dronePos = CarrierEnemy->model*Vector3(); minimal_Radius = CarrierEnemy->getHalfSize(); }
			else{ dronePos = pos; }
		
		}
		float azimuth, altitude;
		azimuth = vectorRandom.x * M_PI;
		altitude = vectorRandom.y * M_PI;
		float x, y, z, r;
		r = minimal_Radius;
		x = r * sin(altitude) * cos(azimuth);
		y = r * sin(altitude) * sin(azimuth);
		z = r * cos(altitude);

		//double x = (vectorRandom.x < 1.5) ? (vectorRandom.x > 0.5) ? 0.5 : vectorRandom.x : 1.5;
		//double z = (vectorRandom.z < 1.5) ? (vectorRandom.z > 0.5) ? 0.5 : vectorRandom.z : 1.5;
		drone->model.setTranslation(dronePos.x + x, dronePos.y + y, dronePos.z + z);

		if (name != ""){
			drone->name = name;
		}
		drone->maxHull = 10;
		drone->maxShield = 5;
		drone->maxEnergy = 50;
		drone->hull = drone->maxHull;
		drone->shield = drone->maxShield;
		drone->energy = drone->maxEnergy;
		addChild(drone, team);
				
		emit->type = 0;

		return drone;
}


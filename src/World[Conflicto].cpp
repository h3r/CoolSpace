#include "World.h"

#define CERTAIN_BULLET_HIT_DISTANCE 50
#define CERTAIN_M2M_DISTANCE 400

World::World(void){
	fustrum = Clipper();
	bullets = BulletManager();
	test = new Mesh();
	test->primitive = GL_POINTS;
}

World::~World(void){
}

void World::render(Vector3 cameraEye){
	std::vector<Entity*> * toRender;
	fustrum.ExtractFrustum();
	
	glDisable(GL_DEPTH_TEST);
		toRender = &children["skybox"];
		for (unsigned int i = 0; i< toRender->size(); i++){
			toRender->at(i)->render();
		}
	glEnable(GL_DEPTH_TEST);


	
	toRender = &children["player"];
	//glDisable(GL_CULL_FACE);
	for (unsigned int i = 0; i< toRender->size(); i++){
		Entity* renderItem = toRender->at(i);
		if (renderItem->status){ renderItem->render(); }
	}
	//glEnable(GL_CULL_FACE);
	
	toRender = &children["aliedTeam"];
	for (unsigned int i = 0; i< toRender->size(); i++){
		Entity* renderItem = toRender->at(i);
		if (renderItem->status){
			if (fustrum.SphereInFrustum(renderItem->getCenter().x, renderItem->getCenter().y, renderItem->getCenter().z, renderItem->getHalfSize())){
				renderItem->render((renderItem->getCenter() - cameraEye).length()); //El parametro que le pasamos es la distancia del  objeto al eye, lo utilizamos para utilizar una mesh low
				
			}
		}
	}
	toRender = &children["enemyTeam"];
	for (unsigned int i = 0; i< toRender->size(); i++){
		Entity* renderItem = toRender->at(i);
		if (renderItem->status){
			if (fustrum.SphereInFrustum(renderItem->getCenter().x, renderItem->getCenter().y, renderItem->getCenter().z, renderItem->getHalfSize())){
				renderItem->render((renderItem->getCenter() - cameraEye).length()); //El parametro que le pasamos es la distancia del  objeto al eye, lo utilizamos para utilizar una mesh low
			}
		}
	}
	//Renderizado de aviones y demás no especiales
	toRender = &children["none"];
	for (unsigned int i = 0; i< toRender->size(); i++){
		Entity* renderItem = toRender->at(i);
		if (renderItem->status){
			if (fustrum.SphereInFrustum(renderItem->getCenter().x, renderItem->getCenter().y, renderItem->getCenter().z, renderItem->getHalfSize())){
				renderItem->render((renderItem->getCenter() - cameraEye).length()); //El parametro que le pasamos es la distancia del  objeto al eye, lo utilizamos para utilizar una mesh low
			}
		}
	}
		
	//test->render(); //Para ver el punto donde colisiona
	bullets.render();

	particles->bind();
	pManager->render();
	particles->unbind();
	
}
void World::update(double elapsed_time){	
	//Actualizamos la logica del juego (define como van a tener que moverse/actuar )
	controllers.update(elapsed_time);

	//regular updates
	for (std::map<std::string, std::vector<Entity *>>::const_iterator it = children.begin(); it != children.end(); ++it){	
		std::vector<Entity*> objects = children[it->first];
		for (unsigned int j = 0; j < objects.size(); j++){
			objects.at(j)->update(elapsed_time);
		}
	}

	checkCollisions();

	//Actualizamos la posicion y condicion de las balas
	bullets.update(elapsed_time);		

	pManager->update(elapsed_time);
	
}
void World::checkCollisions(){
	std::vector<Entity*> objects = children["none"];	

	//Comprobamos colisiones entre Entidad-Entidad Bala-Entidad
	for (unsigned int i = 0; i < bullets.first_inactive_position; i++)
	{
		Bullet* tBullet = bullets.bullets_vector[i];
		Vector3 collisionPoint;
		Entity* from = getChildren(tBullet->author);
		Vector3 from_center = from->getCenter();

		for (unsigned int j = 0; j < objects.size(); j++)
		{
			Entity* to = objects[j];
			Vector3 to_center = to->getCenter();
			float to_radius = to->getHalfSize();

			//Comprobamos la distancia del target respecto del origen es menor de un cierto valor
			//Comprobamos que el objeto con el que va a colisionar no sea el que ha disparado y buscamos la colision con esfera
			if ( (to_center - tBullet->position).length() - to_radius < CERTAIN_BULLET_HIT_DISTANCE &&  from->name != to->name){
				//Si la distancia de impacto es mas pequeña que X entonces comprueba hit con mesh
				if ((to_center - from_center).length() - to_radius < CERTAIN_M2M_DISTANCE){
					if (to->testRayCollision(tBullet->last_position, tBullet->position - tBullet->last_position, collisionPoint, true)){
						tBullet->ttl = -1.0;
						test->vertices.push_back(collisionPoint);
					}
				}
				else{
					if (to->testRayCollision(tBullet->last_position, tBullet->position - tBullet->last_position, collisionPoint, false)){
						tBullet->ttl = -1.0;
						test->vertices.push_back(collisionPoint); //Para el test del punto de colision
					}
				}
			}
		}
	}
}

void World::addChild(Entity* ent){
	addChild(ent, "none");
}

void World::addChild(Entity* ent, std::string Material){
	ent->status = true;
	ent->Material = Material;
	childrensByID[ent->name] = ent;
	children[Material].push_back(ent);
	controllers.addtoControl(ent);
}

Entity* World::getChildren(std::string ID){
	return childrensByID[ID];
}

//Podriamos añadir un vector donde meter los objetos que se han de destruir y el tiempo para ello por si han de hacer alguna animacion o algo
void World::removeChild(Entity* ent){
	std::vector<Entity *> * aux = &children[ent->Material];
	ent->status = false;

	//Remove it from controllers
	controllers.removeControlled(ent->name);

	//Search and remove it from the world
	for (unsigned int i = 0; i< aux->size(); i++){
		if(aux->at(i) == ent){
			aux->erase(aux->begin() + i);
			//ent->removeAllChild();			implementar para borrar todos los hijos anidados
		return; //by this point ent is parentless and is not inside any entity now world. his children are still inside him however.
		}
	}
}


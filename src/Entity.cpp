#include "Entity.h"
#include "includes.h"
#include <sstream>

int Entity::ID = 0;

Entity::Entity(void){
	std::ostringstream aux;
	aux << ++ID;
	name = aux.str();
	parent = NULL;
	Material = "none";
	dir = Vector3(0.0, 0.0, 0.0);
	rot = Vector3(0.0, 0.0, 0.0);
	agi = Vector3(1.0, 1.0, 1.0);

	children.clear();

}


Entity::~Entity(void){
	for (unsigned int i = 0; i<children.size(); i++){
		delete children[i];
	}
}

void Entity::render(void){
	glPushMatrix();

	model.set();
	for(unsigned int i = 0; i< children.size(); i++){
		children[i]->render();
	}

	glPopMatrix();
}
void Entity::render(float distance){
	render();
}
void Entity::update(double elapsed_time)
{
	for( unsigned int i = 0; i< children.size(); i++){
		children[i]->update(elapsed_time);
	}

}

void Entity::addChild(Entity* ent)
{
	ent->alive = true;
	ent->parent = this;
	children.push_back(ent);
}

void Entity::removeChild(Entity* ent)
{
	ent->alive = false;
	ent->parent = NULL;

	for(unsigned int i = 0; i< children.size(); i++)
	{
		if(children[i] == ent)
		{
			children.erase(children.begin()+i);
			return; //by this point ent is parentless and is not inside any entity now world. his children are still inside him however.
		}
	}
}

Matrix44 Entity::getGlobalMatrix(){
	if(parent)
	{
		return model*parent->getGlobalMatrix();
	}
	return model;
}

void Entity::move(float x, float y, float z)
{
	model.traslateLocal(x, y, z);
}
void Entity::move(Vector3 dir)
{
	model.traslateLocal(dir.x, dir.y, dir.z);
}
void Entity::scale(float x, float y, float z)
{
	model.scale(x,y,z);
}
void Entity::rotate(float angle, Vector3 vec){
	model.rotateLocal(angle, vec);
}
void Entity::rotate(Vector3 vec){
	model.rotateLocal(vec.x, Vector3(1.0, 0.0, 0.0));
	model.rotateLocal(vec.y, Vector3(0.0, 1.0, 0.0));
	model.rotateLocal(vec.z, Vector3(0.0, 0.0, 1.0));
}
void Entity::rotate(double x, double y, double z){
	model.rotateLocal(x, Vector3(1.0, 0.0, 0.0));
	model.rotateLocal(y, Vector3(0.0, 1.0, 0.0));
	model.rotateLocal(z, Vector3(0.0, 0.0, 1.0));
}


Vector3 Entity::getCenter(){
	//std::cout << "Warning: trying to get center of plain Entity" << std::endl;
	return Vector3(model.m[12], model.m[13], model.m[14]);
}

float Entity::getHalfSize(){
	//std::cout << "Warning: asking to get halfSize of plain Entity" << std::endl;
	return 0.0; }

bool Entity::collision(Entity* ent){
	//explanation, if we were to accidentally check for collision with an entity
	//we simply would return null because this entities by default don't have mesh
	//std::cout << "Warning: checking collision with plain Entity" << std::endl;
	return false;
}

Vector3 Entity::collisionPoint(Entity* ent){
	//explanation, if we were to accidentally check for collision with an entity
	//we simply would return null because this entities by default don't have mesh
	//std::cout << "Warning: checking collision with plain Entity" << std::endl;
	return Vector3(0.0,0.0,0.0);
}

bool Entity::testMesh2MeshCollision(Entity* ent){
	return false;
}

bool Entity::testMesh2SphereCollision(Entity * ent){
	
	return false;
}


bool  Entity::testRayCollision(Vector3 start, Vector3 dir, Vector3 & result, bool meshCheck, float max ){ return false; }

float Entity::distance(Entity * ent){
	return getCenter().distance(ent->getCenter());
}

void Entity::reset(){
	
	std::ostringstream aux;
	aux << ++ID;
	name = aux.str();
	parent = NULL;
	Material = "none";
	dir = Vector3(0.0, 0.0, 0.0);
	rot = Vector3(0.0, 0.0, 0.0);
	agi = Vector3(1.0, 1.0, 1.0);

	children.clear();

}

void Entity::destroy(){
	alive = false;
	
	for (unsigned int i = 0; i<children.size(); i++){
		delete children[i];
	}

}
void Entity::rotateToVecAngle(Vector3 unitDir, float angleinRad){
	//Guardamos la posicion antes de la transformacion
	Vector3 currentpos = model*Vector3();
	
	//Parametros
	double altura = unitDir.y;
	double ancho = unitDir.x;
	Vector3 hProj = Vector3(unitDir.x, 0.0, unitDir.z);
	double hProjDist = hProj.length();

	//Rotacion necestaria para orientarlo en la normal unitDir
	double altitude = acos(altura / unitDir.length());
	double azimuth = ((Vector3(0.0, 0.0, 1.0).dot(hProj)) > 0) ? acos(ancho / hProjDist) : -acos(ancho / hProjDist);

	//Reiniciamos la posicion de la model
	model.setIdentity();
		//Rotamos en altitud
		model.rotate(altitude, Vector3(1.0, 0.0, 0.0));
		//Rotamos horizontalmente
		model.rotate(azimuth + (M_PI*0.5), Vector3(0.0, 1.0, 0.0));
		//Rotamos sobre el eje de la normal
		model.rotate(angleinRad, unitDir);

}
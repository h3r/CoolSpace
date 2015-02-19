#include "EntityMesh.h"

void EntityMesh::render()
{
	render(0.0);
}

void EntityMesh::render(float distance)
{
	glPushMatrix();
	model.set();

	if (mesh_low && distance > 400.0)
	{
		glAlphaFunc(GL_GREATER, 0.5);
		glEnable(GL_ALPHA_TEST);
		texture_low->bind();
		mesh_low->render();
		texture_low->unbind();
		glDisable(GL_ALPHA_TEST);
	}
	else
	{
		if (texture){ texture->bind(); }

		mesh->render();
		if (texture){ texture->unbind(); }
		for (unsigned int i = 0; i< children.size(); i++)
		{
			children[i]->render();
		}
		
	}

	glPopMatrix();

}
void EntityMesh::update(double elapsed_time)
{

	for(unsigned int i = 0; i< children.size(); i++){
		children[i]->update(elapsed_time);
	}

}

Vector3 EntityMesh::getCenter(){
	return (mesh->center) + Vector3(model.m[12], model.m[13], model.m[14]);
}
float EntityMesh::getHalfSize(){
	return mesh->halfSize;
}


bool EntityMesh::collision(Entity* ent){
	if (getCenter().distance(ent->getCenter()) < getHalfSize() + ent->getHalfSize()){
		return true;
	}else{
		return false;
	}
}
Vector3 EntityMesh::collisionPoint(Entity* ent){
	//for spheres it's easy peasy, simplemente tenes que sumar los centros y dividirlos por 2
	Vector3 centerSum = getCenter() + ent->getCenter();
	centerSum.set(centerSum.x*0.5, centerSum.y*0.5, centerSum.z*0.5);
	return centerSum;	
}

bool EntityMesh::testMesh2SphereCollision(Entity* ent){

	EntityMesh* tEnt = (EntityMesh*)ent;
	
	mesh->collisionModel->setTransform(model.m);
	return mesh->collisionModel->sphereCollision(tEnt->getCenter().v ,tEnt->getHalfSize());
}


bool EntityMesh::testMesh2MeshCollision(Entity* ent){
	EntityMesh* tEnt = (EntityMesh*)ent;

	if (tEnt->mesh == mesh){
		//tienen el mismo mesh!
		mesh->collisionModel->setTransform(model.m);
		return mesh->collisionModel->collision(mesh->collisionModel, -1, 0, tEnt->model.m);
	}
	else{
		//tienen distinto mesh
		mesh->collisionModel->setTransform(model.m);
		tEnt->mesh->collisionModel->setTransform(tEnt->model.m);
		return mesh->collisionModel->collision(tEnt->mesh->collisionModel);
	}
}

bool  EntityMesh::testRayCollision(Vector3 start, Vector3 dir, Vector3 & result, bool meshCheck, float max ){
	if (SphereRayCollision(getCenter().v, getHalfSize(), start.v,dir.v, result.v)){
		if (meshCheck){//Si ha colisionado con esfera, comprobamos si ha colisionado con mesh	
			mesh->collisionModel->setTransform(model.m);
			if (mesh->collisionModel->rayCollision(start.v, (dir.normalize()).v, true, 0.0F, (dir).length())){
				mesh->collisionModel->getCollisionPoint(result.v, false);
				return true;
			}
		}
		else{ return true; }
	}
	return false;
}

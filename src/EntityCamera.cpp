#include "EntityCamera.h"

void EntityCamera::render(){

	//glPushMatrix();
	model.set();
	for(int i = 0; i< children.size(); i++){
		children[i]->render();
	}

	//glPopMatrix();

}
//void EntityMesh::update(float dt){}
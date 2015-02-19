#include "EntityBillBoard.h"

void EntityBillBoard::render()
{

	render(0.0);
}

void EntityBillBoard::setup(){
	
	if (texture){
		mesh->createBillboard(texture->width, texture->height, Vector4(0.0, 1.0, 0.0, -1.0));
	}
	else{
		mesh->createBillboard(size.x, size.y, Vector4(0.0, 1.0, 0.0, -1.0));
	}
}

EntityBillBoard::EntityBillBoard()
{
	size = Vector2(1.0, 1.0);
	texture = NULL;
	mesh = new Mesh();
	
}


void EntityBillBoard::render(float distance)
{
	glPushMatrix();
	//
	//aca hay que modificar la model para que apunte a la camara o tenga la orientacion de la camara
	//
	model.set();
	if (texture){
		texture->bind();
	}
		glColor4f(color.x,color.y,color.z,color.a);
		mesh->render();
	if (texture){
		texture->unbind();
	}
		glColor4f(1.0,1.0,1.0,1.0);
		for (unsigned int i = 0; i< children.size(); i++)
		{
			children[i]->render();
		}
		

	glPopMatrix();

}
void EntityBillBoard::update(double elapsed_time)
{

	for(unsigned int i = 0; i< children.size(); i++){
		children[i]->update(elapsed_time);
	}

}

#pragma once
#ifndef ENTITY_BILLBOARD_H
#define ENTITY_BILLBOARD_H

#include "Entity.h"
#include "mesh.h"
#include "texture.h"
#include "game.h"
#include "camera.h"

class EntityBillBoard: public Entity{

	public:
		Mesh* mesh;
		Texture* texture;
		Vector4 color;
		Camera* _cam;

		Vector2 size;
		
		EntityBillBoard();
		void setup();

		virtual void render(void);
		virtual void render(float);
		virtual void update(double);
	
};
#endif
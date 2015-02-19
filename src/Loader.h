#pragma once
#ifndef LOADER_H
#define LOADER_H

#include "State.h"
#include <string>
#include <vector>
#include "texture.h"
#include "mesh.h"
#include "EntityMesh.h"

class Loader:public State{
public:
	//Atributos
	std::vector<std::string> textures;
	std::vector<std::string> meshes;

	EntityMesh* disk;

	int totalText;
	int totalMesh;

	bool sounds;

	//Funciones
	Loader(void);
	~Loader();

	virtual void init();
	virtual void clean();

	virtual void pause(){};
	virtual void resume(){};

	virtual void update(double elapsed_time);
	virtual void render(Camera* cam);

	
};

#endif
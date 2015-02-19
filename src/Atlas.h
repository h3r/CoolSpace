#pragma once
#include "texture.h"
#include "mesh.h"
#include "framework.h"
#include "text.h"

class Atlas
{
	//Esta clase abre un fichero txt con las
	//cordenadas en pixeles de cada textura dentro de un atlas determinado y las traduce a uv
	//luego con solo dar el nombre de la textura puede devolver el uv necesario.
	//el formato del txt en cada linea es "*[nombre] [frame] = [x1] [x2] [y1] [y2]
public:
	Atlas();
	Atlas(std::string filename, Texture* texture);
	~Atlas();

	std::map<std::string,std::vector<Mesh*>> uvs;
	std::string myCoords;

	Texture* myTexture;
	
	int height;
	int width;

	Vector4 toUV(int x1, int y1, int texWidth, int texHeight); //convierte coordenada a uv;
	void loadFile(std::string); //le dice a el atlas donde esta el txt con coordenadas y este se pone a interpretarlo
	Vector4 getCoords(std::string name, int frame); //devuelve un vec4 con uvs
	
};
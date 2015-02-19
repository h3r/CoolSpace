/*  by Javi Agenjo 2013 UPF  javi.agenjo@gmail.com
The Mesh contains the info about how to render a mesh and also how to parse it from a file.
*/

#ifndef MESH_H
#define MESH_H

#include <vector>
#include "framework.h"

//
#include "text.h"
#include <map>
#include <string>

#include "coldet\src\coldet.h"

class Mesh
{
public:

	std::vector< Vector3 > vertices; //here we store the vertices
	std::vector< Vector3 > normals;	 //here we store the normals
	std::vector< Vector2 > uvs;	 //here we store the texture coordinates
	std::vector< Vector4 > color;

	CollisionModel3D* collisionModel;
	int primitive; //used to tell which primitive to use when rendering (quad, points, lines)

	Mesh();
	void clear();
	void render();

	void createPlane(float size);
	void createFromAse(std::string);
	void binarySave(std::string, std::vector< Vector3 >, std::vector< Vector3 >, std::vector< Vector2 >, Vector3 centro, float radio);
	bool createFromBin(std::string);

	//manager
	static Mesh* LOAD(std::string filename);
	//estaria bueno implementar un "unload" para recursos que sepamos que no van a volver a usarse y etc.
	//pero dejemoslo para cuando veamos que las cosas van lento


	//Sphere data
	Vector3 center;
	float halfSize;

	void createBullet(Vector3 principio, Vector3 fin);
	void createBillboard(int texWidth, int texHeight, Vector4 input_uvs);

	float height;
	float width;

private:
	static std::map<std::string,Mesh*> meshes;
};

#endif
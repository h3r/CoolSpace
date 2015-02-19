#include "mesh.h"
#include <cassert>
#include "includes.h"


std::map<std::string,Mesh*> Mesh::meshes;

Mesh* Mesh::LOAD(std::string filename){
	if(meshes.count(filename)>0){
		//si encuentra algo, lo puede devolver
		return meshes[filename];
		
	}else{
		//sin embargo, si no...
		Mesh* tMesh = new Mesh();
		tMesh->createFromAse(filename);
		meshes.emplace(filename,tMesh);
		return tMesh;
	}

}

Mesh::Mesh()
{
	primitive = GL_TRIANGLES;
}

void Mesh::clear()
{
	vertices.clear();
	normals.clear();
	uvs.clear();
	//color.clear();
}

void Mesh::render()
{

	//assert(vertices.size() && "No vertices in this mesh");

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, &vertices[0] );

	if (normals.size())
	{
		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer(GL_FLOAT, 0, &normals[0] );
	}

	if (uvs.size())
	{
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2,GL_FLOAT, 0, &uvs[0] );
	}
	/*
	if (color.size())
	{
		glEnableClientState(GL_COLOR_ARRAY);
		glTexCoordPointer(4, GL_FLOAT, 0, &color[0]);
	}*/

	glDrawArrays(primitive, 0, vertices.size() );
	glDisableClientState(GL_VERTEX_ARRAY);

	if (normals.size())
		glDisableClientState(GL_NORMAL_ARRAY);
	if (uvs.size())
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	/*if (color.size())
		glDisableClientState(GL_COLOR_ARRAY);*/

}

void Mesh::createBillboard(int texWidth, int texHeight, Vector4 input_uvs){
	vertices.clear();
	normals.clear();
	uvs.clear();


	//centramos
	width = float(texWidth);
	height = float(texHeight);

	//create six vertices (3 for upperleft triangle and 3 for lowerright)
	vertices.push_back(Vector3(width, 0, height));
	vertices.push_back(Vector3(width, 0, -height));
	vertices.push_back(Vector3(-width, 0, -height));

	vertices.push_back(Vector3(-width, 0, height));
	vertices.push_back(Vector3(width, 0, height));
	vertices.push_back(Vector3(-width, 0, -height));

	//all of them have the same normal
	normals.push_back(Vector3(0, 1, 0));
	normals.push_back(Vector3(0, 1, 0));
	normals.push_back(Vector3(0, 1, 0));
	normals.push_back(Vector3(0, 1, 0));
	normals.push_back(Vector3(0, 1, 0));
	normals.push_back(Vector3(0, 1, 0));

	float tUVs[] = { input_uvs.v[0], input_uvs.v[1], input_uvs.v[2], input_uvs.v[3] };
	//texture coordinates
	uvs.push_back(Vector2(tUVs[1], tUVs[3]));
	uvs.push_back(Vector2(tUVs[1], tUVs[2]));
	uvs.push_back(Vector2(tUVs[0], tUVs[2]));
	uvs.push_back(Vector2(tUVs[0], tUVs[3]));
	uvs.push_back(Vector2(tUVs[1], tUVs[3]));
	uvs.push_back(Vector2(tUVs[0], tUVs[2]));


}

void Mesh::createPlane(float size)
{
	vertices.clear();
	normals.clear();
	uvs.clear();

	//create six vertices (3 for upperleft triangle and 3 for lowerright)
	vertices.push_back( Vector3(size,0,size) );
	vertices.push_back( Vector3(size,0,-size) );
	vertices.push_back( Vector3(-size,0,-size) );
	vertices.push_back( Vector3(-size,0,size) );
	vertices.push_back( Vector3(size,0,size) );
	vertices.push_back( Vector3(-size,0,-size) );

	//all of them have the same normal
	normals.push_back( Vector3(0,1,0) );
	normals.push_back( Vector3(0,1,0) );
	normals.push_back( Vector3(0,1,0) );
	normals.push_back( Vector3(0,1,0) );
	normals.push_back( Vector3(0,1,0) );
	normals.push_back( Vector3(0,1,0) );

	//texture coordinates
	uvs.push_back( Vector2(0,1) );
	uvs.push_back( Vector2(0,0) );
	uvs.push_back( Vector2(1,0) );
	uvs.push_back( Vector2(1,1) );
	uvs.push_back( Vector2(0,1) );
	uvs.push_back( Vector2(1,0) );
}

void Mesh::createBullet(Vector3 principio, Vector3 fin){
	vertices.push_back(principio);
	vertices.push_back(fin);
}


void Mesh::createFromAse(std::string filename){
	text* tempText = new text();
	std::string binfilename = filename;
	const char* filenametxt = (filename.append(".ASE")).c_str();
	if(createFromBin(binfilename)){return;}
	if (!tempText->create(filenametxt)){
		std::cerr << "File not found: " << filenametxt << std::endl; 
		exit(1);
	}
	
	////////////////////////
	//Notas sobre text.cpp:
	//text->seek("texto") hace que la variable text apunte a la siguiente ocurrencia de lo que le pedimos, pero no devuelve nada.
	//tempText->seek("PATATA") nos enviaria a la primera linea que encuentre apartir de la posicion actual donde encuentre este texto; 
	//tempText->getXXX() nos devolveria un string/int/float correspondiente a el siguiente contenido a partir de lo que haya encontrado en seek;
	//y haria que text apunte a la siguiente variable.
	////////////////////////

	/*Cargamos los vertices indexados en un vector temporal tVertices y en vertices 
	cargamos posteriormente los vertices de cada cara en un orden de óptimo procesado*/
	std::vector< Vector3 > tVertices; 
	tempText->seek("*MESH_NUMVERTEX");
	int qVert = 0;
	qVert = tempText->getint();
	tempText->seek("*MESH_NUMFACES");
	int qFac = tempText->getint();
	tVertices = std::vector<Vector3>(qVert); //nuestro vector tendra tantas posiciones como vertices
	
	Vector3 tCenter= Vector3(0.0,0.0,0.0);

	for (int i = 0; i < qVert; i++){
		tempText->seek("*MESH_VERTEX");
		tempText->getword(); //ignoramos el numero de vertices
		//como dijo el profesor en clase, intercambiamos x con y para rotar 90º el mesh
		//correcion: creo que el profe se confundio, habia que intercambiar y con z
		
		float tx = tempText->getfloat();
		float tz = tempText->getfloat();
		float ty = tempText->getfloat();
		tVertices[i].set(tx, ty, -tz);
		
		//sumamos todas las posiciones
		tCenter = tCenter + tVertices[i];
	}

	//haciendo el promedio obtenemos el centro
	center.set(tCenter.x/qVert,tCenter.y/qVert,tCenter.z/qVert);

	//ahora calculamos HalfSize
	halfSize = 0;
	for (int i = 0; i < qVert; i++){
	float distance = tVertices[i].distance(center);
		if (distance>halfSize){
			halfSize = distance;
		}
	}

	/*ahora parseamos las caras, inicializamos nuestro vector de vertices.
	por cada cara iteramos por nuestro triangulo de vertices temporal para rellenar el definitivo*/
	vertices = std::vector<Vector3>(qFac*3);
	for (int i = 0; i < qFac; i++){
		tempText->seek("*MESH_FACE");//vamos a la siguiente linea
		tempText->getword();//ignoramos numero de cara
		tempText->getword();/*A:*/int ta = tempText->getint();
		tempText->getword();/*B:*/int tb = tempText->getint();
		tempText->getword();/*C:*/int tc = tempText->getint();
		
		int index=i*3;
		vertices[index]		= tVertices[ta];
		vertices[index+1]	= tVertices[tb];
		vertices[index+2]	= tVertices[tc];
	}
	
	//Rellenar las UV's
	tempText->seek("*MESH_NUMTVERTEX");
	qVert = tempText->getint();
	std::vector< Vector2 > tUVVerts; 
	tUVVerts = std::vector<Vector2>(qVert);

	for (int i = 0; i < qVert; i++){
		tempText->seek("*MESH_TVERT");//vamos a la siguiente linea
		tempText->getword(); //ignoramos el numero de vertice
		float tx = tempText->getfloat();
		float ty = tempText->getfloat();
		
		tUVVerts[i].set(tx, ty);
	}
	
	tempText->seek("*MESH_NUMTVFACES");
	qFac = tempText->getint();
	uvs = std::vector<Vector2>(qFac*3);
	for (int i = 0; i < qFac; i++){
		tempText->seek("*MESH_TFACE");
		tempText->getword();//ignoramos numero de cara
		int ta = tempText->getint();
		int tb = tempText->getint();
		int tc = tempText->getint();
		
		int index=i*3;
		uvs[index]		= tUVVerts[ta];
		uvs[index+1]	= tUVVerts[tb];
		uvs[index+2]	= tUVVerts[tc];
	}

	//rellenar normales
	normals = std::vector<Vector3>(qFac*3);
	for( int i = 0; i < qFac*3; i++){
		tempText->seek("*MESH_VERTEXNORMAL");
		tempText->getword();//ignoramos numero de vertice
		float nx = tempText->getfloat();
		float ny = tempText->getfloat();
		float nz = tempText->getfloat();

		normals[i].set(nx,ny,nz);
	}

	//Añadir el modelo de colision
	collisionModel = newCollisionModel3D();
	collisionModel->setTriangleNumber(qFac);

	for (int i = 0; i < qFac; i = i + 3){
		collisionModel->addTriangle(
			vertices.at(i).x, vertices.at(i).y, vertices.at(i).z,				//Vertice 1
			vertices.at(i + 1).x, vertices.at(i + 1).y, vertices.at(i + 1).z,	//Vertice 2
			vertices.at(i + 2).x, vertices.at(i + 2).y, vertices.at(i + 2).z	//Vertice 3
			);
	}
	collisionModel->finalize();

	//Guardamos el contenido en un fichero binario para permitir una carga posterior más rapida
	binarySave(binfilename,vertices,normals,uvs, center, halfSize);

}
void Mesh::binarySave(std::string filename, std::vector< Vector3 > vertices, std::vector< Vector3 > normals, std::vector< Vector2 > uvs, Vector3 centro, float radio){
	const char* filenameBIN = (filename.append(".bin")).c_str();
	FILE *binaryFile = fopen( filenameBIN, "wb" ); //Abrimos el fichero 'loquesea.bin' en modo write-binary
	//Guardamos todos los vectores cargados.

	int dataSize = vertices.size();
	fwrite (&dataSize,sizeof(int),1,binaryFile);
	fwrite (&vertices[0] , sizeof(Vector3), vertices.size(), binaryFile);
	fwrite (&normals[0] , sizeof(Vector3), normals.size(), binaryFile);
	fwrite (&uvs[0] , sizeof(Vector2), uvs.size(), binaryFile);
	
	///////////////////////////////////////////////////
	//datos para no tener que recalcular centro y radio
	fwrite(&centro, sizeof(Vector3), 1, binaryFile);
	fwrite(&radio, sizeof(float), 1, binaryFile);
	///////////////////////////////////////////////////

	std::cout << "Binary file created: " << filenameBIN << std::endl;
	fclose(binaryFile);
}
bool Mesh::createFromBin(std::string filename){
	const char* filenamebin = (filename.append(".bin")).c_str();
		
	FILE *binFile = fopen (filenamebin, "rb");
	if(binFile == NULL){
		std::cerr << "Binary file not found: " << filenamebin << " \nTrying from other source" << std::endl;
		return false;
	}
	std::cout<<"Loaded From Binary File"<<std::endl;
	int dataSize = 0;
	//Leemos los meta-datos
	fread(&dataSize,sizeof(int), 1 ,binFile); //Meta-info
	//Leemos los vertices
	vertices = std::vector<Vector3>(dataSize);
	fread(&vertices[0],sizeof(Vector3), dataSize,binFile);
	//Leemos las normales
	normals = std::vector<Vector3>(dataSize);
	fread(&normals[0],	sizeof(Vector3), dataSize,binFile);
	//Leemos los uv's
	uvs = std::vector<Vector2>(dataSize);
	fread(&uvs[0],		sizeof(Vector2), dataSize,binFile);


	////////////
	//Leemos el centro
	center = Vector3(0,0,0);
	fread(&center, sizeof(Vector3), 1, binFile);
	halfSize = 0.0;
	fread(&halfSize, sizeof(float), 1, binFile);
	///////////

	collisionModel = newCollisionModel3D();

	int qFac = dataSize;
	collisionModel->setTriangleNumber(qFac);

	for (int i = 0; i < qFac; i = i + 3){
		collisionModel->addTriangle(
			vertices.at(i).x, vertices.at(i).y, vertices.at(i).z,				//Vertice 1
			vertices.at(i + 1).x, vertices.at(i + 1).y, vertices.at(i + 1).z,	//Vertice 2
			vertices.at(i + 2).x, vertices.at(i + 2).y, vertices.at(i + 2).z	//Vertice 3
			);
	}
	collisionModel->finalize();

	fclose(binFile);
	return true;	
}


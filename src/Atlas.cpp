#include "Atlas.h"

Atlas::Atlas(std::string filename, Texture* texture){
	myTexture = texture;
	width = texture->width;
	height = texture->height;
	std::cout << "Aqui esta el ancho de la texture:" << texture->width;
	uvs.clear();
	loadFile(filename);

}

Atlas::~Atlas(){
	
}

void Atlas::loadFile(std::string filename){
	text* tempText = new text();

	if (!tempText->create(filename.c_str())){
		std::cerr << "File not found: " << filename << std::endl;
		exit(1);
	}

	std::string current;
	int frameNum;
	int x1, y1, texHeight, texWidth;


	while (1){//no logre ver como parar al desgraciado
		
		current = tempText->getword();
		if (current == "STOP"){ break; }
		frameNum = tempText->getint();
		tempText->getword(); //skipieamos el =
		x1 = tempText->getint();
		y1 = tempText->getint();
		texWidth = tempText->getint();
		texHeight = tempText->getint();
		
		
		Vector4 tUV = toUV(x1, y1, texWidth, texHeight);

		Mesh* tMesh = new Mesh();
		tMesh->createBillboard(texWidth, texHeight, tUV);

		if (uvs.find(current) == uvs.end()) {
			uvs.insert(std::pair<std::string, std::vector<Mesh*>>(current, { tMesh }));
		}
		else {
			uvs.at(current).push_back(tMesh);	
		}

	}
}

Vector4 Atlas::getCoords(std::string name, int frame){
	return Vector4(1, 1, 1, 1);
}


Vector4 Atlas::toUV(int x1, int y1, int texWidth, int texHeight){
	
	float translatedX1=  float(x1) / float(width);
	float translatedX2 =  float(x1+texWidth) / float(width);
	float translatedY1 =  1- float(y1) / float(height);
	float translatedY2 =  1- float(y1 + texHeight) / float(height);

	return Vector4(translatedX1, translatedX2, translatedY1, translatedY2);
}
#include "UIManager.h"

UIManager::UIManager(){
}

UIManager::UIManager(int qty, Camera* cam){


	camera = cam; 

	particles_vector = std::vector<UIElement*>(qty);
	//initialize particle
	for (int i = 0; i < qty; i++){
		particles_vector[i] = new UIElement();
		particles_vector[i]->ttl = -1;
		particles_vector[i]->type = 0;
		particles_vector[i]->pos = Vector3(0.0, 0.0, 0.0);
		particles_vector[i]->inUse = false;
	}
	max_position = qty;
	next_position = 0;

	
	
}

void UIManager::loadTexture(std::string filename){
	UIElements.emplace(filename, textures.size());
	textures.push_back(Texture::LOAD("assets/UI/" + filename + ".tga"));
}

inline bool comparer(const UIElement* x, const UIElement* y) {
	//nos fijamos cual esta mas cerca y cual esta mas lejos
	return x->camera_dist>y->camera_dist;
}

void UIManager::render(){

	
		//get how far away are from the camera
		UIElement* tParticle;
		Vector3 lookAt = camera->eye;
		unsigned int a = particles_vector.size();
		for (unsigned int i = 0; i < particles_vector.size(); i++) {
			tParticle = particles_vector[i];
			tParticle->camera_dist = (tParticle->pos - lookAt).length();

			//aqui deberiamos orientarlos a la camara tambien?
		}

		//and order them!
		std::sort(particles_vector.begin(), particles_vector.end(), comparer);

		Matrix44 model;
		tMesh.clear();

		Vector3 tVectorP;
		Vector3 tVectorS;
		float tFloatR;
		Vector3 tNormal;
		Vector4 tColor;

		std::vector<Vector3> vertices;
		std::vector<Vector3> normals;
		std::vector<Vector2> uvs;
		std::vector<Vector4> color;


		glDepthMask(GL_FALSE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_DEPTH_TEST);


		int tLastTexture = -1; //aqui guardamos la ultima textura que usamos, si esta repetida no cambiamos, seria un desperdicio.
		for (unsigned int i = 0; i < particles_vector.size(); i++){
			//std::cerr << "\ri: " << i;
			
			/*
			vertexpos0 = (tVectorP + up * tVectorS.x + right * tVectorS.z);
			vertexpos1 = (tVectorP - up * tVectorS.x + right * tVectorS.z);
			vertexpos2 = (tVectorP - up * tVectorS.x - right * tVectorS.z);
			vertexpos3 = (tVectorP + up * tVectorS.x - right * tVectorS.z);
			vertexpos4 = vertexpos0;
			vertexpos5 = vertexpos2;
			*/

			tParticle = particles_vector[i];
		


			if (tParticle->ttl > 0.0){

				tVectorP = tParticle->pos;
				tVectorS = tParticle->size;
				tFloatR = tParticle->rotation;
				tColor = tParticle->current_Color;

				Vector3 up = camera->non_interpolative_up;
				Vector3 top = up;// Vector3(0.0, 1.0, 0.0);
				Vector3 dir = (tParticle->camera_dist > tParticle->size.x) ? (/*camera->non_interpolative_center*/  camera->non_interpolative_eye - tParticle->pos).normalize() : (camera->non_interpolative_eye - camera->non_interpolative_center /*tParticle->pos*/).normalize();
				Vector3 right = (dir.cross(top));
				up = right.cross(dir);

				
				if (tParticle->type == 0){
					tParticle->ttl = -1.0;
					tParticle->inUse = false;
				}

				
				//tColor.a = tColor.a * (tParticle->camera_dist*0.001);


				//si lo ultimo impreso tiene la misma textura, no nos gastemos usemos la misma textura.
				if (tParticle->texture != tLastTexture){
					tLastTexture = tParticle->texture;
					textures[tLastTexture]->bind();
				}

				Vector3 vertexpos0 = Vector3(tVectorP + (up * tVectorS.x) + (right * tVectorS.z));
				Vector3 vertexpos1 = Vector3(tVectorP - (up * tVectorS.x) + (right * tVectorS.z));
				Vector3 vertexpos2 = Vector3(tVectorP - (up * tVectorS.x) - (right * tVectorS.z));
				Vector3 vertexpos3 = Vector3(tVectorP + (up * tVectorS.x) - (right * tVectorS.z));
				Vector3 vertexpos4 = vertexpos0;
				Vector3 vertexpos5 = vertexpos2;

				vertices.push_back(vertexpos0);
				vertices.push_back(vertexpos1);
				vertices.push_back(vertexpos2);
				vertices.push_back(vertexpos3);
				vertices.push_back(vertexpos4);
				vertices.push_back(vertexpos5);


				normals.push_back(Vector3(0, 1, 0));
				normals.push_back(Vector3(0, 1, 0));
				normals.push_back(Vector3(0, 1, 0));
				normals.push_back(Vector3(0, 1, 0));
				normals.push_back(Vector3(0, 1, 0));
				normals.push_back(Vector3(0, 1, 0));


				uvs.push_back(Vector2(1, 1));
				uvs.push_back(Vector2(1, 0));
				uvs.push_back(Vector2(0, 0));
				uvs.push_back(Vector2(0, 1));
				uvs.push_back(Vector2(1, 1));
				uvs.push_back(Vector2(0, 0));

				color.push_back(Vector4(tColor));
				color.push_back(Vector4(tColor));
				color.push_back(Vector4(tColor));
				color.push_back(Vector4(tColor));
				color.push_back(Vector4(tColor));
				color.push_back(Vector4(tColor));

				///
				//lamentablemente como son todos distintos elementos tendremos que usar mas texturas, siendo mucho menos optimo.

				glEnableClientState(GL_VERTEX_ARRAY);
				if (vertices.size()){
					glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);
				}

				if (normals.size())
				{
					glEnableClientState(GL_NORMAL_ARRAY);
					glNormalPointer(GL_FLOAT, 0, &normals[0]);
				}

				if (uvs.size())
				{
					glEnableClientState(GL_TEXTURE_COORD_ARRAY);
					glTexCoordPointer(2, GL_FLOAT, 0, &uvs[0]);
				}

				if (color.size())
				{
					glEnableClientState(GL_COLOR_ARRAY);
					glColorPointer(4, GL_FLOAT, 0, &color[0]);
				}

				glDrawArrays(GL_TRIANGLES, 0, vertices.size());
				glDisableClientState(GL_VERTEX_ARRAY);


				if (normals.size())
					glDisableClientState(GL_NORMAL_ARRAY);
				if (uvs.size())
					glDisableClientState(GL_TEXTURE_COORD_ARRAY);
				if (color.size())
					glDisableClientState(GL_COLOR_ARRAY);

			}

		}

		//desactivamos flags

		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
	
}



void UIManager::update(float elapsed_time){

	for (unsigned int i = 0; i < max_position; i++){
		UIElement* tParticle = particles_vector[i];
		if (tParticle->ttl>=0){
			if (tParticle->type != 0){
				tParticle->ttl -= elapsed_time;
			}
		}
	}
}

UIElement* UIManager::createUI(Vector3 pos, std::string _texture){
	UIElement* tParticle;
	do{

		if (next_position >= max_position){
			next_position = 0;
		}
		tParticle = particles_vector[next_position];
		next_position++;

	} while (tParticle->inUse);
	tParticle = particles_vector[next_position];
	tParticle->inUse = true;
	next_position++;

		tParticle->type = 0;
		tParticle->texture = UIElements[_texture];
		tParticle->ttl = 1; //clouds don't die from time they cycle from 1 to 10;
		tParticle->pos = pos;
			
		tParticle->size.set(1.0,0.0,1.0);
		tParticle->rotation =0.5;
		tParticle->current_Color = Vector4(1.0,1.0,1.0,1.0);
		return tParticle;
}


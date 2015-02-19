#include "ParticleManager.h"


ParticleManager* ParticleManager::instance = NULL;




ParticleManager::ParticleManager(){


}
ParticleManager::~ParticleManager(){

}

void ParticleManager::setup(int qty,Atlas* inputAtlas, Camera* cam){


	atlas = inputAtlas;
	camera = cam; 

	particles_vector = std::vector<Particle*>(qty);
	//initialize particle
	for (int i = 0; i < qty; i++){
		particles_vector[i] = new Particle();
		particles_vector[i]->ttl = -1.5;
		particles_vector[i]->life_time = 0;
		particles_vector[i]->kind = "CLOUD";
		particles_vector[i]->frame = 0;

		///
		particles_vector[i]->cameraType = 1; //hay que arreglar la 0;
		particles_vector[i]->type = 1;
		particles_vector[i]->kind = "FLARE";
		particles_vector[i]->frame = 0;
		particles_vector[i]->life_time = -1.5; particles_vector[i]->ttl = -1.5;
		particles_vector[i]->start_Color = Vector4(0.1, 0.1, 0.1, 1.0);
		particles_vector[i]->end_color = Vector4(0.1, 0.1, 0.0, 0.1);
		particles_vector[i]->start_size = Vector3(1.0, 0.0, 1.0);// .random(0.5);
		particles_vector[i]->end_size = Vector3(0.0, 0.0, 0.0);
		///

	}
	max_position = qty;
	next_position = 0;


	//count size of animations and etc
	
}

inline bool comparer(const Particle* x, const Particle* y) {
	//nos fijamos cual esta mas cerca y cual esta mas lejos

	return x->camera_dist>y->camera_dist;
}

void ParticleManager::render(){
	texture->bind();
	//get how far away are from the camera
	
	Particle* tParticle;
	Vector3 lookAt = camera->eye;
	unsigned int a = particles_vector.size();
	for (unsigned int i = 0; i<particles_vector.size(); i++) {
		tParticle = particles_vector[i];
		tParticle->camera_dist = (tParticle->pos - lookAt).length();

		//aqui deberiamos orientarlos a la camara tambien?
	}
	
	//and order them!
	//std::sort(particles_vector.begin(), particles_vector.end(), comparer);
	
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

	for (unsigned int i = 0; i < particles_vector.size(); i++){
		//std::cerr << "\ri: " << i;
		tParticle = particles_vector[i];
		if (tParticle->ttl>=0.0){
			
			Mesh* frame = atlas->uvs.at(tParticle->kind)[tParticle->frame];

				tVectorP = tParticle->pos;
				tVectorS = tParticle->size;
				tVectorS.x = tVectorS.x*frame->height;
				tVectorS.z = tVectorS.z*frame->width;
				tColor = tParticle->current_Color;

				Vector3 up = camera->non_interpolative_up;
				Vector3 top;
				Vector3 right;
	
				if (tParticle->cameraType ==0){ // Particulas que se encaran a la camara
					top = up;
					Vector3 dir = (tParticle->camera_dist > tParticle->size.x) ? (/*camera->non_interpolative_center*/  camera->non_interpolative_eye - tParticle->pos).normalize() : (camera->non_interpolative_eye - camera->non_interpolative_center /*tParticle->pos*/).normalize();
					right = (top.cross(dir));
					up = right.cross(dir);
				}
				else{
					Vector3  dir = tParticle->normal;
					right = (top.cross(dir));
					up = right.cross(dir);

					float anguloalfa, angulobeta;
					anguloalfa = tParticle->angle;

					angulobeta = acos((Vector3(right.x, 0.0, right.y).normalize()).dot(right));
					if (right.dot(Vector3(0.0, 1.0, 0.0)) < 0.0){ angulobeta = -angulobeta; }
					right = Vector3().rotateInAxis(anguloalfa + angulobeta, tParticle->normal);

					angulobeta = angulobeta + 0.5*M_PI;
					up = Vector3().rotateInAxis(anguloalfa + angulobeta, tParticle->normal);
					
				}
				

				if (tParticle->type == 0){
					//si es polvo o gas
					tColor.a = tColor.a * (tParticle->camera_dist*0.001);
				}
				else if (tParticle->type == 2){
					tParticle->ttl = -1.5;

				}
				else if (tParticle->type == 4){
					Vector3  dir = tParticle->normal;
					right = (abs(dir.dot(Vector3(0.0, 1.0, 0.0))) == 1.0f) ? Vector3(1.0, 0.0, 0.0).cross(dir) : Vector3(0.0, 1.0, 0.0).cross(dir);
					up = right.cross(dir);
				}
				else if(false){
					Vector3 bullet_pos;
					Vector3 bullet_lastpos;
					Vector3 dir = bullet_pos - bullet_lastpos;
					float dist = dir.length();
					Vector3 unitDir = dir.normalize();

					tVectorP = bullet_lastpos + (unitDir * dist*0.5);
					
				}
				/*
				Vector3 vertexpos0 = Vector3(tVectorP + (up * tVectorS.x) + (right * tVectorS.z));
				Vector3 vertexpos1 = Vector3(tVectorP - (up * tVectorS.x) + (right * tVectorS.z));
				Vector3 vertexpos2 = Vector3(tVectorP - (up * tVectorS.x) - (right * tVectorS.z));
				Vector3 vertexpos3 = Vector3(tVectorP + (up * tVectorS.x) - (right * tVectorS.z));
				Vector3 vertexpos4 = vertexpos0;
				Vector3 vertexpos5 = vertexpos2;*/
				//float originalangle = (up * tVectorS.x) / (right * tVectorS.z);
				
				
				
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


				
				uvs.insert(uvs.end(), frame->uvs.begin(), frame->uvs.end());

				color.push_back(Vector4(tColor));
				color.push_back(Vector4(tColor));
				color.push_back(Vector4(tColor));
				color.push_back(Vector4(tColor));
				color.push_back(Vector4(tColor));
				color.push_back(Vector4(tColor));

				///
				//pos+(top+right)*size



				///
		}
		else{}
		}


	//enviamos la informacion propiamente dicha
	glDisable(GL_CULL_FACE);
	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	//glDisable(GL_DEPTH_TEST);

	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
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


	glEnable(GL_CULL_FACE);

	glDisable(GL_BLEND);
	//glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	texture->unbind();
}



void ParticleManager::update(float elapsed_time){

	for (unsigned int i = 0; i < max_position; i++){

		Particle* tParticle = particles_vector[i];
		tParticle->pos = tParticle->pos + tParticle->velocity*elapsed_time*10;
		if (tParticle->ttl>=0.0){
			if (tParticle->type == 2){
				//particula tipo 2
				if (tParticle->first_update == false){
					tParticle->current_Color = tParticle->start_Color;
					tParticle->size = tParticle->start_size;
					tParticle->first_update == true;
				}
				else{
					tParticle->ttl = -1.5;
					tParticle->first_update == false;
				}
			}else if (tParticle->type==1 || tParticle->type==0|| tParticle->type==4){
				//particula comun
				tParticle->ttl -= elapsed_time;

				float nt = 1.0 - (tParticle->ttl / tParticle->life_time);
				float nnt = 1.0 - nt;

				tParticle->current_Color = tParticle->start_Color*nnt + tParticle->end_color*nt;
				tParticle->size = tParticle->start_size*nnt + tParticle->end_size*nt;

				tParticle->pos= tParticle->pos+tParticle->velocity*elapsed_time;
			}else if(tParticle->type ==3) {
				//son tipo 3 yo-yo
				tParticle->ttl -= elapsed_time;
				float nt = (1.0 - (tParticle->ttl / tParticle->life_time))*M_PI;

					tParticle->current_Color = tParticle->start_Color*(1-sin(nt)) + tParticle->end_color*sin(nt);
					tParticle->size = tParticle->start_size*(1-sin(nt)) + tParticle->end_size*sin(nt);

				


			}
			else if (tParticle->type == 4){}
		}
		else{
			tParticle->ttl = -1.5;
		}
	}
}

Particle* ParticleManager::getParticle(){
	Particle* tParticle;
	
	do{
		
		if (next_position >= max_position){
			next_position = 0;
			tParticle = new Particle();
			tParticle->ttl = -1.5;
			std::cerr << "\r" << particles_vector.size();
			particles_vector.push_back(tParticle);
			max_position++;
		}
		else{
			tParticle = particles_vector[next_position];
			next_position++;
		}
		

	} while (tParticle->ttl > 0);

	tParticle->velocity = Vector3(0.0, 0.0, 0.0);

	
	return tParticle;
}


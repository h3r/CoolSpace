#pragma once
#include "framework.h"
#include "Particle.h"

#include "mesh.h"
#include "UIElement.h"
#include "texture.h"
#include "includes.h"
#include "camera.h"
#include <algorithm>


//
// UIManager gestiona todos los graficos de UI que van a verse en pantalla
// UIElement es un grafico en si que va a dibujarse en pantalla, con su posicion, textura y tamaño
// UIComponent es un "emisor" de UIElement, por el momento su unica funcion es imprimir "targets" en la posicion de su padre(es decir, si ponemos a un UIcomponent, de hijo de cualquier objeto, imprimira una textura Target encima de este) 
// a diferencia de las particulas, los UI elements son un mesh por separado cada uno (ya que tienen texturas separadas). Se podria optimizar, y hacer texturas mas chicas, pero el tema es que el numero de objetos de interfaz sera mucho menor y cuantizable.
// Para modificar el comportamiento de los UIElement, hay que modificar o extender UIComponent, eso te lo dejo a vos Hermann, al menos hasta que logre terminar las particulas.

class UIManager
{
public:

	UIManager();
	UIManager(int qty, Camera* cam);
	void loadTexture(std::string filename); //loads a texture into the UIManager

	std::map<std::string, int> UIElements; //aqui guardamos los numeros de posicion del vector de texturas
	std::vector<Texture*> textures;

	std::vector<UIElement*> particles_vector;
	std::vector<int> dead_positions;

	void render();
	void update(float elapsed_time);
	
	UIElement* createUI(Vector3 pos, std::string); //initializes particle and gives a pointer to it to be configured.

	unsigned int next_position;
	int max_position;
	Camera* camera;

	Mesh tMesh;

};
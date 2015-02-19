#include "State.h"
#include "mesh.h"
#include "game.h"
#include "Level1.h"

class Menu :public State{
public:
		
	EntityMesh* skybox;
	EntityMesh* panelMenu;
	//EntityMesh* flecha;

	int currentOption;	 //3 <start>
						 //2 <dificulty>
						 //1 howToPlay
						 //0 exit
	int difficulty;
	int level;


	Game* game;
	
		

	   //Funciones
	   Menu(void);
	   ~Menu();

	   virtual void init();
	   virtual void clean();

	   virtual void pause(){};
	   virtual void resume(){};

	   virtual void update(double elapsed_time);
	   virtual void render(Camera* cam);
};
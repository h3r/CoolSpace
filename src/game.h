/*  by Javi Agenjo 2013 UPF  javi.agenjo@gmail.com
	This class encapsulates the game, is in charge of creating the game, getting the user input, process the update and render.
*/

#ifndef GAME_H
#define GAME_H
//--------------------------------------------------------------------------------------------
//mapped as in SDL

enum PS3pad
{
	//axis
	L_X = 0,
	L_Y = 1,
	R_X = 3,
	R_Y = 4,
	TRIGGERS = 2, //both triggers share an axis (positive is right, negative is left trigger)

	//buttons
	TRIANGLE_BUTTON = 0,
	CIRCLE_BUTTON = 1,
	CROSS_BUTTON = 2,
	SQUARE_BUTTON = 3,
	L1_BUTTON = 4,
	R1_BUTTON = 5,
	L2_BUTTON = 6,
	R2_BUTTON = 7,
	SELECT_BUTTON = 8,
	L3_BUTTON = 9,
	R3_BUTTON = 10,
	START_BUTTON = 11
};

enum HATState
{
	HAT_CENTERED = 0x00,
	HAT_UP = 0x01,
	HAT_RIGHT = 0x02,
	HAT_DOWN = 0x04,
	HAT_LEFT = 0x08,
	HAT_RIGHTUP = (HAT_RIGHT | HAT_UP),
	HAT_RIGHTDOWN = (HAT_RIGHT | HAT_DOWN),
	HAT_LEFTUP = (HAT_LEFT | HAT_UP),
	HAT_LEFTDOWN = (HAT_LEFT | HAT_DOWN)
};

struct JoystickState
{
	int num_axis;	//num analog sticks
	int num_buttons; //num buttons
	float axis[8]; //analog stick
	char button[16]; //buttons
	HATState hat; //digital pad
	bool press;
	bool release;
};
#include <map>
#include <assert.h>

#include "includes.h"
#include "camera.h"
#include "mesh.h"
#include "texture.h"
#include "shader.h"
#include "framework.h"

//#include "Level1.h"
#include "EntityMovile.h"

#include  "Atlas.h"
#include  "ParticleManager.h"

#include "UIComponent.h"

#include "State.h"
#include "Loader.h"


#include "bass\c\bass.h"


class Game{
	static Game* instance;
	Game();
public:
	
	static Game* getInstance(){
		if (instance == NULL){
			instance = new Game();
		}
		return instance;
	}
	//window
	SDL_Window* window;
	float window_width;
	float window_height;

	//keyboard state
	const Uint8* keystate;

	//mouse state
	int mouse_state; //tells which buttons are pressed
	Vector2 mouse_position; //last mouse position
	Vector2 mouse_delta; //mouse movement in the last frame
	bool mouse_locked; //tells if the mouse is locked (not seen)

	//joystick;
	SDL_Joystick* myJoystick1 = NULL;
	JoystickState joystickstate;
	SDL_Haptic * haptic;

	Camera* camera; //our global camera
	Camera* camera2D; //our UI camera
	Camera* cameraMAP; //our UI camera
	Camera* cameraMiniMap; //our UI camera

	bool map, gui, menu;	float mapangle;

	//World* world;
	int gamestate;

	void setWindow(SDL_Window* window);
	void init( void );
	void render( void );
	void update( double dt );

	void updateCameras(EntityMovile* _player, float elapsed_time);
	void getJoyStickEvents();

	void onKeyPressed( SDL_KeyboardEvent event );
	void onMouseButton( SDL_MouseButtonEvent event );
	void onMouseWheel(SDL_MouseWheelEvent event);

	void setWindowSize(int width, int height);
	
	void quit();

	///////////
	//stateManager

	void changeState(State* state);
	void pushState(State* state);
	void popState();
	void clear();

	//press controls on and offs

	bool gUP;
	bool gDOWN;
	bool gLEFT;
	bool gRIGHT;
	bool gSTART;
	bool gSELECT;

	bool gCROSS;
	bool gCIRCLE;
	bool gSQUARE;
	bool gTRIANGLE;
	
	bool gR1;
	bool gR2;
	bool gL1;
	bool gL2;

	bool dUP;
	bool dDOWN;
	bool dLEFT;
	bool dRIGHT;
	bool dSTART;
	bool dSELECT;

	bool dCROSS;
	bool dCIRCLE;
	bool dSQUARE;
	bool dTRIANGLE;

	bool dR1;
	bool dR2;
	bool dL1;
	bool dL2;

	bool LockOn;
	//channels?
	std::vector<HCHANNEL> sfx;

	//samples?

	HSAMPLE menuMusic;
	HSAMPLE battle1Music;
	HSAMPLE battle2Music;
	HSAMPLE battle3Music;
	HSAMPLE over;
	HSAMPLE win;

	HSAMPLE pew;
	HSAMPLE wrum;
	HSAMPLE boom1;
	HSAMPLE boom2;
	HSAMPLE boom3;
	HSAMPLE boom4;
	HSAMPLE bla;
	HSAMPLE transition;
	HSAMPLE select;
	
	int current;
	bool enforcedMapView;

	std::vector<State*> states;
private:
	






};

#endif 
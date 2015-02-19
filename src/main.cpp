/*  by Javi Agenjo 2013 UPF  javi.agenjo@gmail.com

	MAIN:
	 + This file creates the window and the game instance. 
	 + It also contains the mainloop
	 + This is the lowest level, here we access the system to create the opengl Context
	 + It takes all the events from SDL and redirect them to the game
*/
#include "includes.h"

#include "camera.h"
#include "utils.h"
#include "input.h"

#include "game.h"


double last_time = 0; //this is used to calcule the elapsed time between frames
double time_since_last_update = 0;
double fps = 1.0 / 55.0;
double elapsed_time;

Game* game = NULL;

// ********************************

//create a window using SDL
SDL_Window* createWindow(const char* caption, int width, int height, bool fullscreen=false)
{
	int bpp = 0;

	SDL_Init(SDL_INIT_EVERYTHING);

	//set attributes
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16); //or 24
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	//antialiasing (disable this lines if it goes too slow)
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8 ); //increase to have smoother polygons

	//create the window
	SDL_Window *window = SDL_CreateWindow(
		caption, 100, 100, width, height, 
		SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);

	if(!window)
	{
		fprintf(stderr, "Window creation error: %s\n", SDL_GetError());
		exit(-1);
	}
  
	// Create an OpenGL context associated with the window.
	SDL_GLContext glcontext = SDL_GL_CreateContext(window);


	//in case of exit...
	atexit(SDL_Quit);

	//get events from the queue of unprocessed events
	SDL_PumpEvents(); //without this line asserts could fail on windows

	return window;
}


//The application main loop
void mainLoop()
{
	SDL_Event sdlEvent;
	int x,y;

	SDL_GetMouseState(&x,&y);
	game->mouse_position.set(x,y);
	

	while (1)
	{
		//read keyboard state and stored in keystate
		game->keystate = SDL_GetKeyboardState(NULL);
		
		//render frame
		game->render();

		//update events
		while(SDL_PollEvent(&sdlEvent))
		{
			switch(sdlEvent.type)
				{
					case SDL_QUIT: return; break; //EVENT for when the user clicks the [x] in the corner
					case SDL_MOUSEBUTTONDOWN: //EXAMPLE OF sync mouse input
						game->onMouseButton( sdlEvent.button );
						break;
					case SDL_MOUSEBUTTONUP:
						//...
						break;
					case SDL_MOUSEWHEEL:
						game->onMouseWheel(sdlEvent.wheel);
						break;
					case SDL_KEYDOWN: //EXAMPLE OF sync keyboard input
						game->onKeyPressed( sdlEvent.key );
						break;

					case SDL_WINDOWEVENT:
						switch (sdlEvent.window.event) {
							case SDL_WINDOWEVENT_RESIZED: //resize opengl context
								game->setWindowSize( sdlEvent.window.data1, sdlEvent.window.data2 );
								break;
						}
				}
		}

		//get mouse position and delta
		game->mouse_state = SDL_GetMouseState(&x,&y);
		game->mouse_delta.set( game->mouse_position.x - x, game->mouse_position.y - y );
		game->mouse_position.set(x,y);
		
		
		elapsed_time = (SDL_GetTicks() - last_time) * 0.001;
		last_time = SDL_GetTicks();
		time_since_last_update += elapsed_time;
		do{
			time_since_last_update -= fps;
			game->update(fps);
			
		}while (time_since_last_update > fps);
		
		//check errors in opengl only when working in debug
		#ifdef _DEBUG
			checkGLErrors();
		#endif
	}
	return;
}

int main(int argc, char **argv)
{
	//create the game window (WINDOW_WIDTH and WINDOW_HEIGHT are two macros defined in includes.h

	SDL_Window* window = createWindow(WINDOW_CAPTION, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_FULLSCREEN );
	if (!window)
		return 0;

	//setup the game (game is a global variable)
	game = Game::getInstance();//new Game(window);
	game->setWindow(window);
	game->init();

	//Launch the game
	mainLoop();
	
	//destroy everything and save

	return 0;
}

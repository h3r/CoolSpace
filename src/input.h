#ifndef INPUT_H
#define INPUT_H

class Input{
	public:
		static const Uint8* keystate;

		//mouse state
		static int* mouse_state; //tells which buttons are pressed
		static Vector2* mouse_position; //last mouse position
		static Vector2* mouse_delta; //mouse movement in the last frame
		static bool* mouse_locked; //tells if the mouse is locked (not seen)
};

#endif
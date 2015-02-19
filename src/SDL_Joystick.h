//mapped as in SDL
enum XBOXpad
{
	//axis
	LEFT_ANALOG_X = 0,
	LEFT_ANALOG_Y = 1,
	RIGHT_ANALOG_X = 4,
	RIGHT_ANALOG_Y = 3,
	TRIGGERS = 2, //both triggers share an axis (positive is right, negative is left trigger)

	//buttons
	A_BUTTON = 0,
	B_BUTTON = 1,
	X_BUTTON = 2,
	Y_BUTTON = 3,
	LB_BUTTON = 4,
	RB_BUTTON = 5,
	BACK_BUTTON = 6,
	START_BUTTON = 7,
	LEFT_ANALOG_BUTTON = 8,
	RIGHT_ANALOG_BUTTON = 9
};

enum PS3pad
{
	//axis
	LEFT_ANALOG_X = 0,
	LEFT_ANALOG_Y = 1,
	RIGHT_ANALOG_X = 4,
	RIGHT_ANALOG_Y = 3,
	TRIGGERS = 2, //both triggers share an axis (positive is right, negative is left trigger)

	//buttons
	A_BUTTON = 0,
	B_BUTTON = 1,
	X_BUTTON = 2,
	Y_BUTTON = 3,
	LB_BUTTON = 4,
	RB_BUTTON = 5,
	BACK_BUTTON = 6,
	START_BUTTON = 7,
	LEFT_ANALOG_BUTTON = 8,
	RIGHT_ANALOG_BUTTON = 9
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
};



SDL_Joystick* openJoystick(int num_joystick)
{
	// Initialize the joystick subsystem
	SDL_InitSubSystem(SDL_INIT_JOYSTICK);

	// Check for number of joysticks available
	if (SDL_NumJoysticks() <= num_joystick)
		return NULL;

	// Open joystick and return it
	return SDL_JoystickOpen(num_joystick);
}


JoystickState:getJoystickState(SDL_Joystick* joystick)
{
				  JoystickState state;
				  memset(&state, 0, sizeof(JoystickState)); //clear

				  if (joystick == NULL)
				  {
					  //std::cout << "Error: Joystick not opened" << std::endl;
					  return state;
				  }

				  state.num_axis = SDL_JoystickNumAxes((::SDL_Joystick*) joystick);
				  state.num_buttons = SDL_JoystickNumButtons((::SDL_Joystick*)joystick);

				  if (state.num_axis > 8) state.num_axis = 8;
				  if (state.num_buttons > 16) state.num_buttons = 16;

				  for (int i = 0; i < state.num_axis; ++i) //axis
					  state.axis[i] = SDL_JoystickGetAxis((::SDL_Joystick*) joystick, i) / 32768.0f; //range -32768 to 32768
				  for (int i = 0; i < state.num_buttons; ++i) //buttons
					  state.button[i] = SDL_JoystickGetButton((::SDL_Joystick*) joystick, i);
				  state.hat = (HATState)(SDL_JoystickGetHat((::SDL_Joystick*) joystick, 0) - SDL_HAT_CENTERED); //one hat is enough

				  return state;
}

#ifndef STATE_H
#define STATE_H

#include "camera.h"
#include "includes.h"
#include "framework.h"
#include "EntityMesh.h"
#include "utils.h"

#include "bass\c\bass.h"

class State{

	public:
		virtual void init() = 0;
		virtual void clean() = 0;

		virtual void pause() = 0;
		virtual void resume() = 0;

		//virtual void getEvents() = 0;
		virtual void update(double elapsed_time) = 0;
		virtual void render(Camera* cam) = 0;



		Camera* _cam;
		Camera* _cam2d;
		Camera* _camMini;
		bool shouldExit;
		Vector4 screenColor;// = Vector4(1.0, 1.0, 1.0, 0.0);
		bool entering; //
		Vector4 panelColor;// = Vector4(1.0, 1.0, 1.0, 0.0);
		int auxLevel;
};

#endif
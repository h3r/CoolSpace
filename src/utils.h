/*  by Javi Agenjo 2013 UPF  javi.agenjo@gmail.com
	This contains several functions that can be useful when programming your game.
*/
#ifndef UTILS_H
#define UTILS_H

#include "framework.h"
#include <stdio.h>    //  Standard Input\Output C Library
#include <stdarg.h>   //  To use functions with variables arguments
#include <stdlib.h>   //  for malloc
//requiere  el glut
//General functions **************
long getTime();

//generic purposes fuctions
void drawGrid(float dist);

//check opengl errors
bool checkGLErrors();

void printw(float x, float y, float z, char* format, ...);
void printw(Vector3 pos, char* format, ...);
#endif

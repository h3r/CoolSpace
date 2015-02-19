#include "utils.h"

#ifdef WIN32
	#include <windows.h>
#else
	#include <sys/time.h>
#endif

#include "includes.h"

long getTime()
{
	#ifdef WIN32
		return GetTickCount();
	#else
		struct timeval tv;
		gettimeofday(&tv,NULL);
		return (int)(tv.tv_sec*1000 + (tv.tv_usec / 1000));
	#endif
}

//Draw the grid
void drawGrid(float dist)
{
	int num_lines = 20;
	glLineWidth(1);
	glColor3f(0.5,0.5,0.5);
	glBegin( GL_LINES );
		for (int i = 0; i <= num_lines * 0.5; ++i)
		{
			float a = dist * num_lines * 0.5;
			float b = i * dist;

			if (i == num_lines * 0.5)
				glColor3f(1,1,1);
			else if (i%2)
				glColor3f(0.25,0.25,0.25);
			else
				glColor3f(0.5,0.5,0.5);


			glVertex3f(a,b,-a);
			glVertex3f(-a,b,-a);
			glVertex3f(a,-b,-a);
			glVertex3f(-a,-b,-a);
			glVertex3f(b,a,-a);
			glVertex3f(b,-a,-a);
			glVertex3f(-b,a,-a);
			glVertex3f(-b,-a,-a);
			glVertex3f(a,b,a);
			glVertex3f(-a,b,a);
			glVertex3f(a,-b,a);
			glVertex3f(-a,-b,a);
			glVertex3f(b,a,a);
			glVertex3f(b,-a,a);
			glVertex3f(-b,a,a);
			glVertex3f(-b,-a,a);


			glVertex3f(a,-a,b);
			glVertex3f(-a,-a,b);
			glVertex3f(a,-a,-b);
			glVertex3f(-a,-a,-b);

			glVertex3f(b,-a,a);
			glVertex3f(b,-a,-a);
			glVertex3f(-b,-a,a);
			glVertex3f(-b,-a,-a);

			glVertex3f(-a, a,b);
			glVertex3f(-a, -a,b);
			glVertex3f(-a, a,-b);
			glVertex3f(-a, -a,-b);
			glVertex3f(-a, b,a);
			glVertex3f(-a, b,-a);
			glVertex3f(-a, -b,a);
			glVertex3f(-a, -b,-a);
			glVertex3f(a, a,b);
			glVertex3f(a, -a,b);
			glVertex3f(a, a,-b);
			glVertex3f(a, -a,-b);
			glVertex3f(a, b,a);
			glVertex3f(a, b,-a);
			glVertex3f(a, -b,a);
			glVertex3f(a, -b,-a);
		}
	glEnd();
}

//this function is used to access OpenGL Extensions (special features not supported by all cards)
void* getGLProcAddress(const char* name)
{
	return SDL_GL_GetProcAddress(name);
}


bool checkGLErrors()
{
	GLenum errCode;
	const GLubyte *errString;

	if ((errCode = glGetError()) != GL_NO_ERROR) {
		errString = gluErrorString(errCode);
		std::cerr << "OpenGL Error: " << errString << std::endl;
		return false;
	}

	return true;
}

//-------------------------------------------------------------------------
//  Draws a string at the specified coordinates.
// ejemplo:
//	printf(         "char: %c, decimal: %d, float: %f, string: %s", 'X', 1618, 1.618, "text");
//	printw(x, y, z, "char: %c, decimal: %d, float: %f, string: %s", 'X', 1618, 1.618, "text");
//
//-------------------------------------------------------------------------
//  A pointer to a font style..
//  Fonts supported by GLUT are: GLUT_BITMAP_8_BY_13,
//  GLUT_BITMAP_9_BY_15, GLUT_BITMAP_TIMES_ROMAN_10,
//  GLUT_BITMAP_TIMES_ROMAN_24, GLUT_BITMAP_HELVETICA_10,
//  GLUT_BITMAP_HELVETICA_12, and GLUT_BITMAP_HELVETICA_18.
GLvoid *font_style = GLUT_BITMAP_HELVETICA_12; 
void printw(float x, float y, float z, char* format, ...)
{
	va_list args;   //  Variable argument list
	int len;        // String length
	int i;          //  Iterator
	char * text;    // Text

	//  Initialize a variable argument list
	va_start(args, format);

	//  Return the number of characters in the string referenced the list of arguments.
	// _vscprintf doesn't count terminating '\0' (that's why +1)
	len = _vscprintf(format, args) + 1;

	//  Allocate memory for a string of the specified size
	//text = malloc(len * sizeof(char));
	text = (char*)malloc(len * sizeof(char));

	//  Write formatted output using a pointer to the list of arguments
	vsprintf_s(text, len, format, args);

	//  End using variable argument list
	va_end(args);

	//  Specify the raster position for pixel operations.
	glRasterPos3f(x, y, z);

	//  Draw the characters one by one
	for (i = 0; text[i] != '\0'; i++)
		glutBitmapCharacter(font_style, text[i]);

	//  Free the allocated memory for the string
	free(text);
}
void printw(Vector3 pos, char* format, ...)
{
	va_list args;   //  Variable argument list
	int len;        // String length
	int i;          //  Iterator
	char * text;    // Text

	//  Initialize a variable argument list
	va_start(args, format);

	//  Return the number of characters in the string referenced the list of arguments.
	// _vscprintf doesn't count terminating '\0' (that's why +1)
	len = _vscprintf(format, args) + 1;

	//  Allocate memory for a string of the specified size
	//text = malloc(len * sizeof(char));
	text = (char*)malloc(len * sizeof(char));

	//  Write formatted output using a pointer to the list of arguments
	vsprintf_s(text, len, format, args);

	//  End using variable argument list
	va_end(args);

	//  Specify the raster position for pixel operations.
	glRasterPos3f(pos.x, pos.y, pos.z);

	//  Draw the characters one by one
	for (i = 0; text[i] != '\0'; i++)
		glutBitmapCharacter(font_style, text[i]);

	//  Free the allocated memory for the string
	free(text);
}
#ifndef MAIN_H
	#define MAIN_H
	#include <stdlib.h>
	#include <GL/gl.h>
	#include <GL/glu.h>
	#include <iostream>
	#include "SDL.h"
	#include "bone.h"
	#include "point.h"
	#include "armature.h"
	
	#define SCREEN_WIDTH  800
	#define SCREEN_HEIGHT 800
	#define SCREEN_BPP     16
	
	void Quit(int returnCode);
	int resizeWindow(int width, int height);
	void handleKeyPress(SDL_keysym *keysym);
	void handleMousePress();
	int initGL();
	void initSDL();
	void eventLoop();
	void setUpArmature();
	void drawArmature();
	int drawGLScene();
	void drawAim();
	void setTime();
	void animateBone();
	
	GLfloat secondsElapsed;
	GLint lastTick;
	SDL_Surface *surface;
	bool locSet;
	Point dest;
	int videoFlags;
	Armature arm;
#endif
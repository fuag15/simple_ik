#include "main.h"

int main( int argc, char **argv )
{
	secondsElapsed = 0;
	lastTick = 0;
	locSet = false;
	setUpArmature();
	initSDL();
	initGL();
	resizeWindow(SCREEN_WIDTH, SCREEN_HEIGHT);
	eventLoop();
	Quit(0);
	return 0;
}

void setUpArmature()
{
	arm.addBone(25.0, 45.0);
	arm.addBone(15.0, 0.01);
	arm.addBone(25.0, 0.02);
	arm.addBone(30.0, 0.02);
	arm.setSpeed(1.5);
	return;
}

void drawArmature()
{
	Bone* cursor;
	
	arm.resetCursor();
	glPushMatrix();
	
	while(arm.getNext(cursor))
	{
		glRotatef(cursor -> getOrientation(), 0, 0, 1);
		glBegin(GL_QUADS);
			glColor3f(0.8f, 0.0f, 0.2f);
			glVertex3f(0.0f, 0.0f, 0.0f);
			glVertex3f(0.2 * cursor->getLength(), 2.0f, 0.0f);
			glVertex3f(cursor -> getLength(), 0.0f, 0.0f);
			glVertex3f(0.2 * cursor->getLength(), -2.0f, 0.0f);
		glEnd();
		glTranslatef(cursor -> getLength(), 0, 0);
	}
	
	glPopMatrix();
}

void eventLoop()
{
	SDL_Event event;
	int done = 0;
	int isActive = 1;
	
	while(!done)
	{
		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
				case SDL_ACTIVEEVENT:
					if(event.active.gain == 0)
					{
						isActive = 0;
					}
					else
					{
						lastTick = SDL_GetTicks();
						isActive = 1;
					}
					break;
				case SDL_VIDEORESIZE:
					surface = SDL_SetVideoMode(event.resize.w, event.resize.h, 16, videoFlags);
					if(!surface)
					{
						std::cerr << "Could not get a surface after resize: " << SDL_GetError() << std::endl;
						Quit(1);
					}
					resizeWindow(event.resize.w, event.resize.h);
					break;
				case SDL_KEYDOWN:
					handleKeyPress(&event.key.keysym);
					break;
				case SDL_MOUSEBUTTONDOWN:
					handleMousePress();
					break;
				case SDL_QUIT:
					done = 1;
					break;
				default:
					break;
			}
		}
		
		if (isActive)
		{
			setTime();
			animateBone();
			drawGLScene();
		}
	}
	
	return;
}

void animateBone()
{
	arm.animateStep(secondsElapsed, 0.5);
}

void initSDL()
{
	const SDL_VideoInfo *videoInfo;
	if ( SDL_Init(SDL_INIT_VIDEO) < 0 )
	{
		std::cerr << "Video initialization failed: " << SDL_GetError() << std::endl;
		Quit(1);
	}
	
	videoInfo = SDL_GetVideoInfo();
	if (!videoInfo)
	{
		std::cerr << "Video query failed: " << SDL_GetError() << std::endl;
		Quit(1);
	}
	videoFlags  = SDL_OPENGL; 
	videoFlags |= SDL_GL_DOUBLEBUFFER;
	videoFlags |= SDL_HWPALETTE;
	videoFlags |= SDL_RESIZABLE;
	
	if(videoInfo->hw_available)
	{
		videoFlags |= SDL_HWSURFACE;
	}
	else
	{
		videoFlags |= SDL_SWSURFACE;
	}
	
	if(videoInfo->blit_hw)
	{
		videoFlags |= SDL_HWACCEL;
	}
	
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	
	surface = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, videoFlags );
	
	if(!surface)
	{
		std::cerr << "Video mode set failed: " << SDL_GetError() << std::endl;
		Quit(1);
	}
	
	return;
}

int initGL()
{
	glShadeModel(GL_SMOOTH);
	glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	return 1;
}

int resizeWindow(int width, int height)
{
	GLfloat ratio;
	
	if(height == 0)
	{
		height = 1;
	}
	
	ratio = (GLfloat)width / (GLfloat)height;
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-150, 150, -150, 150);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	return 1;
}

void handleKeyPress(SDL_keysym *keysym)
{
	switch ( keysym->sym )
	{
		case SDLK_ESCAPE:
			Quit( 0 );
			break;
		case SDLK_F1:
			SDL_WM_ToggleFullScreen( surface );
			break;
		default:
			break;
	}
	return;
}

void handleMousePress()
{
	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	glGetIntegerv(GL_VIEWPORT, viewport);
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	int x, y;
	GLfloat z;
	GLdouble w_x, w_y, w_z;
	if(SDL_BUTTON(SDL_GetMouseState(&x, &y)) == SDL_BUTTON_LEFT)
	{
		y = (float)viewport[3] - y;
		glReadPixels(x, y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &z);
		gluUnProject(x, y, z, modelview, projection, viewport, &w_x, &w_y, &w_z);
		locSet = true;
		dest.setX(w_x);
		dest.setY(w_y);
		arm.setDestination(dest);
	}
	else
	{
		locSet = false;
		arm.defaultPose();
	}
	
	return;
}

void Quit(int returnCode)
{
	SDL_Quit();
	exit(returnCode);
	return;
}

void drawAim()
{
	glPushMatrix();
	
	glTranslatef(dest.getX(), dest.getY(), 0.0);
	
	glBegin(GL_QUADS);
		glColor3f(0.8, 0.8, 0.2);
		glVertex3f(-1.0, 1.0, 0.0);
		glVertex3f(1.0, 1.0, 0.0);
		glVertex3f(1.0, -1.0, 0.0);
		glVertex3f(-1.0, -1.0, 0.0);
	glEnd();
	
	glPopMatrix();
}

void setTime()
{
	GLint thisTick = SDL_GetTicks();
	secondsElapsed = (thisTick - lastTick)/1000.0;
	lastTick = thisTick;
}

int drawGLScene()
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glLoadIdentity();
	
	if(locSet)
	{
		drawAim();
	}
	
	drawArmature();
	
	SDL_GL_SwapBuffers();
	
	return 1;
}
//g++ main2.cpp GetawayView.cpp GetawayDoc.cpp BSimulation.cpp OpenGLHelpers.cpp BCar.cpp BGround.cpp BCamera.cpp BObject.cpp BaseClasses.cpp ControllerModule.cpp -o Pakoon -lSDL -lSDL_ttf -lGL -lGLU
#include <iostream>
#include <string>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_ttf.h>

using namespace std;

#include "NewDialogs/Item.h"
#include "NewDialogs/Dialog.h"
#include "ControllerModule.h"
#include "GetawayDoc.h"
#include "GetawayView.h"
//#include "NewDialogs/DlgMainMenu.h"


int main(int argc, char *argv[])
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		cout << "SDL init failed" << endl;
		return 1;
	}

	if(TTF_Init() == -1) {
		cout << "TTF_Init: " << TTF_GetError() << endl;
		return 1;
	}

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	
	SDL_Window *window = SDL_CreateWindow("Pakoon", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 0, 0, SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN_DESKTOP);
	if (window == NULL) {
		cout << "error creating window" << endl;
		return 0;
	}
	SDL_GLContext context = SDL_GL_CreateContext(window);
	
	SDL_GL_SetSwapInterval(0);
	
	CGetawayView gw;
	gw.setWindow(window);
	SDL_GetWindowSize(window, &gw.window_width, &gw.window_height);
	gw.OnCreate();
	
	while (!gw.exit)
	{
		gw.OnDraw();

		SDL_Event event;

		while(SDL_PollEvent(&event))
		{
			if(event.type == SDL_KEYDOWN)
				gw.OnKeyDown(event.key.keysym.sym);
			else if (event.type == SDL_KEYUP)
				gw.OnKeyUp(event.key.keysym.sym);
			else if (event.type == SDL_MOUSEBUTTONDOWN) {
				if (event.button.button == 1)
					gw.OnLButtonDown(event.motion.x, event.motion.y);
			}
			else if (event.type == SDL_MOUSEBUTTONUP) {
				if (event.button.button == 1)
					gw.OnLButtonUp(event.motion.x, event.motion.y);
			}
			else if (event.type == SDL_MOUSEMOTION)
				gw.OnMouseMove(event.motion.x, event.motion.y);
			else if (event.type == SDL_QUIT) 
				gw.exit = true;
		}
	}
	
	TTF_Quit();
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();
	
	return 0;
}

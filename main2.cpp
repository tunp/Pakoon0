//g++ main2.cpp GetawayView.cpp GetawayDoc.cpp BSimulation.cpp OpenGLHelpers.cpp BCar.cpp BGround.cpp BCamera.cpp BObject.cpp BaseClasses.cpp ControllerModule.cpp -o Pakoon -lSDL -lSDL_ttf -lGL -lGLU
#include <iostream>
#include <string>
#include <vector>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <SDL/SDL_ttf.h>

using namespace std;

#include "NewDialogs/Item.h"
#include "NewDialogs/Dialog.h"
#include "ControllerModule.h"
#include "GetawayDoc.h"
#include "GetawayView.h"
//#include "NewDialogs/DlgMainMenu.h"


int main(int argc, char *argv[])
{
	CGetawayView gw;
	
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
	SDL_Quit();
	
	return 0;
}

//g++ main2.cpp GetawayView.cpp GetawayDoc.cpp BSimulation.cpp OpenGLHelpers.cpp BCar.cpp BGround.cpp BCamera.cpp BObject.cpp BaseClasses.cpp ControllerModule.cpp -o Pakoon -lSDL -lSDL_ttf -lGL -lGLU
#include <iostream>
#include <string>
#include <vector>
#include <SDL2/SDL.h>
//#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_ttf.h>

#ifdef __EMSCRIPTEN__
#include <GL/gl.h>
#include <emscripten.h>
extern "C" void initialize_gl4es();
#endif

using namespace std;

#include "NewDialogs/Item.h"
#include "NewDialogs/Dialog.h"
#include "ControllerModule.h"
#include "GetawayDoc.h"
#include "GetawayView.h"
//#include "NewDialogs/DlgMainMenu.h"

void mainLoop(CGetawayView *gw) {
  gw->OnDraw();

  SDL_Event event;

  while(SDL_PollEvent(&event))
  {
    if(event.type == SDL_KEYDOWN) {
      if (!((SDL_KeyboardEvent *) &event)->repeat) {
        gw->OnKeyDown(event.key.keysym.sym);
      }
    } else if (event.type == SDL_KEYUP) {
      gw->OnKeyUp(event.key.keysym.sym);
    } else if (event.type == SDL_MOUSEBUTTONDOWN) {
      if (event.button.button == 1) {
        gw->OnLButtonDown(event.motion.x, event.motion.y);
      }
    } else if (event.type == SDL_MOUSEBUTTONUP) {
      if (event.button.button == 1) {
        gw->OnLButtonUp(event.motion.x, event.motion.y);
      }
    } else if (event.type == SDL_MOUSEMOTION) {
      gw->OnMouseMove(event.motion.x, event.motion.y);
    } else if (event.type == SDL_FINGERDOWN) {
      gw->OnFingerDown(event.tfinger.x, event.tfinger.y, event.tfinger.fingerId);
    } else if (event.type == SDL_FINGERUP) {
      gw->OnFingerUp(event.tfinger.x, event.tfinger.y, event.tfinger.fingerId);
    } else if (event.type == SDL_QUIT) {
      gw->exit = true;
    }
  }
}

#ifdef __EMSCRIPTEN__
void emscripten_loop(void *gw) {
  if (((CGetawayView *)gw)->exit) {
    emscripten_run_script("history.back()");
    emscripten_cancel_main_loop();
  } else {
    mainLoop((CGetawayView *)gw);
  }
}
#endif

int main(int argc, char *argv[])
{
#ifdef __EMSCRIPTEN__
  initialize_gl4es();
#endif
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
	
#ifdef __EMSCRIPTEN__
	SDL_Window *window = SDL_CreateWindow("Pakoon", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_OPENGL);
#else
	SDL_Window *window = SDL_CreateWindow("Pakoon", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 0, 0, SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN_DESKTOP);
#endif
	if (window == NULL) {
		cout << "error creating window" << endl;
		return 0;
	}
	SDL_GLContext context = SDL_GL_CreateContext(window);
	
	CGetawayView gw;
	gw.setWindow(window);
	SDL_GetWindowSize(window, &gw.window_width, &gw.window_height);
	gw.OnCreate();
	
#ifdef __EMSCRIPTEN__
  emscripten_set_main_loop_arg((em_arg_callback_func)emscripten_loop, &gw, 0, 1);
  SDL_GL_SetSwapInterval(0);
#else
  SDL_GL_SetSwapInterval(0);
  while (!gw.exit) {
    mainLoop(&gw);
  }
#endif
	
	TTF_Quit();
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();
	
	return 0;
}

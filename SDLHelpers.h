#ifndef SDLHELPERS_H
#define SDLHELPERS_H

#include <SDL/SDL.h>

static SDL_Rect getRect(Sint16 x, Sint16 y, Uint16 w, Uint16 h) {
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;
	return rect;
}

static SDL_Color getColor(Uint8 r, Uint8 g, Uint8 b) {
	SDL_Color color;
	color.r = r;
	color.g = g;
	color.b = b;
	return color;
}

#endif

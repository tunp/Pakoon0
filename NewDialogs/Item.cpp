#include <SDL/SDL.h>

#include "Item.h"

Item::Item(SDL_Rect pos) {
	//pos.x = 0;
	//pos.y = 0;
	//pos.w = 42*2.235;
	//pos.h = 12*2.235;
	this->pos = pos;
	//init();
}

Item::~Item() {
	//destroy();
}

/*void Item::destroy() {
}*/

/*void Item::init() {
}*/

SDL_Surface *Item::getSurface() {
	return surface;
}

void Item::setSurface(SDL_Surface *surface) {
	this->surface = surface;
}

void Item::setPos(SDL_Rect pos) {
	this->pos = pos;
}

SDL_Rect *Item::getPos() {
	return &pos;
}

void Item::onMousePress(int x, int y) {
}

void Item::onMouseRelease(int x, int y) {
}

void Item::onMouseMove(int x, int y) {
}

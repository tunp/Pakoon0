#include <SDL2/SDL.h>

#include "Item.h"

Item::Item(SDL_Rect pos) {
	//pos.x = 0;
	//pos.y = 0;
	//pos.w = 42*2.235;
	//pos.h = 12*2.235;
	this->pos = pos;
	//init();
  needs_redraw = true;
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
  needs_redraw = true;
	this->surface = surface;
}

void Item::setPos(SDL_Rect pos) {
	this->pos = pos;
}

SDL_Rect *Item::getPos() {
	return &pos;
}

bool Item::onMousePress(int x, int y) {
  return false;
}

void Item::onMouseRelease(int x, int y) {
}

void Item::onMouseMove(int x, int y) {
}

void Item::onFingerDown(int x, int y, int finger_id) {
}

void Item::onFingerUp(int x, int y, int finger_id) {
}

bool Item::needsRedraw() {
  return needs_redraw;
}

void Item::drawDone() {
  needs_redraw = false;
}

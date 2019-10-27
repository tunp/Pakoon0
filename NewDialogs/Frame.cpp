#include <string>
#include <iostream>
#include <vector>

using namespace std;

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "Item.h"
#include "Dialog.h"
#include "Frame.h"

Frame::Frame(SDL_Rect pos) : Item(pos) {
	setSurface(SDL_CreateRGBSurface(0, getPos()->w, getPos()->h, 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000));
	memset((int *) getSurface()->pixels, 0xFFFFFFFF, pos.w * pos.h * 4);
}

Frame::~Frame() {
	SDL_FreeSurface(getSurface());
	for (unsigned int z = 0; z < items.size(); z++) {
		delete items[z];
	}
}

SDL_Surface *Frame::getSurface() {
	draw();
	return surface;
}

void Frame::draw() {
	for (unsigned int z = 0; z < items.size(); z++) {
		SDL_Rect src = *items[z]->getPos();
		SDL_Rect dst = {0,0,0,0};
		if (src.x < 0) {
			src.x *= -1;
		} else {
			dst.x = src.x;
			src.x = 0;
		}
		if (src.y < 0) {
			src.y *= -1; 
		} else {
			dst.y = src.y;
			src.y = 0;
		}
		SDL_BlitSurface(items[z]->getSurface(), &src, surface, &dst);
	}
}

void Frame::addItem(Item *item) {
	items.push_back(item);
}

Item *Frame::getItem(int i) {
	return items[i];
}

void Frame::onMousePress(int x, int y) {
	for (unsigned int z = 0; z < items.size(); z++) {
		items[z]->onMousePress(x - getPos()->x, y - getPos()->y);
	}
}

void Frame::onMouseRelease(int x, int y) {
	for (unsigned int z = 0; z < items.size(); z++) {
		items[z]->onMouseRelease(x - getPos()->x, y - getPos()->y);
	}
}

void Frame::onMouseMove(int x, int y) {
	for (unsigned int z = 0; z < items.size(); z++) {
		items[z]->onMouseMove(x - getPos()->x, y - getPos()->y);
	}
}

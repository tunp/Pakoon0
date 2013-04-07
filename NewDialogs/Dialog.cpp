#include <vector>
#include <iostream>
#include <string>

using namespace std;

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include "Item.h"
#include "Dialog.h"

bool Dialog::loadBackground(string name) {
	background = IMG_Load(name.c_str());
	if (!background)
		return false;
	surface = SDL_CreateRGBSurface(SDL_HWSURFACE, background->w, background->h, 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
	SDL_SetAlpha(background, 0, 0);
	pos.w = background->w;
	pos.h = background->h;
	default_pos = true;
	return true;
}

void Dialog::addItem(Item *item) {
	items.push_back(item);
}

void Dialog::deleteItems() {
	for (unsigned int z = 0; z < items.size(); z++) {
		delete items[z];
	}
}

void Dialog::draw() {
	SDL_BlitSurface(background, NULL, surface, NULL);
	for (unsigned int x = 0; x < items.size(); x++) {
		SDL_BlitSurface(items[x]->getSurface(), NULL, surface, items[x]->getPos());
	}
}

SDL_Surface *Dialog::getSurface() {
	draw();
	return surface;
}

void Dialog::deleteSurfaceAndBackground() {
	delete background;
	delete surface;
}

void Dialog::onMousePress(int x, int y) {
	if (x < 0 || y < 0 || x > surface->w || y > surface->h) {
		exit = true;
		return;
	}
	for (unsigned int z = 0; z < items.size(); z++) {
		items[z]->onMousePress(x, y);
	}
}

void Dialog::onMouseRelease(int x, int y) {
	for (unsigned int z = 0; z < items.size(); z++) {
		items[z]->onMouseRelease(x, y);
	}
}

void Dialog::onMouseMove(int x, int y) {
	for (unsigned int z = 0; z < items.size(); z++) {
		items[z]->onMouseMove(x, y);
	}
}

void Dialog::setPos(SDL_Rect pos) {
	this->pos = pos;
}

SDL_Rect *Dialog::getPos() {
	return &pos;
}

void Dialog::setExit(bool exit) {
	this->exit = exit;
}

bool Dialog::isExit() {
	return exit;
}

bool Dialog::isDefaultPos() {
	return default_pos;
}

void Dialog::setDefaultPos(bool default_pos) {
	this->default_pos = default_pos;
}

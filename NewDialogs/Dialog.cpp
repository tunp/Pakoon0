#include <vector>
#include <iostream>
#include <string>

using namespace std;

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Item.h"
#include "Dialog.h"

bool Dialog::loadBackground(string name) {
	background = IMG_Load(name.c_str());
	if (!background)
		return false;
	surface = SDL_CreateRGBSurface(0, background->w, background->h, 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
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

bool Dialog::needsRedraw() {
  return true; // always drawing, only performance optimizaion on some dialogs which override this method
}

void Dialog::draw() {
  if (background) {
    SDL_BlitSurface(background, NULL, surface, NULL);
  }
	for (unsigned int x = 0; x < items.size(); x++) {
		SDL_BlitSurface(items[x]->getSurface(), NULL, surface, items[x]->getPos());
    items[x]->drawDone();
	}
}

SDL_Surface *Dialog::getSurface() {
  if (needsRedraw()) {
    draw();
  }
  return surface;
}

void Dialog::deleteSurfaceAndBackground() {
	delete background;
	delete surface;
}

bool Dialog::onMousePress(int x, int y) {
	if (x < 0 || y < 0 || x > surface->w || y > surface->h) {
		exit = true;
		return false;
	}
	for (unsigned int z = 0; z < items.size(); z++) {
		items[z]->onMousePress(x, y);
	}
  return false;
}

bool Dialog::onMouseRelease(int x, int y) {
	for (unsigned int z = 0; z < items.size(); z++) {
		items[z]->onMouseRelease(x, y);
	}
  return false;
}

bool Dialog::onMouseMove(int x, int y) {
	for (unsigned int z = 0; z < items.size(); z++) {
		items[z]->onMouseMove(x, y);
	}
  return false;
}

bool Dialog::onFingerDown(int x, int y, int finger_id) {
  if (x < 0 || y < 0 || x > surface->w || y > surface->h) {
    exit = true;
    return false;
  }
  for (unsigned int z = 0; z < items.size(); z++) {
    items[z]->onFingerDown(x, y, finger_id);
  }
  return false;
}

bool Dialog::onFingerUp(int x, int y, int finger_id) {
  for (unsigned int z = 0; z < items.size(); z++) {
    items[z]->onFingerUp(x, y, finger_id);
  }
  return false;
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

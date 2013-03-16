#include <string>
#include <iostream>
#include <vector>

using namespace std;

#include <SDL/SDL.h>

#include "Item.h"
#include "Dialog.h"
#include "Scrollbar.h"

Scrollbar::Scrollbar(int min_value, int max_value, int value, SDL_Rect pos) : Item(pos) {
	vertical = (pos.h > pos.w);
	this->min_value = min_value;
	this->max_value = max_value;
	this->value = value;
	bar_size = 18;
	pressed = false;
	value_changed_func = NULL;
	setSurface(SDL_CreateRGBSurface(SDL_HWSURFACE, getPos()->w, getPos()->h, 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000));
	drawScrollbar();
}

Scrollbar::~Scrollbar() {
	SDL_FreeSurface(getSurface());
}

void Scrollbar::drawScrollbar() {
	SDL_Surface *surface = getSurface();
	int w = getPos()->w;
	int h = getPos()->h;

	Uint32 *p = (Uint32 *) surface->pixels;
	SDL_Color color;
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			int x_or_y = (vertical ? y : x);
			bool bar_pos_test = (x_or_y > getBarPos() && x_or_y < getBarPos() + bar_size);
			
			if (y == 0 || y == h-1 || x == 0 || x == w-1 || bar_pos_test) {
				color.r = 0;
				color.g = 0;
				color.b = 0;
			} else {
				color.r = 255;
				color.g = 255;
				color.b = 255;
			}
			*p = SDL_MapRGB(surface->format, color.r, color.g, color.b);
			p++;
		}
	}
}

void Scrollbar::onMousePress(int x, int y) {
	bool is_in_x = x > getPos()->x && x < getPos()->x + getPos()->w;
	bool is_in_y = y > getPos()->y && y < getPos()->y + getPos()->h;
	if (is_in_x && is_in_y) {
		//setSurface(buttonDown);
		mouse_offset = (vertical ? y : x) - getBarPos();
		pressed = true;
	}
}

void Scrollbar::onMouseRelease(int x, int y) {
	//bool is_in_x = x > getPos()->x && x < getPos()->x + getPos()->w;
	//bool is_in_y = y > getPos()->y && y < getPos()->y + getPos()->h;
	//if (is_in_x && is_in_y && pressed) {
	if (pressed) {
		setValueByBarPos((vertical ? y : x) - mouse_offset);
		drawScrollbar();
		if (value_changed_func)
			(*value_changed_func)(p);
	}
	//setSurface(buttonUp);
	pressed = false;
}

void Scrollbar::onMouseMove(int x, int y) {
	if (pressed) {
		setValueByBarPos((vertical ? y : x) - mouse_offset);
		drawScrollbar();
		if (value_changed_func)
			(*value_changed_func)(p);
	}
}

int Scrollbar::getBarPos() {
	int w_or_h = (vertical ? getPos()->h : getPos()->w);
	return (w_or_h - bar_size) * (value - min_value) / (max_value - min_value);
}

void Scrollbar::setValueByBarPos(int pos) {
	int w_or_h = (vertical ? getPos()->h : getPos()->w);
	value = (max_value - min_value) * pos / (w_or_h - bar_size) + min_value;
	if (value < min_value)
		value = min_value;
	if (value > max_value)
		value = max_value;
}

void Scrollbar::setValueChangedFunc(void (*value_changed_func)(void *p), void *p) {
	this->value_changed_func = value_changed_func;
	this->p = p;
}

int Scrollbar::getValue() {
	return value;
}

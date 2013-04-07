#include <string>
#include <iostream>

using namespace std;

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#include "../SDLHelpers.h"

#include "Item.h"
#include "Button.h"

Button::Button(string text, SDL_Color color, SDL_Rect pos) : Item(pos) {
	this->color = color;
	this->text = text;
	pressed = false;
	selected = false;
	button_func = NULL;
	drawButton();
	selectSurface();
}

Button::~Button() {
	SDL_FreeSurface(button_up);
	SDL_FreeSurface(button_down);
}

/*void Button::init() {
	buttonPressed = false;
	drawButton();
	setSurface(buttonUp);
}*/

/*void Button::destroy() {
	SDL_FreeSurface(buttonUp);
	SDL_FreeSurface(buttonDown);
}*/

void Button::drawButton() {
	//Uint32 color = 0xFFE6DCDC;
	//SDL_Color color = 0xFFB4C8B4;
	//Uint32 color = 0xFFB4B4C8;
	SDL_Color color_reflection = color;
	SDL_Color color_shadow = color;
	color_reflection.r = (int) color_reflection.r + 50 < 255 ? color_reflection.r + 50 : 255;
	color_reflection.g = (int) color_reflection.g + 50 < 255 ? color_reflection.g + 50 : 255;
	color_reflection.b = (int) color_reflection.b + 50 < 255 ? color_reflection.b + 50 : 255;
	color_shadow.r = (int) color_shadow.r - 50 > 0 ? color_shadow.r - 50 : 0;
	color_shadow.g = (int) color_shadow.g - 50 > 0 ? color_shadow.g - 50 : 0;
	color_shadow.b = (int) color_shadow.b - 50 > 0 ? color_shadow.b - 50 : 0;
	
	int w = getPos()->w;
	int h = getPos()->h;
	button_up = SDL_CreateRGBSurface(SDL_HWSURFACE, w, h, 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
	button_down = SDL_CreateRGBSurface(SDL_HWSURFACE, w, h, 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
	Uint32 *p_up = (Uint32 *) button_up->pixels;
	Uint32 *p_down = (Uint32 *) button_down->pixels;
	SDL_Color color_up;
	SDL_Color color_down;
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			if (y == 0 || y == h-1 || x == 0 || x == w-1) {
				color_up.r = 0;
				color_up.g = 0;
				color_up.b = 0;
				color_down = color_up;
			} else if (y == 1 || x == 1) {
				color_up = color_reflection;
				color_down = color_shadow;
			} else if (y == h-2 || x == w-2) {
				color_up = color_shadow;
				color_down = color_reflection;
			} else {
				color_up = color;
				if ((y + x) % 2 == 0) {
					color_down = color;
				} else {
					color_down.r = 255;
					color_down.g = 255;
					color_down.b = 255;
				}
			}
			*p_up = SDL_MapRGB(button_up->format, color_up.r, color_up.g, color_up.b);
			*p_down = SDL_MapRGB(button_down->format, color_down.r, color_down.g, color_down.b);
			p_up++;
			p_down++;
		}
	}
	
	SDL_Color text_color = {0, 0, 0};
	vector<string> fonts;
	fonts.push_back("ariblk.ttf");
	TTF_Font *font = openFont(fonts, 14);
	SDL_Surface *text_surface;
	if (!font)
		cout << "TTF_OpenFont is broken!" << endl;
	if(!(text_surface = TTF_RenderText_Blended(font, text.c_str(), text_color)))
		cout << TTF_GetError() << endl;
	SDL_Rect pos;
	pos.x = button_up->w / 2 - text_surface->w / 2;
	pos.y = button_up->h / 2 - text_surface->h / 2;
	SDL_BlitSurface(text_surface, NULL, button_up, &pos);
	SDL_BlitSurface(text_surface, NULL, button_down, &pos);
	SDL_FreeSurface(text_surface);
	TTF_CloseFont(font);
}

void Button::onMousePress(int x, int y) {
	bool isInX = x > getPos()->x && x < getPos()->x + getPos()->w;
	bool isInY = y > getPos()->y && y < getPos()->y + getPos()->h;
	if (isInX && isInY) {
		pressed = true;
		selectSurface();
	}
}

void Button::onMouseRelease(int x, int y) {
	bool is_in_x = x > getPos()->x && x < getPos()->x + getPos()->w;
	bool is_in_y = y > getPos()->y && y < getPos()->y + getPos()->h;
	if (is_in_x && is_in_y && pressed && button_func) {
		(*button_func)(p, this);
	}
	pressed = false;
	selectSurface();
}

void Button::setButtonFunc(void (*button_func)(void *p, Button *b), void *p) {
	this->button_func = button_func;
	this->p = p;
}

void Button::setText(string text) {
	this->text = text;
}

string Button::getText() {
	return text;
}

void Button::setColor(SDL_Color color) {
	this->color = color;
}

void Button::setSelected(bool selected) {
	this->selected = selected;
	selectSurface();
}

void Button::selectSurface() {
	if (pressed) {
		setSurface(button_down);
	} else {
		if (selected) {
			setSurface(button_down);
		} else {
			setSurface(button_up);
		}
	}
}

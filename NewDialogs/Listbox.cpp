#include <string>
#include <iostream>
#include <vector>

using namespace std;

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "../SDLHelpers.h"

#include "Item.h"
#include "Dialog.h"
#include "Listbox.h"

Listbox::Listbox(vector<string> list, int item_size, SDL_Rect pos) : Item(pos) {
	this->list = list;
	this->item_size = item_size;
	selected = 1;
	setSurface(SDL_CreateRGBSurface(0, getPos()->w, getPos()->h, 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000));
	drawListbox();
}

Listbox::~Listbox() {
	SDL_FreeSurface(getSurface());
}

void Listbox::drawListbox() {
	SDL_Surface *surface = getSurface();
	int w = getPos()->w;
	int h = getPos()->h;

	Uint32 *p = (Uint32 *) surface->pixels;
	SDL_Color color;
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			if (y == 0 || y == h-1 || x == 0 || x == w-1) {
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
	
	vector<string> fonts;
	fonts.push_back("ariblk.ttf");
	fonts.push_back("FreeSans.ttf");
	fonts.push_back("DejaVuSans.ttf");
	TTF_Font *font = openFont(fonts, 14);
	if (!font) 
		cout << "TTF_OpenFont is broken!" << endl;
	else {
		for (int x = 0; x < list.size(); x++) {
			SDL_Color text_color = {0, 0, 0};
			SDL_Surface *text_surface;
			if(!(text_surface = TTF_RenderText_Blended(font, list[x].c_str(), text_color)))
				cout << TTF_GetError() << endl;
			SDL_Rect pos;
			pos.x = 8;
			pos.y = x * item_size;
			SDL_BlitSurface(text_surface, NULL, surface, &pos);
			SDL_FreeSurface(text_surface);
		}
		TTF_CloseFont(font);
	}
}

void Listbox::onMousePress(int x, int y) {
	bool is_in_x = x > getPos()->x && x < getPos()->x + getPos()->w;
	bool is_in_y = y > getPos()->y && y < getPos()->y + getPos()->h;
	if (is_in_x && is_in_y) {
		selected = (y - getPos()->y) / item_size;
		if (value_changed_func)
			(*value_changed_func)(p);
	}
}

void Listbox::setValueChangedFunc(void (*value_changed_func)(void *p), void *p) {
	this->value_changed_func = value_changed_func;
	this->p = p;
}

int Listbox::getSelected() {
	return selected;
}

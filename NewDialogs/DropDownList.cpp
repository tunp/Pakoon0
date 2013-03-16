#include <string>
#include <iostream>
#include <vector>

using namespace std;

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#include "Item.h"
#include "Dialog.h"
#include "DropDownList.h"
#include "DlgDropDownList.h"

DropDownList::DropDownList(vector<string> list, vector<Dialog *> *dialogs, SDL_Rect pos) : Item(pos) {
	this->list = list;
	this->dialogs = dialogs;
	selected = 0;
	pressed = false;
	listbox_item_size = 20;
	setSurface(SDL_CreateRGBSurface(SDL_HWSURFACE, getPos()->w, getPos()->h, 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000));
	drawDropDownList();
}

DropDownList::~DropDownList() {
	SDL_FreeSurface(getSurface());
}

void DropDownList::drawDropDownList() {
	listbox_height = list.size() * listbox_item_size;
	
	SDL_Surface *surface = getSurface();
	int w = getPos()->w;
	int h = getPos()->h;
	Uint32 *p = (Uint32 *) surface->pixels;
	SDL_Color color;
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			if (y == 0 || y == h-1 || x == 0 || x == w-1 || x == w-18) {
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
	
	//drawing arrow
	p = (Uint32 *) surface->pixels;
	int posx = w - 13;
	int posy = h / 2 - 2;
	for (int y = 0; y < 4; y++) {
		for (int x = 0; x < 8 - y * 2; x++) {
			p[(posy + y) * w + (posx + x + y)] = SDL_MapRGB(surface->format, color.r, color.g, color.b);
		}
	}
	
	SDL_Color text_color = {0, 0, 0};
	TTF_Font *font = TTF_OpenFont("/usr/share/fonts/TTF/ariblk.ttf", 14);
	SDL_Surface *text_surface;
	if (!font)
		cout << "TTF_OpenFont is broken!" << endl;
	if(!(text_surface = TTF_RenderText_Blended(font, list[selected].c_str(), text_color)))
		cout << TTF_GetError() << endl;
	SDL_Rect pos;
	pos.x = 8;
	pos.y = surface->h / 2 - text_surface->h / 2;
	SDL_BlitSurface(text_surface, NULL, surface, &pos);
	SDL_FreeSurface(text_surface);
	TTF_CloseFont(font);
}

void DropDownList::onMousePress(int x, int y) {
	bool is_in_x = x > getPos()->x && x < getPos()->x + getPos()->w;
	bool is_in_y = y > getPos()->y && y < getPos()->y + getPos()->h;
	if (is_in_x && is_in_y) {
		//setSurface(buttonDown);
		pressed = true;
	}
}

void DropDownList::onMouseRelease(int x, int y) {
	bool is_in_x = x > getPos()->x && x < getPos()->x + getPos()->w;
	bool is_in_y = y > getPos()->y && y < getPos()->y + getPos()->h;
	if (is_in_x && is_in_y && pressed) {
		//(*buttonFunc)(p);
		cout << "joku parempi ehkä?" << endl;
		vector<Dialog *>::iterator i = dialogs->end();
		i--;
		int height;
		bool add_scrollbar;
		if (listbox_height < 200) {
			height = listbox_height;
			add_scrollbar = false;
		} else {
			height = 200;
			add_scrollbar = true;
		}
		dialogs->push_back(new DlgDropDownList(this, (SDL_Rect) {(*i)->getPos()->x + getPos()->x, (*i)->getPos()->y + getPos()->y + getPos()->h, getPos()->w, height}, add_scrollbar));
	}
	//setSurface(buttonUp);
	pressed = false;
}

vector<string> DropDownList::getList() {
	return list;
}

void DropDownList::setSelected(int selected) {
	this->selected = selected;
	drawDropDownList();
}

string DropDownList::getString() {
	return list[selected];
}

void DropDownList::reset() {
	list.clear();
}

void DropDownList::add(string s) {
	list.push_back(s);
}

bool DropDownList::select(string s) {
	for (int x = 0; x < list.size(); x++) {
		if (list[x].compare(s) == 0) {
			setSelected(x);
			return true;
		}
	}
	return false;
}

int DropDownList::getListboxItemSize() {
	return listbox_item_size;
}

int DropDownList::getListboxHeight() {
	return listbox_height;
}

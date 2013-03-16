#include <string>
#include <vector>
#include <iostream>

using namespace std;

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

#include "Item.h"
#include "Dialog.h"
#include "Scrollbar.h"
#include "Listbox.h"
#include "DropDownList.h"
#include "Frame.h"

#include "DlgDropDownList.h"

DlgDropDownList::DlgDropDownList(DropDownList *ddl, SDL_Rect pos, bool add_scrollbar) : Dialog() {
	this->ddl = ddl;
	this->pos = pos;
	setDefaultPos(false);

	background = SDL_CreateRGBSurface(SDL_HWSURFACE, pos.w, pos.h, 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
	memset((int *) background->pixels, 0xFFFFFFFF, pos.w * pos.h * 4);
	surface = SDL_CreateRGBSurface(SDL_HWSURFACE, background->w, background->h, 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
	SDL_SetAlpha(background, 0, 0);
	
	int listbox_width = pos.w;
	if (add_scrollbar)
		listbox_width -= 18;
	Item *listbox = new Listbox(ddl->getList(), ddl->getListboxItemSize(), (SDL_Rect) {0, 0, listbox_width, ddl->getListboxHeight()});
	((Listbox *) listbox)->setValueChangedFunc(&listboxValueChangedCallback, (void *)this);
	
	Item *item = new Frame((SDL_Rect) {0, 0, listbox_width, pos.h});
	((Frame *)item)->addItem(listbox);
	addItem(item);
	
	if (add_scrollbar) {
		item = new Scrollbar(0, ddl->getListboxHeight() - pos.h, 0, (SDL_Rect) {pos.w - 18, 0, 18, pos.h});
		((Scrollbar *) item)->setValueChangedFunc(&scrollbarValueChangedCallback, (void *)this);
		addItem(item);
	}
	
	setExit(false);
}

DlgDropDownList::~DlgDropDownList() {
	deleteSurfaceAndBackground();
	deleteItems();
}

void DlgDropDownList::listboxValueChangedCallback(void *p) {
	((DlgDropDownList *)p)->listboxValueChanged();
}

void DlgDropDownList::listboxValueChanged() {
	int selected = ((Listbox *)((Frame *) items[0])->getItem(0))->getSelected();
	ddl->setSelected(selected);
	setExit(true);
}

void DlgDropDownList::scrollbarValueChangedCallback(void *p) {
	((DlgDropDownList *)p)->scrollbarValueChanged();
}

void DlgDropDownList::scrollbarValueChanged() {
	int value = ((Scrollbar *) items[1])->getValue();
	SDL_Rect *pos = ((Frame *) items[0])->getItem(0)->getPos();
	pos->y = 0 - value;
}

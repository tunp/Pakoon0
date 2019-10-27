#include <string>
#include <iostream>
#include <vector>

using namespace std;

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "Item.h"
#include "Dialog.h"
#include "Frame.h"
#include "Button.h"
#include "ButtonGroup.h"

ButtonGroup::ButtonGroup(SDL_Rect pos) : Frame(pos) {
}

ButtonGroup::~ButtonGroup() {
}

void ButtonGroup::addItem(Button *button) {
	items.push_back(button);
	button->setButtonFunc(&changeCallback, (void *)this);
}

void ButtonGroup::changeCallback(void *p, Button *b) {
	((ButtonGroup *)p)->change(b);
}

void ButtonGroup::change(Button *b) {
	for (int z = 0; z < items.size(); z++) {
		((Button *)items[z])->setSelected(false);
	}
	value = b->getText();
	b->setSelected(true);
	if (value_changed_func)
		(*value_changed_func)(p, this);
}

void ButtonGroup::setValueChangedFunc(void (*value_changed_func)(void *p, ButtonGroup *bg), void *p) {
	this->value_changed_func = value_changed_func;
	this->p = p;
}

string ButtonGroup::getValue() {
	return value;
}

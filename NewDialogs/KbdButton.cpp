#include <string>
#include <iostream>

using namespace std;

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "KbdButton.h"

KbdButton::KbdButton(string text, SDL_Color color, SDL_Rect pos, int *press_key) : Button(text, color, pos) {
  this->press_key = press_key;
}

int *KbdButton::getPressKey() {
  return this->press_key;
}

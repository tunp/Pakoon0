#ifndef KBDBUTTON_H
#define KBDBUTTON_H
#include "Button.h"

class KbdButton : public Button {
private:
  int *press_key;
public:
  KbdButton(string text, SDL_Color color, SDL_Rect pos, int *press_key);
  int *getPressKey();
};

#endif

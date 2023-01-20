#include <string>
#include <vector>
#include <iostream>

using namespace std;

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "../SDLHelpers.h"

#include "KbdButton.h"
#include "Dialog.h"
#include "Frame.h"
#include "ButtonGroup.h"
#include "DropDownList.h"

#include "../ControllerModule.h"
#include "../GetawayDoc.h"
#include "../GetawayView.h"
#include "../SoundModule.h"

#include "DlgOnScreenKbd.h"

DlgOnScreenKbd::DlgOnScreenKbd(CGetawayView *pView) : Dialog() {
  this->pView = pView;

  pos.x = 0;
  pos.y = 0;
  pos.w = pView->window_width;
  pos.h = pView->window_height;

  surface = SDL_CreateRGBSurface(0, pos.w, pos.h, 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);

  Item *item = new KbdButton("Gas", getColor(0xB4, 0xC8, 0xB4), getRect(pos.w * 0.85, pos.h * 0.85, pos.w * 0.1, pos.h * 0.1 ), &ControllerModule::m_keymap.m_unAccelerate);
  addItem(item);
  item = new KbdButton("Brake", getColor(0xC8, 0xB4, 0xB4), getRect(pos.w * 0.75, pos.h * 0.85, pos.w * 0.1, pos.h * 0.1 ), &ControllerModule::m_keymap.m_unReverse);
  addItem(item);
  item = new KbdButton("Left", getColor(0xDC, 0xDC, 0xE6), getRect(pos.w * 0.05, pos.h * 0.85, pos.w * 0.1, pos.h * 0.1 ), &ControllerModule::m_keymap.m_unLeft);
  addItem(item);
  item = new KbdButton("Right", getColor(0xDC, 0xDC, 0xE6), getRect(pos.w * 0.15, pos.h * 0.85, pos.w * 0.1, pos.h * 0.1 ), &ControllerModule::m_keymap.m_unRight);
  addItem(item);
  item = new KbdButton("Lift", getColor(0xDC, 0xDC, 0xE6), getRect(pos.w * 0.85, pos.h * 0.65, pos.w * 0.1, pos.h * 0.1 ), &ControllerModule::m_keymap.m_unLift);
  addItem(item);
  item = new KbdButton("Heli", getColor(0xDC, 0xDC, 0xE6), getRect(pos.w * 0.85, pos.h * 0.55, pos.w * 0.1, pos.h * 0.1 ), &ControllerModule::m_keymap.m_unHeli);
  addItem(item);
  item = new KbdButton("Jet", getColor(0xDC, 0xDC, 0xE6), getRect(pos.w * 0.85, pos.h * 0.45, pos.w * 0.1, pos.h * 0.1 ), &ControllerModule::m_keymap.m_unJet);
  addItem(item);
  esc_code = SDLK_ESCAPE;
  item = new KbdButton("Esc", getColor(0xDC, 0xDC, 0xE6), getRect(pos.w * 0.05, pos.h * 0.05, pos.w * 0.1, pos.h * 0.1 ), &esc_code);
  addItem(item);
  for (vector<Item *>::iterator i = items.begin(); i != items.end(); i++) {
    Item *item = *i;
    ((KbdButton *) item)->setButtonPressFunc(&keyPressCallback, (void *)this);
    ((KbdButton *) item)->setButtonReleaseFunc(&keyReleaseCallback, (void *)this);
  }

  setExit(false);
}

bool DlgOnScreenKbd::onMousePress(int x, int y) {
  //Dialog::onMousePress(x, y);
  return true;
}

bool DlgOnScreenKbd::onMouseRelease(int x, int y) {
  //Dialog::onMouseRelease(x, y);
  return true;
}

bool DlgOnScreenKbd::onMouseMove(int x, int y) {
  //Dialog::onMouseMove(x, y);
  return true;
}

bool DlgOnScreenKbd::onFingerDown(int x, int y, int finger_id) {
  bool ret = Dialog::onFingerDown(x, y, finger_id);
  setExit(false); // never exit by pressing outside of on screen keyboard
  return ret;
}

DlgOnScreenKbd::~DlgOnScreenKbd() {
  deleteSurfaceAndBackground();
  deleteItems();
}

void DlgOnScreenKbd::keyPressCallback(void *p, Button *b) {
  ((DlgOnScreenKbd *)p)->keyPress(((KbdButton *)b)->getPressKey());
}

void DlgOnScreenKbd::keyPress(int *press_key) {
  pView->OnKeyDown(*press_key);
}

void DlgOnScreenKbd::keyReleaseCallback(void *p, Button *b) {
  ((DlgOnScreenKbd *)p)->keyRelease(((KbdButton *)b)->getPressKey());
}

void DlgOnScreenKbd::keyRelease(int *press_key) {
  pView->OnKeyUp(*press_key);
}

bool DlgOnScreenKbd::needsRedraw() {
  for (unsigned int x = 0; x < items.size(); x++) {
    if (items[x]->needsRedraw()) {
      return true;
    }
  }
  return false;
}

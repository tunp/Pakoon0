#include <string>
#include <vector>
#include <iostream>

using namespace std;

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "../SDLHelpers.h"

#include "Item.h"
#include "Button.h"
#include "Dialog.h"
#include "Frame.h"
#include "ButtonGroup.h"
#include "DropDownList.h"

#include "../ControllerModule.h"
#include "../GetawayDoc.h"
#include "../GetawayView.h"
#include "../SoundModule.h"

#include "DlgMainMenu.h"
#include "DlgVisuals.h"
#include "DlgSounds.h"
#include "DlgControls.h"
#include "DlgOnScreenKbd.h"

DlgMainMenu::DlgMainMenu(CGetawayView *pView) : Dialog() {
	this->pView = pView;
	
	if (!loadBackground("NewDialogs/menu.png"))
		cout << "Background open failed NewDialogs/menu.png!" << endl;
	
	Item *item = new Button("Visuals...", getColor(0xDC, 0xDC, 0xE6), getRect(16, 47, 95, 27));
	((Button *) item)->setButtonFunc(&visualsCallback, (void *)this);
	addItem(item);
	
	item = new Button("Sounds...", getColor(0xDC, 0xDC, 0xE6), getRect(16, 77, 95, 27));
	((Button *) item)->setButtonFunc(&soundsCallback, (void *)this);
	addItem(item);
	
	item = new Button("Controls...", getColor(0xDC, 0xDC, 0xE6), getRect(16, 106, 95, 27));
	((Button *) item)->setButtonFunc(&controlsCallback, (void *)this);
	addItem(item);
	
	item = new Button("Continue", getColor(0xB4, 0xC8, 0xB4), getRect(16, 135, 95, 27));
	((Button *) item)->setButtonFunc(&contCallback, (void *)this);
	addItem(item);
	
	item = new Button("Quit", getColor(0xC8, 0xB4, 0xB4), getRect(16, 178, 95, 27));
	((Button *) item)->setButtonFunc(&quitCallback, (void *)this);
	addItem(item);
	
	setExit(false);
}

DlgMainMenu::~DlgMainMenu() {
	deleteSurfaceAndBackground();
	deleteItems();
}

void DlgMainMenu::visualsCallback(void *p, Button *b) {
	((DlgMainMenu *)p)->visuals();
}

void DlgMainMenu::visuals() {
	pView->dialogs.push_back(new DlgVisuals(pView));
}

void DlgMainMenu::soundsCallback(void *p, Button *b) {
	((DlgMainMenu *)p)->sounds();
}

void DlgMainMenu::sounds() {
	pView->dialogs.push_back(new DlgSounds(pView));
}

void DlgMainMenu::controlsCallback(void *p, Button *b) {
	((DlgMainMenu *)p)->controls();
}

void DlgMainMenu::controls() {
	pView->dialogs.push_back(new DlgControls(pView));
}

void DlgMainMenu::contCallback(void *p, Button *b) {
	((DlgMainMenu *)p)->cont();
}

void DlgMainMenu::cont() {
	setExit(true);
}

void DlgMainMenu::quitCallback(void *p, Button *b) {
	((DlgMainMenu *)p)->quit();
}

void DlgMainMenu::quit() {
	if(pView) {
		/*DEVMODE devmode;
		EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &devmode);
		if((devmode.dmPelsWidth != g_devmodeorig.dmPelsWidth) || 
		   (devmode.dmPelsHeight != g_devmodeorig.dmPelsHeight) || 
		   (devmode.dmBitsPerPel != g_devmodeorig.dmBitsPerPel) ||
		   (devmode.dmDisplayFrequency != g_devmodeorig.dmDisplayFrequency)) {
		  ChangeDisplaySettings(&g_devmodeorig, 0);
		  AfxGetMainWnd()->SetWindowPos(NULL, -2, -2, g_devmodeorig.dmPelsWidth + 4, g_devmodeorig.dmPelsHeight + 4, 0);
		}*/
		SoundModule::StopSkidSound();
		SoundModule::StopJetSound();
		SoundModule::StopEngineSound();
		SoundModule::StartMenuMusic();

		pView->m_pDrawFunction = &CGetawayView::OnDrawStartMenu;
		pView->m_bDrawOnlyMenu = false;
		pView->m_bFullRedraw = true;
    vector<Dialog *>::iterator kbdi = pView->getFirstDialogOfType<DlgOnScreenKbd *>();
    if (kbdi != pView->dialogs.end()) {
      pView->dialogs.erase(kbdi);
    }
		//pView->InvalidateRect(NULL);
	}
	setExit(true);
}

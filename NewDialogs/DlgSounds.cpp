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
#include "Scrollbar.h"
#include "ButtonGroup.h"

#include "../ControllerModule.h"
#include "../GetawayDoc.h"
#include "../GetawayView.h"
#include "../SoundModule.h"

#include "DlgSounds.h"

DlgSounds::DlgSounds(CGetawayView *pView) : Dialog() {
	this->pView = pView;
	
	m_rbSoundSpace = SoundModule::GetSoundSpace();
	
	if (!loadBackground("NewDialogs/sounds.png"))
		cout << "Background open failed!" << endl;
		
	Item *item = new Scrollbar(0, 255, SoundModule::GetMenuMusicVolume(), getRect(180, 147, 231, 22));
	addItem(item);
	
	item = new Scrollbar(0, 255, SoundModule::GetVehicleSoundsVolume(), getRect(180, 179, 231, 22));
	addItem(item);
	
	item = new ButtonGroup(getRect(178, 205, 235, 27));
	
	for (int x = 0; x < 2; x++) {
		string values[] = {"Lame", "3D"};
		Button *button = new Button(values[x], getColor(0xF0, 0xF0, 0xF0), getRect(x * 119, 0, 116, 27));
		button->setSelected(x == m_rbSoundSpace);
		((ButtonGroup *)item)->addItem(button);
	}
	
	((ButtonGroup *)item)->setValueChangedFunc(soundSpaceCallback, this);
	addItem(item);
	
	item = new Button("Yeah", getColor(0xB4, 0xC8, 0xB4), getRect(178, 263, 117, 27));
	((Button *) item)->setButtonFunc(&yeahCallback, (void *)this);
	addItem(item);
	
	item = new Button("No Way", getColor(0xC8, 0xB4, 0xB4), getRect(297, 263, 115, 27));
	((Button *) item)->setButtonFunc(&noWayCallback, (void *)this);
	addItem(item);
	
	setExit(false);
}

DlgSounds::~DlgSounds() {
	deleteSurfaceAndBackground();
	deleteItems();
}

void DlgSounds::soundSpaceCallback(void *p, ButtonGroup *bg) {
	string values[] = {"Lame", "3D"};
	int x;
	for (x = 0; bg->getValue() != values[x]; x++);
	((DlgSounds *)p)->m_rbSoundSpace = x;
}

void DlgSounds::yeahCallback(void *p, Button *b) {
	((DlgSounds *)p)->yeah();
}

void DlgSounds::yeah() {
  //UpdateData();

  // Set volumes
  SoundModule::SetMenuMusicVolume(((Scrollbar *)items[0])->getValue());
  SoundModule::SetVehicleSoundsVolume(((Scrollbar *)items[1])->getValue());
  
  // Switch to new sound driver/output if needed
  /*CComboBox *pCombo1 = static_cast<CComboBox*>(GetDlgItem(IDC_COMBO1));
  CComboBox *pCombo2 = static_cast<CComboBox*>(GetDlgItem(IDC_COMBO2));
  CString sNewDriverName;
  pCombo2->GetLBText(pCombo2->GetCurSel(), sNewDriverName);
  if((pCombo1->GetCurSel() != m_nOutput) || 
     (sNewDriverName != m_sDriverName)) {
    SoundModule::StopMenuMusic();
    SoundModule::Close();
    SoundModule::SetOutput(pCombo1->GetCurSel());
    SoundModule::SetDriver(pCombo2->GetCurSel());
    SoundModule::Initialize();
    SoundModule::StartMenuMusic();
  }*/

  // Set other stuff
  SoundModule::SetSoundSpace(m_rbSoundSpace);
  
  setExit(true);
}

void DlgSounds::noWayCallback(void *p, Button *b) {
	((DlgSounds *)p)->setExit(true);
}

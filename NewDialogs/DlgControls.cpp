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
#include "DropDownList.h"

#include "../ControllerModule.h"
#include "../GetawayDoc.h"
#include "../GetawayView.h"
#include "../SoundModule.h"

#include "DlgControls.h"

DlgControls::DlgControls(CGetawayView *pView) : Dialog() {
	this->pView = pView;
	m_pSimulation = pView->pDoc->GetSimulation();
	
	m_rbUse = m_pSimulation->m_nController;
	
	if (!loadBackground("NewDialogs/controls.png"))
		cout << "Background open failed!" << endl;
		
	vector<string> list;
	list.push_back("moi");
	
	for (int x = 0; x < 9; x++) {
		DropDownList *ddl = new DropDownList(list, &pView->dialogs, getRect(158, 142 + (x * 25.5), 94, 24));
		lists.push_back(ddl);
		addItem(ddl);
	}
	
	Item *item = new Button("Yeah", getColor(0xB4, 0xC8, 0xB4), getRect(110, 389, 118, 28));
	((Button *) item)->setButtonFunc(&yeahCallback, (void *)this);
	addItem(item);
	
	item = new Button("No Way", getColor(0xC8, 0xB4, 0xB4), getRect(230, 389, 116, 28));
	((Button *) item)->setButtonFunc(&noWayCallback, (void *)this);
	addItem(item);
	
	if(m_rbUse == 0) {
		OnKeyboard();
	} else {
		//OnController();
	}
	
	setExit(false);
}

DlgControls::~DlgControls() {
	deleteSurfaceAndBackground();
	deleteItems();
}

void DlgControls::yeahCallback(void *p, Button *b) {
	((DlgControls *)p)->yeah();
}

void DlgControls::yeah() {
  //UpdateData();
  m_pSimulation->m_nController = m_rbUse;

  /*CComboBox *pCombo1 = static_cast<CComboBox*>(GetDlgItem(IDC_COMBO1));
  CComboBox *pCombo2 = static_cast<CComboBox*>(GetDlgItem(IDC_COMBO2));
  CComboBox *pCombo3 = static_cast<CComboBox*>(GetDlgItem(IDC_COMBO3));
  CComboBox *pCombo4 = static_cast<CComboBox*>(GetDlgItem(IDC_COMBO4));
  CComboBox *pCombo5 = static_cast<CComboBox*>(GetDlgItem(IDC_COMBO5));
  CComboBox *pCombo6 = static_cast<CComboBox*>(GetDlgItem(IDC_COMBO6));
  CComboBox *pCombo7 = static_cast<CComboBox*>(GetDlgItem(IDC_COMBO7));
  CComboBox *pCombo8 = static_cast<CComboBox*>(GetDlgItem(IDC_COMBO8));
  CComboBox *pCombo9 = static_cast<CComboBox*>(GetDlgItem(IDC_COMBO9));*/

  if(m_rbUse == 0) {
    // update keymap
    ControllerModule::m_keymap.m_unAccelerate = GetKeyFromString(lists[0]->getString());
    ControllerModule::m_keymap.m_unBreak = GetKeyFromString(lists[1]->getString());
    ControllerModule::m_keymap.m_unLeft = GetKeyFromString(lists[2]->getString());
    ControllerModule::m_keymap.m_unRight = GetKeyFromString(lists[3]->getString());
    ControllerModule::m_keymap.m_unReverse = GetKeyFromString(lists[4]->getString());
    ControllerModule::m_keymap.m_unJet = GetKeyFromString(lists[5]->getString());
    ControllerModule::m_keymap.m_unHeli = GetKeyFromString(lists[6]->getString());
    ControllerModule::m_keymap.m_unLift = GetKeyFromString(lists[7]->getString());
    ControllerModule::m_keymap.m_unCamera = GetKeyFromString(lists[8]->getString());
  } else {
    // Update controller map
    /*CString sTmp;
    pCombo1->GetWindowText(sTmp);
    ControllerModule::m_controllermap.m_cmiAccelerate.FromString(sTmp + (m_bFlip1 ? " Flipped" : ""));
    pCombo2->GetWindowText(sTmp);
    ControllerModule::m_controllermap.m_cmiBreak.FromString(sTmp + (m_bFlip2 ? " Flipped" : ""));
    pCombo3->GetWindowText(sTmp);
    ControllerModule::m_controllermap.m_cmiLeft.FromString(sTmp + (m_bFlip3 ? " Flipped" : ""));
    pCombo4->GetWindowText(sTmp);
    ControllerModule::m_controllermap.m_cmiRight.FromString(sTmp + (m_bFlip4 ? " Flipped" : ""));
    pCombo5->GetWindowText(sTmp);
    ControllerModule::m_controllermap.m_cmiReverse.FromString(sTmp + (m_bFlip5 ? " Flipped" : ""));
    pCombo6->GetWindowText(sTmp);
    ControllerModule::m_controllermap.m_cmiJet.FromString(sTmp);
    pCombo7->GetWindowText(sTmp);
    ControllerModule::m_controllermap.m_cmiHeli.FromString(sTmp);
    pCombo8->GetWindowText(sTmp);
    ControllerModule::m_controllermap.m_cmiLift.FromString(sTmp);
    pCombo9->GetWindowText(sTmp);
    ControllerModule::m_controllermap.m_cmiCamera.FromString(sTmp);*/
  }
  
  setExit(true);
}

void DlgControls::noWayCallback(void *p, Button *b) {
	((DlgControls *)p)->setExit(true);
}

int DlgControls::GetKeyFromString(string rsKey) {
  if(rsKey[0] == '<') {
    // Interpret virtual key
    if(rsKey.find("LEFT") != string::npos) {
      return SDLK_LEFT;
    } else if(rsKey.find("RIGHT") != string::npos) {
      return SDLK_RIGHT;
    } else if(rsKey.find("UP") != string::npos) {
      return SDLK_UP;
    } else if(rsKey.find("DOWN") != string::npos) {
      return SDLK_DOWN;
    } else if(rsKey.find("CTRL") != string::npos) {
      return SDLK_LCTRL;
    } else if(rsKey.find("SHIFT") != string::npos) {
      return SDLK_LSHIFT;
    }
  } else {
    return (rsKey[0] + 0x20);
  }
  return '?';
}

void DlgControls::OnKeyboard() {
	// Close joystick hinter, if it's on
	/*if(m_bHinterOpen)	{
		m_dlgHinter.ShowWindow(SW_HIDE);
		m_dlgHinter.DestroyWindow();
		m_bHinterOpen = false;
	}
	GetDlgItem(IDC_CONTROLLER)->EnableWindow(FALSE);
	GetDlgItem(IDC_CHECK1)->EnableWindow(FALSE);
	GetDlgItem(IDC_CHECK2)->EnableWindow(FALSE);
	GetDlgItem(IDC_CHECK3)->EnableWindow(FALSE);
	GetDlgItem(IDC_CHECK4)->EnableWindow(FALSE);
	GetDlgItem(IDC_CHECK5)->EnableWindow(FALSE);*/

	// Fill the combos with proper control ids
	/*CComboBox *pCombo1 = static_cast<CComboBox*>(GetDlgItem(IDC_COMBO1));
	CComboBox *pCombo2 = static_cast<CComboBox*>(GetDlgItem(IDC_COMBO2));
	CComboBox *pCombo3 = static_cast<CComboBox*>(GetDlgItem(IDC_COMBO3));
	CComboBox *pCombo4 = static_cast<CComboBox*>(GetDlgItem(IDC_COMBO4));
	CComboBox *pCombo5 = static_cast<CComboBox*>(GetDlgItem(IDC_COMBO5));
	CComboBox *pCombo6 = static_cast<CComboBox*>(GetDlgItem(IDC_COMBO6));
	CComboBox *pCombo7 = static_cast<CComboBox*>(GetDlgItem(IDC_COMBO7));
	CComboBox *pCombo8 = static_cast<CComboBox*>(GetDlgItem(IDC_COMBO8));
	CComboBox *pCombo9 = static_cast<CComboBox*>(GetDlgItem(IDC_COMBO9));

	FillAndSelectCombo(1, pCombo1, true, true);
	FillAndSelectCombo(2, pCombo2, true, true);
	FillAndSelectCombo(3, pCombo3, true, true);
	FillAndSelectCombo(4, pCombo4, true, true);
	FillAndSelectCombo(5, pCombo5, true, true);
	FillAndSelectCombo(6, pCombo6, true, false);
	FillAndSelectCombo(7, pCombo7, true, false);
	FillAndSelectCombo(8, pCombo8, true, false);
	FillAndSelectCombo(9, pCombo9, true, false);*/
	
	for (int x = 0; x < 9; x++)
		FillAndSelectCombo(x + 1, lists[x], true, true);
}

void DlgControls::FillAndSelectCombo(int nCombo, DropDownList *ddl, bool bKeyboard, bool bContinuous) {
  ddl->reset();
  if(bKeyboard) {
	ddl->add("<LEFT>");
    ddl->add("<RIGHT>");
    ddl->add("<UP>");
    ddl->add("<DOWN>");
    ddl->add("<CTRL>");
    ddl->add("<SHIFT>");
    char c;
    for(c = 'A'; c <= 'Z'; ++c) {
		char cl[] = {c, '\0'};
      string sTmp = cl;
      //sTmp.Format("%c (key)", c);
      sTmp.append(" (key)");
      ddl->add(sTmp);
      //pCombo->InsertString(-1, sTmp);
    }
    for(c = '0'; c <= '9'; ++c) {
		char cl[] = {c, '\0'};
      string sTmp = cl;
      sTmp.append(" (key)");
      ddl->add(sTmp);
    }

    int unKey;
    switch(nCombo) {
      case 1:
        unKey = ControllerModule::m_keymap.m_unAccelerate;
        break;
      case 2:
        unKey = ControllerModule::m_keymap.m_unBreak;
        break;
      case 3:
        unKey = ControllerModule::m_keymap.m_unLeft;
        break;
      case 4:
        unKey = ControllerModule::m_keymap.m_unRight;
        break;
      case 5:
        unKey = ControllerModule::m_keymap.m_unReverse;
        break;
      case 6:
        unKey = ControllerModule::m_keymap.m_unJet;
        break;
      case 7:
        unKey = ControllerModule::m_keymap.m_unHeli;
        break;
      case 8:
        unKey = ControllerModule::m_keymap.m_unLift;
        break;
      case 9:
        unKey = ControllerModule::m_keymap.m_unCamera;
        break;
    }

    string sKey;
    sKey.clear();
    switch(unKey) {
      case SDLK_LEFT:
        sKey = "<LEFT>";
        break;
      case SDLK_RIGHT:
        sKey = "<RIGHT>";
        break;
      case SDLK_UP:
        sKey = "<UP>";
        break;
      case SDLK_DOWN:
        sKey = "<DOWN>";
        break;
      case SDLK_LCTRL:
        sKey = "<CTRL>";
        break;
      case SDLK_LSHIFT:
        sKey = "<SHIFT>";
        break;
    }

    if(sKey.empty()) {
      //sKey.Format("%c (key)", char(unKey));
		char cl[] = {char(unKey - 0x20), '\0'};
      sKey = cl;
      sKey.append(" (key)");
    }

    //pCombo->SelectString(-1, sKey);
    ddl->select(sKey);

  } else {
/*    if(bContinuous) {
      pCombo->InsertString(-1, "X");
      pCombo->InsertString(-1, "Y");
      pCombo->InsertString(-1, "Z");
      pCombo->InsertString(-1, "X Rot");
      pCombo->InsertString(-1, "Y Rot");
      pCombo->InsertString(-1, "Z Rot");
      pCombo->InsertString(-1, "Slider1");
      pCombo->InsertString(-1, "Slider2");
      pCombo->InsertString(-1, "X+");
      pCombo->InsertString(-1, "Y+");
      pCombo->InsertString(-1, "Z+");
      pCombo->InsertString(-1, "X Rot+");
      pCombo->InsertString(-1, "Y Rot+");
      pCombo->InsertString(-1, "Z Rot+");
      pCombo->InsertString(-1, "Slider1+");
      pCombo->InsertString(-1, "Slider2+");
      pCombo->InsertString(-1, "X-");
      pCombo->InsertString(-1, "Y-");
      pCombo->InsertString(-1, "Z-");
      pCombo->InsertString(-1, "X Rot-");
      pCombo->InsertString(-1, "Y Rot-");
      pCombo->InsertString(-1, "Z Rot-");
      pCombo->InsertString(-1, "Slider1-");
      pCombo->InsertString(-1, "Slider2-");
    }
    for(int i = 0; i < 32; ++i) {
      CString sTmp;
      sTmp.Format("%d (button)", i + 1);
      pCombo->InsertString(-1, sTmp);
    }

    // Select correct item in the list
    bool bTmp;
    CString sJoy;
    switch(nCombo) {
      case 1:
        sJoy = ControllerModule::m_controllermap.m_cmiAccelerate.ToString(bTmp);
        break;
      case 2:
        sJoy = ControllerModule::m_controllermap.m_cmiBreak.ToString(bTmp);
        break;
      case 3:
        sJoy = ControllerModule::m_controllermap.m_cmiLeft.ToString(bTmp);
        break;
      case 4:
        sJoy = ControllerModule::m_controllermap.m_cmiRight.ToString(bTmp);
        break;
      case 5:
        sJoy = ControllerModule::m_controllermap.m_cmiReverse.ToString(bTmp);
        break;
      case 6:
        sJoy = ControllerModule::m_controllermap.m_cmiJet.ToString(bTmp);
        break;
      case 7:
        sJoy = ControllerModule::m_controllermap.m_cmiHeli.ToString(bTmp);
        break;
      case 8:
        sJoy = ControllerModule::m_controllermap.m_cmiLift.ToString(bTmp);
        break;
      case 9:
        sJoy = ControllerModule::m_controllermap.m_cmiCamera.ToString(bTmp);
        break;
    }

    pCombo->SelectString(-1, sJoy);*/
  }
}

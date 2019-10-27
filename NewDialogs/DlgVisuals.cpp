#include <string>
#include <vector>
#include <iostream>
#include <sstream>

using namespace std;

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "../SDLHelpers.h"

#include "Item.h"
#include "Dialog.h"
#include "Button.h"
#include "DropDownList.h"
#include "Frame.h"
#include "ButtonGroup.h"

#include "../ControllerModule.h"
#include "../GetawayDoc.h"
#include "../GetawayView.h"
#include "../BSimulation.h"
#include "../OpenGLHelpers.h"

#include "DlgVisuals.h"

extern bool g_cbBlackAndWhite;
extern bool g_cbMipMap;

DlgVisuals::DlgVisuals(CGetawayView *pView) : Dialog() {
	this->pView = pView;
	m_pSimulation = pView->pDoc->GetSimulation();
	
	// Setup other variables  
	//m_bOrigMipMap = g_cbMipMap;
	m_nOrigColorMode = m_pSimulation->m_nColorMode;
	m_rbSkyDetail = m_pSimulation->m_nSkyDetail;
	m_rbWaterDetail = m_pSimulation->m_nWaterDetail;
	m_rbDistantDetail = m_pSimulation->m_nDistantDetail;
	m_rbColor = m_pSimulation->m_nColorMode;
	m_rbScreenFormat = m_pSimulation->m_nScreenFormat;
	m_rbTextureSmoothness = m_pSimulation->m_nTextureSmoothness;
	
	if (!loadBackground("NewDialogs/visuals.png"))
		cout << "Background open failed!" << endl;
		
	vector<string> list;
	int selected = -1;
	int nModes = SDL_GetNumDisplayModes(0); //FIXME always 0 display
	for (int x = 0; x < nModes; x++) {
		SDL_DisplayMode mode;
		if (SDL_GetDisplayMode(0, x, &mode) >= 0) {
			stringstream ss;
			ss << mode.w << "x" << mode.h;
			list.push_back(ss.str());
			if (mode.w == pView->window_width && mode.h == pView->window_height)
				selected = x;
		}
	}

	Item *item = new DropDownList(list, &pView->dialogs, getRect(207, 79, 233, 25));
	if (selected >= 0)
		((DropDownList *)item)->setSelected(selected);
	else
		cout << "No current resolution found!" << endl;
	addItem(item);
	
	item = new ButtonGroup(getRect(207, 110, 235, 27));
	
	for (int x = 0; x < 3; x++) {
		string values[] = {"Lame", "Medium", "High"};
		Button *button = new Button(values[x], getColor(0xF0, 0xF0, 0xF0), getRect(x * 79, 0, 77, 27));
		button->setSelected(x == m_rbSkyDetail);
		((ButtonGroup *)item)->addItem(button);
	}
	
	((ButtonGroup *)item)->setValueChangedFunc(skyChangeCallback, this);
	addItem(item);
	
	item = new ButtonGroup(getRect(207, 140, 235, 27));
	
	for (int x = 0; x < 3; x++) {
		string values[] = {"Lame", "Medium", "High"};
		Button *button = new Button(values[x], getColor(0xF0, 0xF0, 0xF0), getRect(x * 79, 0, 77, 27));
		button->setSelected(x == m_rbDistantDetail);
		((ButtonGroup *)item)->addItem(button);
	}
	
	((ButtonGroup *)item)->setValueChangedFunc(distantChangeCallback, this);
	addItem(item);
	
	item = new ButtonGroup(getRect(207, 169, 235, 27));
	
	for (int x = 0; x < 2; x++) {
		string values[] = {"Black&White", "Colors"};
		Button *button = new Button(values[x], getColor(0xF0, 0xF0, 0xF0), getRect(x * 119, 0, 116, 27));
		button->setSelected(x == m_rbColor);
		((ButtonGroup *)item)->addItem(button);
	}
	
	((ButtonGroup *)item)->setValueChangedFunc(colorChangeCallback, this);
	addItem(item);
	
	item = new ButtonGroup(getRect(207, 198, 235, 27));
	
	for (int x = 0; x < 2; x++) {
		string values[] = {"Fullscreen", "Widescreen"};
		Button *button = new Button(values[x], getColor(0xF0, 0xF0, 0xF0), getRect(x * 119, 0, 116, 27));
		button->setSelected(x == m_rbScreenFormat);
		((ButtonGroup *)item)->addItem(button);
	}
	
	((ButtonGroup *)item)->setValueChangedFunc(screenFormatChangeCallback, this);
	addItem(item);
	
	item = new ButtonGroup(getRect(207, 227, 235, 27));
	
	for (int x = 0; x < 2; x++) {
		string values[] = {"No", "Yes"};
		Button *button = new Button(values[x], getColor(0xF0, 0xF0, 0xF0), getRect(x * 119, 0, 116, 27));
		button->setSelected(x == m_rbWaterDetail);
		((ButtonGroup *)item)->addItem(button);
	}
	
	((ButtonGroup *)item)->setValueChangedFunc(waterDetailChangeCallback, this);
	addItem(item);
	
	item = new ButtonGroup(getRect(207, 257, 235, 27));
	
	for (int x = 0; x < 3; x++) {
		string values[] = {"Boxy", "Gritty", "Silky"};
		Button *button = new Button(values[x], getColor(0xF0, 0xF0, 0xF0), getRect(x * 79, 0, 77, 27));
		button->setSelected(x == m_rbTextureSmoothness);
		((ButtonGroup *)item)->addItem(button);
	}
	
	((ButtonGroup *)item)->setValueChangedFunc(textureSmoothnessChangeCallback, this);
	addItem(item);
	
	item = new Button("Yeah", getColor(0xB4, 0xC8, 0xB4), getRect(207, 304, 117, 27));
	((Button *) item)->setButtonFunc(&yeahCallback, (void *)this);
	addItem(item);
	
	item = new Button("No Way", getColor(0xC8, 0xB4, 0xB4), getRect(326, 304, 115, 27));
	((Button *) item)->setButtonFunc(&noWayCallback, (void *)this);
	addItem(item);
	
	setExit(false);
}

DlgVisuals::~DlgVisuals() {
	deleteSurfaceAndBackground();
	deleteItems();
}

void DlgVisuals::skyChangeCallback(void *p, ButtonGroup *bg) {
	string values[] = {"Lame", "Medium", "High"};
	int x;
	for (x = 0; bg->getValue() != values[x]; x++);
	((DlgVisuals *)p)->m_rbSkyDetail = x;
}

void DlgVisuals::distantChangeCallback(void *p, ButtonGroup *bg) {
	string values[] = {"Lame", "Medium", "High"};
	int x;
	for (x = 0; bg->getValue() != values[x]; x++);
	((DlgVisuals *)p)->m_rbDistantDetail = x;
}

void DlgVisuals::colorChangeCallback(void *p, ButtonGroup *bg) {
	string values[] = {"Black&White", "Colors"};
	int x;
	for (x = 0; bg->getValue() != values[x]; x++);
	((DlgVisuals *)p)->m_rbColor = x;
}

void DlgVisuals::screenFormatChangeCallback(void *p, ButtonGroup *bg) {
	string values[] = {"Fullscreen", "Widescreen"};
	int x;
	for (x = 0; bg->getValue() != values[x]; x++);
	((DlgVisuals *)p)->m_rbScreenFormat = x;
}

void DlgVisuals::waterDetailChangeCallback(void *p, ButtonGroup *bg) {
	string values[] = {"No", "Yes"};
	int x;
	for (x = 0; bg->getValue() != values[x]; x++);
	((DlgVisuals *)p)->m_rbWaterDetail = x;
}

void DlgVisuals::textureSmoothnessChangeCallback(void *p, ButtonGroup *bg) {
	string values[] = {"Boxy", "Gritty", "Silky"};
	int x;
	for (x = 0; bg->getValue() != values[x]; x++);
	((DlgVisuals *)p)->m_rbTextureSmoothness= x;
}

void DlgVisuals::yeahCallback(void *p, Button *b) {
	((DlgVisuals *)p)->yeah();
}

void DlgVisuals::yeah() {
  //UpdateData();

  // Update screen mode if it has been changed
  /*CComboBox *pCombo = static_cast<CComboBox*>(GetDlgItem(IDC_COMBO1));
  int nNewScreenMode = pCombo->GetCurSel();
  if(nNewScreenMode != m_nOrigScreenMode) {
    CString sMode;
    pCombo->GetLBText(nNewScreenMode, sMode);
    int nWidth, nHeight, nBits, nHz;
    sscanf(LPCTSTR(sMode), 
           "%d*%d (%d bit, %d",
           &nWidth, 
           &nHeight, 
           &nBits, 
           &nHz);
    m_pSimulation->m_nDispWidth  = nWidth;
    m_pSimulation->m_nDispHeight = nHeight;
    m_pSimulation->m_nDispBits   = nBits;
    m_pSimulation->m_nDispHz     = nHz;
  }*/
  
  int w;
  int h;
  parseResolution(((DropDownList *)items[0])->getString(), w, h);
  
  //SDL_SetVideoMode(w, h, 0, SDL_OPENGL | SDL_FULLSCREEN); // we are currently always using desktop resolution so no need to change
  
	m_pSimulation->m_nDispWidth  = w;
	m_pSimulation->m_nDispHeight = h;

  g_cbMipMap = (m_rbTextureSmoothness == 2);
  switch(m_rbTextureSmoothness) {
    case 0: // Boxy
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      break;
    case 1: // Gritty
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      break;
    case 2: // Silky
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      break;
  }

  if(m_bOrigMipMap != g_cbMipMap) {
    //AfxMessageBox("NOTE: For the texture smoothness to change correctly,\nyou need to exit the game and restart it.\n(I'm working on this problem)");
    cout << "NOTE: For the texture smoothness to change correctly,\nyou need to exit the game and restart it.\n(I'm working on this problem)" << endl;
  }

  if((m_nOrigColorMode != m_rbColor) || (m_bOrigMipMap != g_cbMipMap)) {
    g_cbBlackAndWhite = !m_rbColor;

    // Reload textures to make color mode change
    OpenGLHelpers::FreeTexNames(11);
    OpenGLHelpers::CreateTexNames(11);

    m_pSimulation->GetGround()->LoadTextures();
    m_pSimulation->GetCar()->LoadTextures();

    m_pSimulation->GetGround()->PreProcessVisualization();
    m_pSimulation->GetCar()->PreProcessVisualization();
    m_pSimulation->PreProcessVisualization();

    // Setup background color
    glClearColor(0.6f, 0.7f, 0.9f, 0.0f);
    if(g_cbBlackAndWhite) {
      glClearColor(0.7f, 0.7f, 0.7f, 0.0f);
    }
    glClearColor(0, 0, 0, 0);
  }

  // Update other visualization details
  switch(m_rbDistantDetail) {
    case 0:
      m_pSimulation->GetGround()->m_dCoarseDistance = 300;
      break;
    case 1:
      m_pSimulation->GetGround()->m_dCoarseDistance = 600;
      break;
    case 2:
      m_pSimulation->GetGround()->m_dCoarseDistance = 16000;
      break;
  }

  // Update simulations variables
  m_pSimulation->m_nSkyDetail = m_rbSkyDetail;
  m_pSimulation->m_nWaterDetail = m_rbWaterDetail;
  m_pSimulation->m_nDistantDetail = m_rbDistantDetail;
  m_pSimulation->m_nColorMode = m_rbColor;
  m_pSimulation->m_nScreenFormat = m_rbScreenFormat;
  m_pSimulation->m_nTextureSmoothness = m_rbTextureSmoothness;
  
  setExit(true);
}

void DlgVisuals::noWayCallback(void *p, Button *b) {
	((DlgVisuals *)p)->setExit(true);
}

void DlgVisuals::parseResolution(string s, int &w, int &h) {
	w = 0;
	h = 0;
	bool sec = false;
	for (unsigned int x = 0; x < s.length(); x++) {
		if (s[x] != 'x') {
			if (!sec) {
				w *= 10;
				w += s[x] - 0x30;
			} else {
				h *= 10;
				h += s[x] - 0x30;
			}
		} else {
			sec = true;
		}
	}
}

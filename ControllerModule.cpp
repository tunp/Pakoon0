//
// Full-access controller module for all users of the game
// 

//#include "stdafx.h"
#include <SDL/SDL.h>
#include "ControllerModule.h"

/*bool                  ControllerModule::m_bInitialized = false;
LPDIRECTINPUT         ControllerModule::m_lpDirectInput = 0;
int                   ControllerModule::m_nControllers = 0;
int                   ControllerModule::m_nCurrent = -1;
bool                  ControllerModule::m_bCurrentInitialized = false;
CString               ControllerModule::m_sControllers[];
GUID                  ControllerModule::m_guids[];
HWND                  ControllerModule::m_hwnd = 0;
LPDIRECTINPUTDEVICE2  ControllerModule::m_pDIJoystick = 0;*/
BKeyMap               ControllerModule::m_keymap;
//BControllerMap        ControllerModule::m_controllermap;

ControllerModule::ControllerModule() {
}


/*static BOOL CALLBACK DIEnumDevicesProc(const struct DIDEVICEINSTANCEA *lpddi, LPVOID pvRef) {
  if(ControllerModule::m_nControllers >= 20) {
    return FALSE;
  }
  CString sName;
  sName.Format("%s - %s", lpddi->tszInstanceName, lpddi->tszProductName);
  ControllerModule::m_sControllers[ControllerModule::m_nControllers] = sName;
  ControllerModule::m_guids[ControllerModule::m_nControllers] = lpddi->guidInstance;
  ++ControllerModule::m_nControllers;
  return TRUE;
}*/


/*void ControllerModule::Initialize(HWND hwndFrameWnd) {
  m_hwnd = hwndFrameWnd;

  // Fetch all controllers to the arrays
  m_nControllers = 0;*/
	
  /*
  HRESULT hr = DirectInputCreate(AfxGetInstanceHandle(), // HINSTANCE hinst,                  
                                 DIRECTINPUT_VERSION,    // DWORD dwVersion,                  
                                 &m_lpDirectInput,       // LPDIRECTINPUT * lplpDirectInput,  
                                 NULL);                  // LPUNKNOWN punkOuter               

  if(m_lpDirectInput) {
    m_bInitialized = true;
    hr = m_lpDirectInput->EnumDevices(DIDEVTYPE_JOYSTICK,    // DWORD dwDevType,              
                                      DIEnumDevicesProc,     // LPDIENUMCALLBACK lpCallback,  
                                      0,                     // LPVOID pvRef,                 
                                      DIEDFL_ATTACHEDONLY ); // DWORD dwFlags
  }
  */
  /*if(m_nControllers) {
    m_nCurrent = 0;
    m_bCurrentInitialized = false;
  }
}*/


/*bool ControllerModule::SwitchToController(int nIndex) {

  if((m_nCurrent >= 0) && m_bCurrentInitialized) {
    // Stop using previous controller
    HRESULT hr = m_pDIJoystick->Unacquire();
    m_nCurrent = -1;
    m_bCurrentInitialized = false;
  }

  if(nIndex >= m_nControllers) {
    ASSERT(false);
    return false;
  }

  // Start using new controller
  LPDIRECTINPUTDEVICE lpDirectInputDevice;
  GUID &rguid = m_guids[nIndex];
  HRESULT hr;
  if(m_lpDirectInput) {
    hr = m_lpDirectInput->CreateDevice(rguid, // REFGUID rguid,                               
                                       &lpDirectInputDevice, // LPDIRECTINPUTDEVICE *lplpDirectInputDevice,  
                                       NULL); // LPUNKNOWN pUnkOuter                           
  } else {
    return false;
  }
  if(hr == 0) {
    hr = lpDirectInputDevice->SetCooperativeLevel(m_hwnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
    hr = lpDirectInputDevice->SetDataFormat(&c_dfDIJoystick);
    // Get second generation interface and set ranges
    LPDIRECTINPUTDEVICE2 lpDirectInputDevice2;
    hr = lpDirectInputDevice->QueryInterface(IID_IDirectInputDevice2, (void **) &lpDirectInputDevice2);

    DIPROPRANGE dipRange1;
    dipRange1.diph.dwSize = sizeof(dipRange1);
    dipRange1.diph.dwHeaderSize = sizeof(dipRange1.diph);
    dipRange1.diph.dwObj = DIJOFS_X;
    dipRange1.diph.dwHow = DIPH_BYOFFSET;
    dipRange1.lMin = -100;
    dipRange1.lMax = 100;
    hr = lpDirectInputDevice2->SetProperty(DIPROP_RANGE, &dipRange1.diph);

    dipRange1.diph.dwObj = DIJOFS_Y;
    hr = lpDirectInputDevice2->SetProperty(DIPROP_RANGE, &dipRange1.diph);
    dipRange1.diph.dwObj = DIJOFS_Z;
    hr = lpDirectInputDevice2->SetProperty(DIPROP_RANGE, &dipRange1.diph);
    dipRange1.diph.dwObj = DIJOFS_RX;
    hr = lpDirectInputDevice2->SetProperty(DIPROP_RANGE, &dipRange1.diph);
    dipRange1.diph.dwObj = DIJOFS_RY;
    hr = lpDirectInputDevice2->SetProperty(DIPROP_RANGE, &dipRange1.diph);
    dipRange1.diph.dwObj = DIJOFS_RZ;
    hr = lpDirectInputDevice2->SetProperty(DIPROP_RANGE, &dipRange1.diph);
    dipRange1.diph.dwObj = DIJOFS_SLIDER(0);
    hr = lpDirectInputDevice2->SetProperty(DIPROP_RANGE, &dipRange1.diph);
    dipRange1.diph.dwObj = DIJOFS_SLIDER(1);
    hr = lpDirectInputDevice2->SetProperty(DIPROP_RANGE, &dipRange1.diph);

    hr = lpDirectInputDevice2->Acquire(); // Gain access
    m_pDIJoystick = lpDirectInputDevice2;
    m_nCurrent = nIndex;
    m_bCurrentInitialized = true;
    return true;
  }
  return false;
}*/


/*double ControllerModule::GetValueFromRaw(BControllerMapItem* pcmi, DIJOYSTATE *pRawState) {
  if(pcmi->m_bButton) {
    return pRawState->rgbButtons[pcmi->m_nButton] ? 1.0 : 0.0;
  } else {
    double dValue = 0.0;
    switch(pcmi->m_jiPart) {
      case BControllerMapItem::X:
        dValue = pRawState->lX / 100.0;
        break;
      case BControllerMapItem::Y:
        dValue = pRawState->lY / 100.0;
        break;
      case BControllerMapItem::Z:
        dValue = pRawState->lZ / 100.0;
        break;
      case BControllerMapItem::XROT:
        dValue = pRawState->lRx / 100.0;
        break;
      case BControllerMapItem::YROT:
        dValue = pRawState->lRy / 100.0;
        break;
      case BControllerMapItem::ZROT:
        dValue = pRawState->lRz / 100.0;
        break;
      case BControllerMapItem::SLIDER1:
        dValue = pRawState->rglSlider[0] / 100.0;
        break;
      case BControllerMapItem::SLIDER2:
        dValue = pRawState->rglSlider[1] / 100.0;
        break;
    }
    if(pcmi->m_bWhole) {
      dValue = (dValue + 1.0) / 2.0;
    } else {
      if(pcmi->m_bPlusSide) {
        if(dValue < 0.0) {
          dValue = 0.0;
        }
      } else {
        if(dValue > 0.0) {
          dValue = 0.0;
        }
        dValue *= -1.0;
      }
    }

    if(pcmi->m_bFlipped) {
      dValue = 1.0 - dValue;
    }

    return dValue;
  }
}*/

/*bool ControllerModule::GetButtonEventFromRaw(BControllerMapItem* pcmi, DIJOYSTATE *pRawState) {
  bool bRet = false;
  if(pcmi->m_bButton) {
    if((!m_controllermap.m_bButtonState[pcmi->m_nButton]) &&
      pRawState->rgbButtons[pcmi->m_nButton]) {      
      bRet = true;
    }
  }
  m_controllermap.m_bButtonState[pcmi->m_nButton] = (pRawState->rgbButtons[pcmi->m_nButton] != 0);
  return bRet;
}*/

/*bool ControllerModule::GetButtonValueFromRaw(BControllerMapItem* pcmi, DIJOYSTATE *pRawState) {
  if(pcmi->m_bButton) {
    return pRawState->rgbButtons[pcmi->m_nButton] != 0;
  } else {
    return false;
  }
}*/




/*bool ControllerModule::GetControllerState(BControllerState& rcs, DIJOYSTATE *pRawState) {
  if(m_bCurrentInitialized) {
    HRESULT hr;
    hr = m_pDIJoystick->Poll();
    hr = m_pDIJoystick->GetDeviceState(sizeof(DIJOYSTATE), pRawState);

    // Convert raw states to simulation friendly state
    double dLeft = GetValueFromRaw(&m_controllermap.m_cmiLeft, pRawState);
    double dRight = GetValueFromRaw(&m_controllermap.m_cmiRight, pRawState);
    rcs.m_dTurn = dRight - dLeft;
    rcs.m_dAcceleration = GetValueFromRaw(&m_controllermap.m_cmiAccelerate, pRawState);
    rcs.m_dReverse = GetValueFromRaw(&m_controllermap.m_cmiReverse, pRawState);
    rcs.m_dBrake = GetValueFromRaw(&m_controllermap.m_cmiBreak, pRawState);
    rcs.m_bJetEvent = GetButtonEventFromRaw(&m_controllermap.m_cmiJet, pRawState);
    rcs.m_bHeliEvent = GetButtonEventFromRaw(&m_controllermap.m_cmiHeli, pRawState);
    rcs.m_bCameraEvent = GetButtonEventFromRaw(&m_controllermap.m_cmiCamera, pRawState);
    rcs.m_bLift = GetButtonValueFromRaw(&m_controllermap.m_cmiLift, pRawState);
    return true;
  }
  return false;
}*/



BKeyMap::BKeyMap() {
  m_unLeft = SDLK_LEFT;
  m_unRight = SDLK_RIGHT;
  m_unAccelerate = SDLK_UP;
  m_unReverse = SDLK_DOWN;
  m_unBreak = SDLK_LCTRL;
  m_unJet = SDLK_j;
  m_unHeli = SDLK_h;
  m_unLift = SDLK_l;
  m_unCamera = SDLK_c;
}


/*BControllerMap::BControllerMap() {
  m_cmiLeft.m_bButton = false;
  m_cmiLeft.m_bFlipped = false;
  m_cmiLeft.m_bPlusSide = false;
  m_cmiLeft.m_bWhole = false;
  m_cmiLeft.m_jiPart = BControllerMapItem::X;

  m_cmiRight.m_bButton = false;
  m_cmiRight.m_bFlipped = false;
  m_cmiRight.m_bPlusSide = true;
  m_cmiRight.m_bWhole = false;
  m_cmiRight.m_jiPart = BControllerMapItem::X;

  m_cmiAccelerate.m_bButton = false;
  m_cmiAccelerate.m_bFlipped = false;
  m_cmiAccelerate.m_bPlusSide = false;
  m_cmiAccelerate.m_bWhole = false;
  m_cmiAccelerate.m_jiPart = BControllerMapItem::Y;

  m_cmiReverse.m_bButton = false;
  m_cmiReverse.m_bFlipped = false;
  m_cmiReverse.m_bPlusSide = true;
  m_cmiReverse.m_bWhole = false;
  m_cmiReverse.m_jiPart = BControllerMapItem::Y;

  m_cmiBreak.m_bButton = true;
  m_cmiBreak.m_nButton = 0;

  m_cmiJet.m_bButton = true;
  m_cmiJet.m_nButton = 2;

  m_cmiHeli.m_bButton = true;
  m_cmiHeli.m_nButton = 3;

  m_cmiLift.m_bButton = true;
  m_cmiLift.m_nButton = 9;

  m_cmiCamera.m_bButton = true;
  m_cmiCamera.m_nButton = 1;
}*/



/*BControllerMapItem::BControllerMapItem() {
  m_bButton = false;
  m_nButton = 0;
  m_bWhole = true;
  m_bPlusSide = false;
  m_bFlipped = false;
  m_jiPart = X;
}*/

/*CString BControllerMapItem::ToString(bool& rbFlipped) {
  CString sRet;
  if(m_bButton) {
    sRet.Format("%d (button)", m_nButton + 1);
  } else {
    switch(m_jiPart) {
      case X:
        sRet = "X";
        break;
      case Y:
        sRet = "Y";
        break;
      case Z:
        sRet = "Z";
        break;
      case XROT:
        sRet = "X Rot";
        break;
      case YROT:
        sRet = "Y Rot";
        break;
      case ZROT:
        sRet = "Z Rot";
        break;
      case SLIDER1:
        sRet = "Slider1";
        break;
      case SLIDER2:
        sRet = "Slider2";
        break;
    }

    if(!m_bWhole) {
      if(m_bPlusSide) {
        sRet += "+";
      } else {
        sRet += "-";
      }
    }
  }

  rbFlipped = m_bFlipped;

  return sRet;
}*/

/*bool BControllerMapItem::FromString(CString sItem) {
  if(sItem.IsEmpty()) {
    return false;
  }

  if(sItem.Find("(button)") != -1) {
    m_bButton = true;
    sscanf(LPCTSTR(sItem), "%d", &m_nButton);
    --m_nButton;
  } else {
    m_bButton = false;

    if(sItem.Find("X Rot") != -1) {
      m_jiPart = XROT;
    } else if(sItem.Find("Y Rot") != -1) {
      m_jiPart = YROT;
    } else if(sItem.Find("Z Rot") != -1) {
      m_jiPart = ZROT;
    } else if(sItem.Find("X") != -1) {
      m_jiPart = X;
    } else if(sItem.Find("Y") != -1) {
      m_jiPart = Y;
    } else if(sItem.Find("Z") != -1) {
      m_jiPart = Z;
    } else if(sItem.Find("Slider1") != -1) {
      m_jiPart = SLIDER1;
    } else if(sItem.Find("Slider2") != -1) {
      m_jiPart = SLIDER2;
    }
  
    m_bWhole = true;

    if(sItem.Find("+") != -1) {
      m_bWhole = false;
      m_bPlusSide = true;
    }

    if(sItem.Find("-") != -1) {
      m_bWhole = false;
      m_bPlusSide = false;
    }
  }

  if(sItem.Find("Flipped") != -1) {
    m_bFlipped = true;
  } else {
    m_bFlipped = false;
  }

  return true;
}*/


//
// All access controller module for all users of the game
// 

//#pragma once

#include "BaseClasses.h"
//#include "DInput.h"

/*class BControllerState {
public:
  double m_dTurn;         // -1 to +1, -1 = full left, +1 = full right
  double m_dAcceleration; // 0 to +1, +1 = full acceleration
  double m_dReverse;      // 0 to +1, +1 = full reverse
  double m_dBrake;        // 0 to +1, +1 = full break
  bool   m_bJetEvent;     // Jet requested
  bool   m_bHeliEvent;    // Heli requested
  bool   m_bLift;         // Lift on
  bool   m_bCameraEvent;  // Camera requested
};*/

class BKeyMap {
public:
  int m_unLeft;
  int m_unRight;
  int m_unAccelerate;
  int m_unReverse;
  int m_unBreak;
  int m_unJet;
  int m_unHeli;
  int m_unLift;
  int m_unCamera;

  BKeyMap();
};

/*class BControllerMapItem {
public:
  enum TJoyItem{X = 0, Y, Z, XROT, YROT, ZROT, SLIDER1, SLIDER2};

  bool     m_bButton;
  int      m_nButton;
  bool     m_bWhole;
  bool     m_bPlusSide;
  bool     m_bFlipped;
  TJoyItem m_jiPart;

  BControllerMapItem();
  CString ToString(bool& rbFlipped);
  bool    FromString(CString sItem);
};*/

/*class BControllerMap {
public:
  bool               m_bButtonState[32];
  BControllerMapItem m_cmiLeft;
  BControllerMapItem m_cmiRight;
  BControllerMapItem m_cmiAccelerate;
  BControllerMapItem m_cmiReverse;
  BControllerMapItem m_cmiBreak;
  BControllerMapItem m_cmiJet;
  BControllerMapItem m_cmiHeli;
  BControllerMapItem m_cmiLift;
  BControllerMapItem m_cmiCamera;
  BControllerMap();
};*/

class ControllerModule {
public:
/*  static bool           m_bInitialized;
  static LPDIRECTINPUT  m_lpDirectInput;
  static int            m_nControllers;
  static CString        m_sControllers[20];
  static GUID           m_guids[20];
  static int            m_nCurrent;
  static bool           m_bCurrentInitialized;
  static HWND           m_hwnd;
  static LPDIRECTINPUTDEVICE2 m_pDIJoystick;*/
  static BKeyMap        m_keymap;
//  static BControllerMap m_controllermap;

  ControllerModule();
  /*static void Initialize(HWND hwndFrameWnd);
  static bool SwitchToController(int nIndex);
  static bool GetControllerState(BControllerState& rcs, DIJOYSTATE *pRawState);

  static double GetValueFromRaw(BControllerMapItem* pcmi, DIJOYSTATE *pRawState);
  static bool   GetButtonEventFromRaw(BControllerMapItem* pcmi, DIJOYSTATE *pRawState);
  static bool   GetButtonValueFromRaw(BControllerMapItem* pcmi, DIJOYSTATE *pRawState);*/
};

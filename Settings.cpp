//
// All access settings module for all users of the game
// 

//#include "stdafx.h"
#include <iostream>

using namespace std;

#include "Settings.h"
#include "SoundModule.h"
#include "ControllerModule.h"
#include "BGround.h"

string Settings::m_sFilename = "settings.dat";
//string Settings::m_sBrowserPath = _T("c:\\program files\\internet explorer\\iexplore.exe"); // try most likely path

extern bool g_cbBlackAndWhite;
extern bool g_cbMipMap;


void Settings::ReadSettings(BSimulation *pSimulation) {
  //char  sComputerName[MAX_COMPUTERNAME_LENGTH + 1];
  //DWORD dwLen = MAX_COMPUTERNAME_LENGTH + 1;
  /*if(GetComputerName(sComputerName, &dwLen)) {
    m_sFilename = ".\\";
    m_sFilename += sComputerName;
    m_sFilename += "settings.dat";
  }*/
  FILE *fp;
  fp = fopen(m_sFilename.c_str(), "r");
  if(fp) {
    char sLine[1024];
    fgets(sLine, 1024, fp);
    sscanf(sLine, "ScreenRes = %d, %d, %d, %d", 
      &(pSimulation->m_nDispWidth),
      &(pSimulation->m_nDispHeight),
      &(pSimulation->m_nDispBits),
      &(pSimulation->m_nDispHz));
    fgets(sLine, 1024, fp);
    sscanf(sLine, "SkyDetail = %d", &(pSimulation->m_nSkyDetail));
    fgets(sLine, 1024, fp);
    sscanf(sLine, "DistantDetail = %d", &(pSimulation->m_nDistantDetail));
    fgets(sLine, 1024, fp);
    sscanf(sLine, "WaterDetail = %d", &(pSimulation->m_nWaterDetail));
    fgets(sLine, 1024, fp);
    sscanf(sLine, "ColorMode = %d", &(pSimulation->m_nColorMode));
    fgets(sLine, 1024, fp);
    sscanf(sLine, "ScreenFormat = %d", &(pSimulation->m_nScreenFormat));
    fgets(sLine, 1024, fp);
    sscanf(sLine, "TextureSmoothness = %d", &(pSimulation->m_nTextureSmoothness));
    g_cbMipMap = (pSimulation->m_nTextureSmoothness == 2);

    int nTmp;
    fgets(sLine, 1024, fp);
    sscanf(sLine, "MenuMusicVolume = %d", &nTmp);
    SoundModule::SetMenuMusicVolume(nTmp);
    fgets(sLine, 1024, fp);
    sscanf(sLine, "VehicleSoundsVolume = %d", &nTmp);
    SoundModule::SetVehicleSoundsVolume(nTmp);
    fgets(sLine, 1024, fp);
    sscanf(sLine, "MessageSoundVolume = %d", &nTmp);
    SoundModule::SetMessageSoundVolume(nTmp);
    fgets(sLine, 1024, fp);
    sscanf(sLine, "SoundSpace = %d", &nTmp);
    SoundModule::SetSoundSpace(nTmp);

    fgets(sLine, 1024, fp);
    sscanf(sLine, "ControllerID = %d", &pSimulation->m_nController);

    fgets(sLine, 1024, fp);
    sscanf(sLine, "KEYMAP-Left = %u", &ControllerModule::m_keymap.m_unLeft);
    fgets(sLine, 1024, fp);
    sscanf(sLine, "KEYMAP-Right = %u", &ControllerModule::m_keymap.m_unRight);
    fgets(sLine, 1024, fp);
    sscanf(sLine, "KEYMAP-Accelerate = %u", &ControllerModule::m_keymap.m_unAccelerate);
    fgets(sLine, 1024, fp);
    sscanf(sLine, "KEYMAP-Reverse = %u", &ControllerModule::m_keymap.m_unReverse);
    fgets(sLine, 1024, fp);
    sscanf(sLine, "KEYMAP-Break = %u", &ControllerModule::m_keymap.m_unBreak);
    fgets(sLine, 1024, fp);
    sscanf(sLine, "KEYMAP-Jet = %u", &ControllerModule::m_keymap.m_unJet);
    fgets(sLine, 1024, fp);
    sscanf(sLine, "KEYMAP-Heli = %u", &ControllerModule::m_keymap.m_unHeli);
    fgets(sLine, 1024, fp);
    sscanf(sLine, "KEYMAP-Lift = %u", &ControllerModule::m_keymap.m_unLift);
    fgets(sLine, 1024, fp);
    sscanf(sLine, "KEYMAP-Camera = %u", &ControllerModule::m_keymap.m_unCamera);

    /*fgets(sLine, 1024, fp);
    ControllerModule::m_controllermap.m_cmiLeft.FromString(sLine + 27);
    fgets(sLine, 1024, fp);
    ControllerModule::m_controllermap.m_cmiRight.FromString(sLine + 27);
    fgets(sLine, 1024, fp);
    ControllerModule::m_controllermap.m_cmiAccelerate.FromString(sLine + 27);
    fgets(sLine, 1024, fp);
    ControllerModule::m_controllermap.m_cmiReverse.FromString(sLine + 27);
    fgets(sLine, 1024, fp);
    ControllerModule::m_controllermap.m_cmiBreak.FromString(sLine + 27);
    fgets(sLine, 1024, fp);
    ControllerModule::m_controllermap.m_cmiJet.FromString(sLine + 27);
    fgets(sLine, 1024, fp);
    ControllerModule::m_controllermap.m_cmiHeli.FromString(sLine + 27);
    fgets(sLine, 1024, fp);
    ControllerModule::m_controllermap.m_cmiLift.FromString(sLine + 27);
    fgets(sLine, 1024, fp);
    ControllerModule::m_controllermap.m_cmiCamera.FromString(sLine + 27);
    fgets(sLine, 1024, fp);
    sLine[strlen(sLine) - 1] = 0;
    m_sBrowserPath = sLine + 14;*/

    g_cbBlackAndWhite = (pSimulation->m_nColorMode == 0);
    switch(pSimulation->m_nDistantDetail) {
      case 0:
        pSimulation->GetGround()->m_dCoarseDistance = 300;
        break;
      case 1:
        pSimulation->GetGround()->m_dCoarseDistance = 600;
        break;
      case 2:
        pSimulation->GetGround()->m_dCoarseDistance = 16000;
        break;
    }
  } else {
    //CString sTmp;
    //sTmp.Format("Cannot open %s. Using default settings.", m_sFilename);
    //AfxMessageBox(sTmp, MB_OK);
    cout << "Cannot open settings. Using default settings." << endl;
  }
}

void Settings::WriteSettings(BSimulation *pSimulation) {
  /*char  sComputerName[MAX_COMPUTERNAME_LENGTH + 1];
  DWORD dwLen = MAX_COMPUTERNAME_LENGTH + 1;
  if(GetComputerName(sComputerName, &dwLen)) {
    m_sFilename = ".\\";
    m_sFilename += sComputerName;
    m_sFilename += "settings.dat";
  }*/
  FILE *fp;
  fp = fopen(m_sFilename.c_str(), "w");
  if(fp) {
    fprintf(fp, "ScreenRes = %d, %d, %d, %d\n", 
            pSimulation->m_nDispWidth,
            pSimulation->m_nDispHeight,
            pSimulation->m_nDispBits,
            pSimulation->m_nDispHz);
    fprintf(fp, "SkyDetail = %d\n", pSimulation->m_nSkyDetail);
    fprintf(fp, "DistantDetail = %d\n", pSimulation->m_nDistantDetail);
    fprintf(fp, "WaterDetail = %d\n", pSimulation->m_nWaterDetail);
    fprintf(fp, "ColorMode = %d\n", pSimulation->m_nColorMode);
    fprintf(fp, "ScreenFormat = %d\n", pSimulation->m_nScreenFormat);
    fprintf(fp, "TextureSmoothness = %d\n", pSimulation->m_nTextureSmoothness);

    fprintf(fp, "MenuMusicVolume = %d\n", SoundModule::GetMenuMusicVolume());
    fprintf(fp, "VehicleSoundsVolume = %d\n", SoundModule::GetVehicleSoundsVolume());
    fprintf(fp, "MessageSoundVolume = %d\n", SoundModule::GetMessageSoundVolume());
    fprintf(fp, "SoundSpace = %d\n", SoundModule::GetSoundSpace());

    fprintf(fp, "ControllerID = %d\n", pSimulation->m_nController);

    fprintf(fp, "KEYMAP-Left = %u\n", ControllerModule::m_keymap.m_unLeft);
    fprintf(fp, "KEYMAP-Right = %u\n", ControllerModule::m_keymap.m_unRight);
    fprintf(fp, "KEYMAP-Accelerate = %u\n", ControllerModule::m_keymap.m_unAccelerate);
    fprintf(fp, "KEYMAP-Reverse = %u\n", ControllerModule::m_keymap.m_unReverse);
    fprintf(fp, "KEYMAP-Break = %u\n", ControllerModule::m_keymap.m_unBreak);
    fprintf(fp, "KEYMAP-Jet = %u\n", ControllerModule::m_keymap.m_unJet);
    fprintf(fp, "KEYMAP-Heli = %u\n", ControllerModule::m_keymap.m_unHeli);
    fprintf(fp, "KEYMAP-Lift = %u\n", ControllerModule::m_keymap.m_unLift);
    fprintf(fp, "KEYMAP-Camera = %u\n", ControllerModule::m_keymap.m_unCamera);

    /*bool bTmp;
    CString sTmp;
    sTmp = ControllerModule::m_controllermap.m_cmiLeft.ToString(bTmp);
    if(bTmp) { sTmp += " Flipped";}
    fprintf(fp, "CONTROLLERMAP-Left       = %s\n", sTmp);
    sTmp = ControllerModule::m_controllermap.m_cmiRight.ToString(bTmp);
    if(bTmp) { sTmp += " Flipped";}
    fprintf(fp, "CONTROLLERMAP-Right      = %s\n", sTmp);
    sTmp = ControllerModule::m_controllermap.m_cmiAccelerate.ToString(bTmp);
    if(bTmp) { sTmp += " Flipped";}
    fprintf(fp, "CONTROLLERMAP-Accelerate = %s\n", sTmp);
    sTmp = ControllerModule::m_controllermap.m_cmiReverse.ToString(bTmp);
    if(bTmp) { sTmp += " Flipped";}
    fprintf(fp, "CONTROLLERMAP-Reverse    = %s\n", sTmp);
    sTmp = ControllerModule::m_controllermap.m_cmiBreak.ToString(bTmp);
    if(bTmp) { sTmp += " Flipped";}
    fprintf(fp, "CONTROLLERMAP-Break      = %s\n", sTmp);
    sTmp = ControllerModule::m_controllermap.m_cmiJet.ToString(bTmp);
    if(bTmp) { sTmp += " Flipped";}
    fprintf(fp, "CONTROLLERMAP-Jet        = %s\n", sTmp);
    sTmp = ControllerModule::m_controllermap.m_cmiHeli.ToString(bTmp);
    if(bTmp) { sTmp += " Flipped";}
    fprintf(fp, "CONTROLLERMAP-Heli       = %s\n", sTmp);
    sTmp = ControllerModule::m_controllermap.m_cmiLift.ToString(bTmp);
    if(bTmp) { sTmp += " Flipped";}
    fprintf(fp, "CONTROLLERMAP-Lift       = %s\n", sTmp);
    sTmp = ControllerModule::m_controllermap.m_cmiCamera.ToString(bTmp);
    if(bTmp) { sTmp += " Flipped";}
    fprintf(fp, "CONTROLLERMAP-Camera     = %s\n", sTmp);
    fprintf(fp, "BrowserPath = %s\n", m_sBrowserPath);*/
  } else {
    /*CString sTmp;
    sTmp.Format("Cannot open %s. Settings were not saved.", m_sFilename);
    AfxMessageBox(sTmp, MB_OK);*/
    cout << "Cannot open settings. Settings were not saved." << endl;
  }
}

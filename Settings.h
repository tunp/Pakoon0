//
// All access settings module for all users of the game
// 

#include "BaseClasses.h"
#include "BSimulation.h"

class Settings {
  static string m_sFilename;
public:
  static string m_sBrowserPath;

  Settings() {};

  static void SetFilename(string sFilename) {m_sFilename = sFilename;}
  static void ReadSettings(BSimulation *pSimulation);
  static void WriteSettings(BSimulation *pSimulation);
};

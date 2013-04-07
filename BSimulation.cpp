//
// Simulation
//

#include <iostream>
using namespace std;

//#include "initguid.h"
#include "BSimulation.h"
#include "BGround.h"
#include "BObject.h"
#include "OpenGLHelpers.h"
#include "SoundModule.h"
#include "ControllerModule.h"
#include "Settings.h"
//#include "DInput.h"

#include <cstdlib>
#include <algorithm>
#include <cstdio>

const double BSimulation::g_cdBrakesFriction = 0.45;
const double BSimulation::g_cdABSBrakeFriction = 0.45;
const double BSimulation::g_cdMaxSpeed = 400.0;
const double BSimulation::g_cdAccelerationFactor = 0.002;
const double BSimulation::g_cdGravityZ = 0.0008; // was 0.00065;
// const double BSimulation::g_cdGravityZ = 0.041; // was 0.00004;
const double BSimulation::g_cdTurnFactor = 0.008;

bool g_cbBlackAndWhite = false;
bool g_cbMipMap        = true;
extern double g_dRate;

//*****************************************************************************
// BASE ALGORITHM AND BASE VISUALIZATION
//*****************************************************************************

BSimulation::BSimulation() {
  m_dRotorVolFactor = 1.0;
  m_bPaused = false;
  m_bLiftingUp = false; 
  m_dLiftZ = 0.0;
  m_dAccelerationFactor = g_cdAccelerationFactor;
  m_dTurnFactor = g_cdTurnFactor;
  m_bRecordTrail = false;
  m_bRaining = false;
  m_bDisplayInfo = false;
  m_fp = 0;
  m_nController = 0; // 0 = keyboard, 1 = joystick
  m_nPhysicsStepsBetweenRender = 10;
  m_dPhysicsFraction = 0.0;
  m_bHitDetected = false;
  m_dHitSpeed = 0;
  m_bSteeringAidOn = true;
  m_bCalibrateSimulationSpeed = true;

  m_nSkyDetail = 2;         // High
  m_nDistantDetail = 2;     // High
  m_nColorMode = 1;         // Color
  m_nWaterDetail = 1;       // Draw water surface
  m_nScreenFormat = 1;      // Widescreen
  m_nTextureSmoothness = 2; // Silky

  // Create jet texture
  for(int i = 0; i < 16; ++i) {
    if((i % 4) == 0) {
      m_pTxtrJet[i * 3]     = 255;
      m_pTxtrJet[i * 3 + 1] = 255;
      m_pTxtrJet[i * 3 + 2] = 255;
    }
    if((i % 4) == 1) {
      m_pTxtrJet[i * 3]     = 0;
      m_pTxtrJet[i * 3 + 1] = 0;
      m_pTxtrJet[i * 3 + 2] = 0;
    }
    if((i % 4) == 2) {
      m_pTxtrJet[i * 3]     = 0;
      m_pTxtrJet[i * 3 + 1] = 0;
      m_pTxtrJet[i * 3 + 2] = 0;
    }
    if((i % 4) == 3) {
      m_pTxtrJet[i * 3]     = 255;
      m_pTxtrJet[i * 3 + 1] = 255;
      m_pTxtrJet[i * 3 + 2] = 255;
    }
  }
  OpenGLHelpers::CreateMipMaps(m_pTxtrJet, 4, 4, 3);

  // Create rotor blade texture
  int x, y;
  for(x = 0; x < 8; ++x) {
    for(y = 0; y < 8; ++y) {
      m_pTxtrBlade[y * 8 * 3 + x * 3] = GLubyte((1.0f - float(fabs(float(x) - 3.5) / 3.5)) * 255.0);
      m_pTxtrBlade[y * 8 * 3 + x * 3+1] = m_pTxtrBlade[y * 8 * 3 + x * 3];
      m_pTxtrBlade[y * 8 * 3 + x * 3+2] = m_pTxtrBlade[y * 8 * 3 + x * 3];
    }
  }
  OpenGLHelpers::CreateMipMaps(m_pTxtrBlade, 8, 8, 3);

  // Create shadow texture
  for(x = 0; x < 8; ++x) {
    for(y = 0; y < 8; ++y) {
      if(y == 0 || x == 0 || y == 7 || x == 7) {
        m_pTxtrShadow[y * 8 * 3 + x * 3] = 0;
        m_pTxtrShadow[y * 8 * 3 + x * 3+1] = 0;
        m_pTxtrShadow[y * 8 * 3 + x * 3+2] = 0;
      } else {
        m_pTxtrShadow[y * 8 * 3 + x * 3] = 255;
        m_pTxtrShadow[y * 8 * 3 + x * 3+1] = 255;
        m_pTxtrShadow[y * 8 * 3 + x * 3+2] = 255;
      }
    }
  }
  OpenGLHelpers::CreateMipMaps(m_pTxtrShadow, 8, 8, 3);

  // setup game resolution to be initially same as current
  /*DEVMODE devmode;
  EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &devmode);
  m_nDispWidth  = devmode.dmPelsWidth;
  m_nDispHeight = devmode.dmPelsHeight;
  m_nDispBits   = devmode.dmBitsPerPel;
  m_nDispHz     = devmode.dmDisplayFrequency;*/
  m_nDispWidth = 0;
  m_nDispHeight = 0;
  m_nDispBits = 0;
}

BSimulation::~BSimulation() {
  Settings::WriteSettings(this);
}


void BSimulation::PreProcessVisualization() {
  OpenGLHelpers::DefineMipMapTextures(8, 8, 3, GL_RGB, m_pTxtrShadow, 7);
  glNewList(DISPLIST_SHADOWTXTR, GL_COMPILE);
  OpenGLHelpers::DefineMipMapTextures(8, 8, 3, GL_RGB, m_pTxtrShadow, 7);
  glEndList();

  OpenGLHelpers::DefineMipMapTextures(8, 8, 3, GL_RGB, m_pTxtrBlade, 8);
  glNewList(DISPLIST_ROTORTXTR, GL_COMPILE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_ZERO, GL_ONE_MINUS_SRC_COLOR);
  glEnable(GL_TEXTURE_2D);
  OpenGLHelpers::SetColorFull(0.3, 0.3, 0.3, 0);
  OpenGLHelpers::DefineMipMapTextures(8, 8, 3, GL_RGB, m_pTxtrBlade, 8);
  glEndList();

  OpenGLHelpers::DefineMipMapTextures(4, 4, 3, GL_RGB, m_pTxtrJet, 9);
  glNewList(DISPLIST_JETTXTR, GL_COMPILE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
  glEnable(GL_TEXTURE_2D);
  OpenGLHelpers::SetColorFull(0.65, 0.6, 1.0, 0);
  OpenGLHelpers::DefineMipMapTextures(4, 4, 3, GL_RGB, m_pTxtrJet, 9);
  glEndList();

}

extern double Random(double);

//void BSimulation::PrePaint(CDC *pDC) {
void BSimulation::PrePaint() {
  /*if(ControllerModule::m_bInitialized && 
     (m_nController == 1) && 
     (ControllerModule::m_nCurrent >= 0)) {
    DIJOYSTATE stateRaw;
    if(ControllerModule::GetControllerState(m_controllerstate, &stateRaw)) {
      m_car.m_dTurn = m_controllerstate.m_dTurn;

      m_car.m_bHeliLifting = false;
      m_bLiftingUp = false;
      if(m_controllerstate.m_bLift) {
        if(m_car.m_nHeliMode > 0) {
          m_car.m_bHeliLifting = true;
        } else if(!m_bLiftingUp) {
          m_bLiftingUp = true;
          m_dLiftZ     = m_car.m_pPoint[0].m_vLocation.m_dZ;
        }
      }

      m_car.m_bHeliRighting = false;
      m_car.m_bHeliLefting = false;
      m_car.m_bHeliForwarding = false;
      m_car.m_bHeliBacking = false;
      if((m_controllerstate.m_dTurn > 0.1)  && 
         (m_car.m_nHeliMode > 399)) {
        m_car.m_bHeliRighting = true;
      } else if((m_controllerstate.m_dTurn < -0.1)  && 
                (m_car.m_nHeliMode > 399)) {
        m_car.m_bHeliLefting = true;
      } 

      m_car.m_bAccelerating = false; 
      m_car.m_bReversing = false;
      if(m_controllerstate.m_dAcceleration > 0) {
        m_car.m_dAccelerationFactor = m_controllerstate.m_dAcceleration;
        m_car.m_bAccelerating = true; 
        if((m_controllerstate.m_dAcceleration > 0.1) && 
           (m_car.m_nHeliMode > 399)) {
          m_car.m_bHeliForwarding = true;
        }
      }
      if(m_controllerstate.m_dReverse > 0) {
        m_car.m_dReversingFactor = m_controllerstate.m_dReverse;
        m_car.m_bReversing = true;
        if((m_controllerstate.m_dReverse > 0.1) && 
           (m_car.m_nHeliMode > 399)) {
          m_car.m_bHeliBacking = true;
        }
      }
      m_car.m_bBreaking = m_controllerstate.m_dBrake > 0.5;
      if(m_controllerstate.m_bHeliEvent) {
        ToggleHeli();
      }
      if(m_controllerstate.m_bJetEvent) {
        ToggleJet();
      }
      if(m_controllerstate.m_bCameraEvent) {
        SwitchCameraMode();
      }      
    }
  }*/

  m_nPhysicsSteps = m_nPhysicsStepsBetweenRender;
  if(Random(1.0) < m_dPhysicsFraction) {
    ++m_nPhysicsSteps;
  }

  m_bHitDetected = false;
  m_dHitSpeed = 0;

  for(int i = 0; i < m_nPhysicsSteps; ++i) {
    UpdateCar();
    BVector vTmp(0, 0, 0);
    
    SoundModule::Update3DSounds(m_car.m_vLocation, 
                                m_car.m_pPoint[8].m_vector * 300.0,
                                m_camera.m_vLocation, 
                                m_camera.m_orientation, 
                                vTmp); // Wrong. Use camera's velocity.

		ApplyAirDragAndGravity();
		ApplySteering(); // NOTE: This must be called before ApplyGroundEffect() for joystick support to work!
		ApplyGroundEffect();
  	UpdateCarPointsUsingStrings();
	  MoveCarPoints();
    if(m_car.m_nHeliMode > 299) {
      UpdateHeliControls();
    }
    if(m_car.m_nJetMode > 99) {
      UpdateJetControls();
    }
    if(m_bLiftingUp) {
      if(m_car.m_pPoint[0].m_vLocation.m_dZ > (m_dLiftZ - 4.5)) {
        m_car.m_pPoint[4].m_vector.m_dZ = g_cdGravityZ * -50;
        m_car.m_pPoint[5].m_vector.m_dZ = g_cdGravityZ * -30;
      }
    }
  }
  UpdateTrails();
  SetUpCamera();
}

void BSimulation::UpdateHeliControls() {
  double dEffect = ((double(m_car.m_nHeliMode) - 299.0) / 101.0) * m_car.m_dHeliBladePower;
  if(m_car.m_bHeliLifting) {
    if(m_car.m_dHeliLift < BSimulation::g_cdGravityZ * 45.0) {
      m_car.m_dHeliLift += 0.00002;
    }        
  } else {
    if(m_car.m_dHeliLift > BSimulation::g_cdGravityZ * 35.0) {
      m_car.m_dHeliLift -= 0.00001;
    }        
  }
  if(m_car.m_bHeliForwarding) {
    if(m_car.m_dHeliForward < 0.01) {
      m_car.m_dHeliForward += 0.0001;
    }        
  } else {
    if(m_car.m_dHeliForward > 0.0) {
      m_car.m_dHeliForward -= 0.0001;
    }        
  }
  if(m_car.m_bHeliBacking) {
    if(m_car.m_dHeliForward > -0.01) {
      m_car.m_dHeliForward -= 0.0001;
    }        
  } else {
    if(m_car.m_dHeliForward < 0.0) {
      m_car.m_dHeliForward += 0.0001;
    }        
  }
  if(m_car.m_bHeliRighting) {
    if(m_car.m_dHeliRight < 0.02) {
      m_car.m_dHeliRight += 0.0002;
    }        
  } else {
    if(m_car.m_dHeliRight > 0.0) {
      m_car.m_dHeliRight = 0;
    }        
  }
  if(m_car.m_bHeliLefting) {
    if(m_car.m_dHeliRight > -0.02) {
      m_car.m_dHeliRight -= 0.0002;
    }        
  } else {
    if(m_car.m_dHeliRight < 0.0) {
      m_car.m_dHeliRight = 0;
    }        
  }
  // Apply lift
  BVector vHeliVector = m_car.m_orientation.m_vUp * (m_car.m_dHeliLift * 0.6) +
                        BVector(0, 0, -m_car.m_dHeliLift * 0.4);
  m_car.m_pPoint[4].m_vector  += vHeliVector * 0.5 * dEffect;
  m_car.m_pPoint[13].m_vector += vHeliVector * 0.5 * dEffect;
  m_car.m_pPoint[3].m_vector  += vHeliVector * 0.01 * dEffect;
  m_car.m_pPoint[12].m_vector += vHeliVector * 0.01 * dEffect;

  // Apply forward
  vHeliVector = m_car.m_orientation.m_vForward * dEffect;
  vHeliVector.m_dZ = 0.0;
  if(m_car.m_dHeliForward > 0.0) {
    m_car.m_pPoint[1].m_vector  += vHeliVector * m_car.m_dHeliForward * 0.2;
    m_car.m_pPoint[10].m_vector += vHeliVector * m_car.m_dHeliForward * 0.2;
  } else {
    m_car.m_pPoint[6].m_vector  += vHeliVector * m_car.m_dHeliForward * 0.2;
    m_car.m_pPoint[15].m_vector += vHeliVector * m_car.m_dHeliForward * 0.2;
  }
  // Apply right turn
  m_car.m_pPoint[0].m_vector += m_car.m_orientation.m_vRight * m_car.m_dHeliRight * 0.05 * dEffect;

  // Apply instability shake
  static double dAngle = 0.0;
  BVector vShakeVector = m_car.m_orientation.m_vForward * cos(dAngle) * 0.001 +
                         m_car.m_orientation.m_vRight   * sin(dAngle) * 0.001;
  m_car.m_pPoint[4].m_vector  += vShakeVector * dEffect;
  m_car.m_pPoint[13].m_vector += vShakeVector * dEffect;
  dAngle += 0.1;
}


void BSimulation::UpdateJetControls() {
  // Apply Jet Boost
  BVector vJet = m_car.m_orientation.m_vForward * 
                 g_cdAccelerationFactor *                  
                 (m_car.m_bAccelerating ? m_car.m_dAccelerationFactor * 1.5 : 0.012);
  m_car.m_pPoint[0].m_vector  += vJet*0.5;
  m_car.m_pPoint[1].m_vector  += vJet;
  m_car.m_pPoint[9].m_vector  += vJet*0.5;
  m_car.m_pPoint[10].m_vector += vJet;
  m_car.m_pPoint[7].m_vector  += vJet*0.5;
  m_car.m_pPoint[6].m_vector  += vJet;
  m_car.m_pPoint[16].m_vector += vJet*0.5;
  m_car.m_pPoint[15].m_vector += vJet;
}

void BSimulation::ToggleHeli() {
  if(GetCar()->m_nHeliMode) {
    GetCar()->m_bHeliModeActivating = !GetCar()->m_bHeliModeActivating;
  } else {
    GetCar()->m_dHeliLift = BSimulation::g_cdGravityZ * 40.0;
    GetCar()->m_bHeliLifting = false;
    GetCar()->m_bHeliForwarding = false;
    GetCar()->m_bHeliRighting = false;
    GetCar()->m_nHeliMode = 1;
    GetCar()->m_bHeliModeActivating = true;
  }
}

void BSimulation::ToggleJet() {
  if(GetCar()->m_nJetMode) {
    GetCar()->m_bJetModeActivating = !GetCar()->m_bJetModeActivating;
  } else {
    GetCar()->m_nJetMode = 1;
    GetCar()->m_bJetModeActivating = true;
  }
}

void BSimulation::SwitchCameraMode() {
  switch(GetCamera()->m_locMode) {
    case BCamera::FIXED:
      GetCamera()->m_locMode = BCamera::FOLLOW;
      GetCamera()->m_dAngleOfView = 75.0;
      break;
    case BCamera::FOLLOW:
      GetCamera()->m_locMode = BCamera::OVERVIEW;
      GetCamera()->m_bInitLoc = true;
      GetCamera()->m_dAngleOfView = 58.0;
      break;
    case BCamera::OVERVIEW:
      GetCamera()->m_locMode = BCamera::INCAR;
      GetCamera()->m_dAngleOfView = 75.0;
      break;
    case BCamera::INCAR:
      GetCamera()->m_locMode = BCamera::ONSIDE;
      GetCamera()->m_dAngleOfView = 75.0;
      break;
    case BCamera::ONSIDE:
      GetCamera()->m_locMode = BCamera::FIXED;
      GetCamera()->m_vFixLocation = GetCamera()->m_vLocation;
      GetCamera()->m_dAngleOfView = 70.0;
      break;
  }
}


double Random(double dRange) {
  return double(rand()) / double(RAND_MAX) * dRange;
}

//void BSimulation::Paint(CDC *pDC) {
void BSimulation::Paint() {
  GLenum error = glGetError();
  m_ground.Paint(m_nSkyDetail, m_nWaterDetail, m_camera, BGround::GT_GROUND);
  DrawShadowAndTrails();
  m_car.Paint(m_nPhysicsSteps);
  if(m_car.m_nJetMode) {
    DrawJet();
  }
  if(m_car.m_nHeliMode || !m_car.m_bHeliHatchesOK) {
    DrawRotor();
  }
  if(m_nWaterDetail == 1) {
    m_ground.Paint(m_nSkyDetail, m_nWaterDetail, m_camera, BGround::GT_WATER);
  }

  // If hit detected, play crash sound
  if(m_bHitDetected) {
    SoundModule::PlayCrashSound(m_dHitSpeed * 6.0);
  }
}


void BSimulation::CheckHeliPartForDamage(THeliPart part, BVector vPoint, int nInfo) {
  static BVector vNormal;
  static double dTmp, dDepth;
  if((PointUnderGround(vPoint, vNormal, dTmp, dDepth) > 0.0 && dDepth > 0.0) ||
     (PointInsideObject(vPoint, vNormal, dTmp, dDepth) > 0.0 && dDepth > 0.0)) {
    if(m_car.m_bHeliModeActivating && (m_car.m_nHeliMode < 300)) {
      m_car.m_bHeliModeActivating = false;
    } else {
      DamageHeliPart(part, nInfo);
    }
  }
}

void BSimulation::DamageHeliPart(THeliPart part, int nInfo) {
  switch(part) {
    case LEFTHATCH:
      m_car.m_bHeliHatchesOK = false;
      m_car.m_dHeliLeftHatchOffsetAngle = 0.1 + Random(0.1);
      break;
    case RIGHTHATCH:
      m_car.m_bHeliHatchesOK = false;
      m_car.m_dHeliRightHatchOffsetAngle = 0.1 + Random(0.1);
      break;
    case COVER:
      m_car.m_bHeliCoverOK = false;
      break;
    case BLADE:
      m_car.m_bHeliBladeOK[nInfo] = false;
      m_car.m_dHeliBladePower = ((m_car.m_bHeliBladeOK[0] ? 1.0 : 0.0) + 
                                 (m_car.m_bHeliBladeOK[1] ? 1.0 : 0.0) + 
                                 (m_car.m_bHeliBladeOK[2] ? 1.0 : 0.0)) / 3.0;
      if(m_car.m_dHeliBladePower < 0.01) {
        SoundModule::SetHeliSoundVolume(0);
      }
      break;
  }
}


const double g_cdPI = 3.141592654;
static const double dPI2 = 2.0 * g_cdPI;

void BSimulation::DrawRotor() {
  static bool bSoundStarted = false;
  static double dAngle = Random(dPI2);
  int i;

  // Draw hatches in correct phase
  glShadeModel(GL_FLAT);
  double dHatchPhase;
  if(m_car.m_nHeliMode > 100) {
    dHatchPhase = 1.0;
  } else {
    dHatchPhase = double(m_car.m_nHeliMode) / 100.0; 
  }

  // Draw left and right hatch
  glCallList(DISPLIST_CARTXTR);
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 128.0f);
  BVector v1 = m_car.m_pPoint[4].m_vLocation * 0.8 +
               m_car.m_pPoint[13].m_vLocation * 0.2;
  BVector v2 = m_car.m_pPoint[5].m_vLocation * 0.8 +
               m_car.m_pPoint[14].m_vLocation * 0.2;
  BVector v3 = m_car.m_pPoint[4].m_vLocation * 0.2 +
               m_car.m_pPoint[13].m_vLocation * 0.8;
  BVector v4 = m_car.m_pPoint[5].m_vLocation * 0.2 +
               m_car.m_pPoint[14].m_vLocation * 0.8;
  double dHatchLen = (v3 - v1).Length() / 2.0;
  BVector vTmp1 = (v3 - v1) * dHatchLen;
  BVector vTmp2 = (v1 - v3) * dHatchLen;
  BVector vTmp = (v2 - v1);
  vTmp.ToUnitLength();
  BVector vUp = m_car.m_orientation.m_vRight.CrossProduct(vTmp);
  vUp.ToUnitLength();

  OpenGLHelpers::SetColorFull(0.5, 0.5, 0.5, 0);
  // left
  vTmp = m_car.m_orientation.m_vRight * dHatchLen * cos(dHatchPhase * 2.5 + m_car.m_dHeliLeftHatchOffsetAngle) +
         vUp * dHatchLen * sin(dHatchPhase * 2.5 + m_car.m_dHeliLeftHatchOffsetAngle);
  BVector vLeftHatch = vTmp;
  BVector vDownLeft = (v2 - v1).CrossProduct(vLeftHatch);
  vDownLeft.ToUnitLength();
  
  glNormal3d(-vDownLeft.m_dX, -vDownLeft.m_dY, -vDownLeft.m_dZ);
  glBegin(GL_TRIANGLE_STRIP);
  glTexCoord2d(0.4, 0);
  glVertex3d(v1.m_dX, v1.m_dY, v1.m_dZ);
  glTexCoord2d(0.4, 1);
  glVertex3d(v2.m_dX, v2.m_dY, v2.m_dZ);
  glTexCoord2d(1, 0);
  glVertex3d(vTmp.m_dX + v1.m_dX, vTmp.m_dY + v1.m_dY, vTmp.m_dZ + v1.m_dZ);
  glTexCoord2d(1, 1);
  glVertex3d(vTmp.m_dX + v2.m_dX, vTmp.m_dY + v2.m_dY, vTmp.m_dZ + v2.m_dZ);
  glEnd();

  CheckHeliPartForDamage(LEFTHATCH, v1 + vTmp);
  CheckHeliPartForDamage(LEFTHATCH, v2 + vTmp);

  // right
  vTmp = m_car.m_orientation.m_vRight * -dHatchLen * cos(dHatchPhase * 2.5 + m_car.m_dHeliRightHatchOffsetAngle) +
         vUp * dHatchLen * sin(dHatchPhase * 2.5 + m_car.m_dHeliRightHatchOffsetAngle);
  BVector vRightHatch = vTmp;
  BVector vUpRight = (v4 - v3).CrossProduct(vRightHatch);
  vUpRight.ToUnitLength();

  glNormal3d(vUpRight.m_dX, vUpRight.m_dY, vUpRight.m_dZ);
  glBegin(GL_TRIANGLE_STRIP);
  glTexCoord2d(1.6, 0);
  glVertex3d(v3.m_dX, v3.m_dY, v3.m_dZ);
  glTexCoord2d(1.6, 1);
  glVertex3d(v4.m_dX, v4.m_dY, v4.m_dZ);
  glTexCoord2d(1, 0);
  glVertex3d(vTmp.m_dX + v3.m_dX, vTmp.m_dY + v3.m_dY, vTmp.m_dZ + v3.m_dZ);
  glTexCoord2d(1, 1);
  glVertex3d(vTmp.m_dX + v4.m_dX, vTmp.m_dY + v4.m_dY, vTmp.m_dZ + v4.m_dZ);
  glEnd();

  CheckHeliPartForDamage(RIGHTHATCH, v3 + vTmp);
  CheckHeliPartForDamage(RIGHTHATCH, v4 + vTmp);

  // Draw remaining roof parts
  glNormal3d(vUp.m_dX, vUp.m_dY, vUp.m_dZ);
  vTmp1 = m_car.m_pPoint[4].m_vLocation;
  vTmp2 = m_car.m_pPoint[5].m_vLocation;
  glBegin(GL_TRIANGLE_STRIP);  
  glTexCoord2d(0, 0);
  glVertex3d(vTmp1.m_dX, vTmp1.m_dY, vTmp1.m_dZ);
  glTexCoord2d(0, 1);
  glVertex3d(vTmp2.m_dX, vTmp2.m_dY, vTmp2.m_dZ);
  glTexCoord2d(0.4, 0);
  glVertex3d(v1.m_dX, v1.m_dY, v1.m_dZ);
  glTexCoord2d(0.4, 1);
  glVertex3d(v2.m_dX, v2.m_dY, v2.m_dZ);
  glEnd();

  vTmp1 = m_car.m_pPoint[13].m_vLocation;
  vTmp2 = m_car.m_pPoint[14].m_vLocation;
  glBegin(GL_TRIANGLE_STRIP);  
  glTexCoord2d(2, 0);
  glVertex3d(vTmp1.m_dX, vTmp1.m_dY, vTmp1.m_dZ);
  glTexCoord2d(2, 1);
  glVertex3d(vTmp2.m_dX, vTmp2.m_dY, vTmp2.m_dZ);
  glTexCoord2d(1.6, 0);
  glVertex3d(v3.m_dX, v3.m_dY, v3.m_dZ);
  glTexCoord2d(1.6, 1);
  glVertex3d(v4.m_dX, v4.m_dY, v4.m_dZ);
  glEnd();

  glDisable(GL_TEXTURE_2D);

  // Draw hatch insides
  OpenGLHelpers::SetColorFull(0.1, 0.1, 0.1, 0);
  // OpenGLHelpers::SetColorFull(0.3, 0.3, 0.3, 0);
  BVector vTiny = vDownLeft * 0.05;
  glBegin(GL_TRIANGLE_STRIP);
  glVertex3d(v1.m_dX, v1.m_dY, v1.m_dZ);
  glVertex3d(v2.m_dX, v2.m_dY, v2.m_dZ);
  glVertex3d(v1.m_dX + vTiny.m_dX, v1.m_dY + vTiny.m_dY, v1.m_dZ + vTiny.m_dZ);
  glVertex3d(v2.m_dX + vTiny.m_dX, v2.m_dY + vTiny.m_dY, v2.m_dZ + vTiny.m_dZ);
  glVertex3d(v1.m_dX + vLeftHatch.m_dX + vTiny.m_dX, v1.m_dY + vLeftHatch.m_dY + vTiny.m_dY, v1.m_dZ + vLeftHatch.m_dZ + vTiny.m_dZ);
  glVertex3d(v2.m_dX + vLeftHatch.m_dX + vTiny.m_dX, v2.m_dY + vLeftHatch.m_dY + vTiny.m_dY, v2.m_dZ + vLeftHatch.m_dZ + vTiny.m_dZ);
  glVertex3d(v1.m_dX + vLeftHatch.m_dX, v1.m_dY + vLeftHatch.m_dY, v1.m_dZ + vLeftHatch.m_dZ);
  glVertex3d(v2.m_dX + vLeftHatch.m_dX, v2.m_dY + vLeftHatch.m_dY, v2.m_dZ + vLeftHatch.m_dZ);
  glEnd();
  glBegin(GL_TRIANGLE_STRIP);
  glVertex3d(v1.m_dX, v1.m_dY, v1.m_dZ);
  glVertex3d(v1.m_dX + vLeftHatch.m_dX, v1.m_dY + vLeftHatch.m_dY, v1.m_dZ + vLeftHatch.m_dZ);
  glVertex3d(v1.m_dX + vTiny.m_dX, v1.m_dY + vTiny.m_dY, v1.m_dZ + vTiny.m_dZ);
  glVertex3d(v1.m_dX + vLeftHatch.m_dX + vTiny.m_dX, v1.m_dY + vLeftHatch.m_dY + vTiny.m_dY, v1.m_dZ + vLeftHatch.m_dZ + vTiny.m_dZ);
  glEnd();
  glBegin(GL_TRIANGLE_STRIP);
  glVertex3d(v2.m_dX, v2.m_dY, v2.m_dZ);
  glVertex3d(v2.m_dX + vLeftHatch.m_dX, v2.m_dY + vLeftHatch.m_dY, v2.m_dZ + vLeftHatch.m_dZ);
  glVertex3d(v2.m_dX + vTiny.m_dX, v2.m_dY + vTiny.m_dY, v2.m_dZ + vTiny.m_dZ);
  glVertex3d(v2.m_dX + vLeftHatch.m_dX + vTiny.m_dX, v2.m_dY + vLeftHatch.m_dY + vTiny.m_dY, v2.m_dZ + vLeftHatch.m_dZ + vTiny.m_dZ);
  glEnd();

  OpenGLHelpers::SetColorFull(0.1, 0.1, 0.1, 0);
  // OpenGLHelpers::SetColorFull(0.3, 0.3, 0.3, 0);
  vTiny = vUpRight * -0.05;
  glBegin(GL_TRIANGLE_STRIP);
  glVertex3d(v3.m_dX, v3.m_dY, v3.m_dZ);
  glVertex3d(v4.m_dX, v4.m_dY, v4.m_dZ);
  glVertex3d(v3.m_dX + vTiny.m_dX, v3.m_dY + vTiny.m_dY, v3.m_dZ + vTiny.m_dZ);
  glVertex3d(v4.m_dX + vTiny.m_dX, v4.m_dY + vTiny.m_dY, v4.m_dZ + vTiny.m_dZ);
  glVertex3d(v3.m_dX + vRightHatch.m_dX + vTiny.m_dX, v3.m_dY + vRightHatch.m_dY + vTiny.m_dY, v3.m_dZ + vRightHatch.m_dZ + vTiny.m_dZ);
  glVertex3d(v4.m_dX + vRightHatch.m_dX + vTiny.m_dX, v4.m_dY + vRightHatch.m_dY + vTiny.m_dY, v4.m_dZ + vRightHatch.m_dZ + vTiny.m_dZ);
  glVertex3d(v3.m_dX + vRightHatch.m_dX, v3.m_dY + vRightHatch.m_dY, v3.m_dZ + vRightHatch.m_dZ);
  glVertex3d(v4.m_dX + vRightHatch.m_dX, v4.m_dY + vRightHatch.m_dY, v4.m_dZ + vRightHatch.m_dZ);
  glEnd();
  glBegin(GL_TRIANGLE_STRIP);
  glVertex3d(v3.m_dX, v3.m_dY, v3.m_dZ);
  glVertex3d(v3.m_dX + vRightHatch.m_dX, v3.m_dY + vRightHatch.m_dY, v3.m_dZ + vRightHatch.m_dZ);
  glVertex3d(v3.m_dX + vTiny.m_dX, v3.m_dY + vTiny.m_dY, v3.m_dZ + vTiny.m_dZ);
  glVertex3d(v3.m_dX + vRightHatch.m_dX + vTiny.m_dX, v3.m_dY + vRightHatch.m_dY + vTiny.m_dY, v3.m_dZ + vRightHatch.m_dZ + vTiny.m_dZ);
  glEnd();
  glBegin(GL_TRIANGLE_STRIP);
  glVertex3d(v4.m_dX, v4.m_dY, v4.m_dZ);
  glVertex3d(v4.m_dX + vRightHatch.m_dX, v4.m_dY + vRightHatch.m_dY, v4.m_dZ + vRightHatch.m_dZ);
  glVertex3d(v4.m_dX + vTiny.m_dX, v4.m_dY + vTiny.m_dY, v4.m_dZ + vTiny.m_dZ);
  glVertex3d(v4.m_dX + vRightHatch.m_dX + vTiny.m_dX, v4.m_dY + vRightHatch.m_dY + vTiny.m_dY, v4.m_dZ + vRightHatch.m_dZ + vTiny.m_dZ);
  glEnd();


  // Draw base axis
  BVector vTranslate;
  BVector vFooFix = m_car.m_orientation.m_vUp * 0.3;
  if(m_car.m_nHeliMode < 100) {
    vTranslate = m_car.m_orientation.m_vUp * -1.5;
  } else if(m_car.m_nHeliMode >= 200) {
    vTranslate.Set(0, 0, 0);
    vFooFix.Set(0, 0, 0);
  } else {
    double dPhase = (double(m_car.m_nHeliMode) - 100.0) / 100.0;
    vTranslate = m_car.m_orientation.m_vUp * -1.5 * (1.0 - dPhase);
    vFooFix = m_car.m_orientation.m_vUp * 0.3 * (1.0 - dPhase);
  }  
  glShadeModel(GL_FLAT);
  OpenGLHelpers::SetColorFull(0.3, 0.3, 0.3, 0);
  BVector vCenter = vTranslate +
                    (m_car.m_pPoint[4].m_vLocation + 
                     m_car.m_pPoint[13].m_vLocation) * 0.5 +
                     m_car.m_orientation.m_vUp * -0.7 +
                     m_car.m_orientation.m_vForward * -0.4;
  BVector vCenterUp = vCenter + m_car.m_orientation.m_vUp * 1.1;
  if(!m_car.m_bHeliCoverOK) {
    vCenterUp -= m_car.m_orientation.m_vUp * 0.3;
    vCenterUp += m_car.m_orientation.m_vForward * cos(dAngle) * 0.1 + 
                 m_car.m_orientation.m_vRight   * sin(dAngle) * 0.1;
  }
  vCenter = vCenter + m_car.m_orientation.m_vUp * 0.5;
  BVector vAxisBase = vCenter + vFooFix;
  BVector vStrip[14];
  BVector vNormals[14];
  for(i = 0; i < 4; ++i) {
    vNormals[i] = m_car.m_orientation.m_vForward * cos(dAngle + double(i) * dPI2 / 3.0) * 0.1 + 
                  m_car.m_orientation.m_vRight   * sin(dAngle + double(i) * dPI2 / 3.0) * 0.1;
    vStrip[i * 2] = 
      vCenter + vFooFix + 
      m_car.m_orientation.m_vForward * cos(dAngle + double(i) * dPI2 / 3.0) * 0.1 + 
      m_car.m_orientation.m_vRight   * sin(dAngle + double(i) * dPI2 / 3.0) * 0.1;
    vStrip[i * 2 + 1] = 
      vCenterUp + 
      m_car.m_orientation.m_vForward * cos(dAngle + double(i) * dPI2 / 3.0) * 0.1 + 
      m_car.m_orientation.m_vRight   * sin(dAngle + double(i) * dPI2 / 3.0) * 0.1;
  }
  OpenGLHelpers::TriangleStripWithNormals(vStrip, vNormals, 8);

  // Draw hood
  glShadeModel(GL_SMOOTH);
  vCenter = vCenterUp + m_car.m_orientation.m_vUp * 0.1;
  for(i = 0; i < 7; ++i) {
    vNormals[i] = m_car.m_orientation.m_vForward * cos(dAngle + double(i) * dPI2 / 6.0) * 0.2 + 
                  m_car.m_orientation.m_vRight   * sin(dAngle + double(i) * dPI2 / 6.0) * 0.2;
    vStrip[i * 2] = 
      vCenterUp + 
      m_car.m_orientation.m_vForward * cos(dAngle + double(i) * dPI2 / 6.0) * 0.2 + 
      m_car.m_orientation.m_vRight   * sin(dAngle + double(i) * dPI2 / 6.0) * 0.2;
    vStrip[i * 2 + 1] = 
      vCenter + 
      m_car.m_orientation.m_vForward * cos(dAngle + double(i) * dPI2 / 6.0) * 0.2 + 
      m_car.m_orientation.m_vRight   * sin(dAngle + double(i) * dPI2 / 6.0) * 0.2;
  }
  OpenGLHelpers::TriangleStripWithNormals(vStrip, vNormals, 14);

  // Draw hood cover
  vStrip[0] = vCenter;
  for(i = 0; i < 7; ++i) {
    vStrip[i + 1] = 
      vCenter + 
      m_car.m_orientation.m_vForward * cos(dAngle + double(i) * dPI2 / 6.0) * 0.2 + 
      m_car.m_orientation.m_vRight   * sin(dAngle + double(i) * dPI2 / 6.0) * 0.2;
  }
  glNormal3d(0, 0, -1);
  OpenGLHelpers::TriangleFan(vStrip, 8);

  CheckHeliPartForDamage(COVER, vCenter);

  // Draw heli linen
  OpenGLHelpers::SetColorFull(0.1, 0.1, 0.1, 0);
  glBegin(GL_TRIANGLE_FAN);
  glVertex3d(vAxisBase.m_dX, vAxisBase.m_dY, vAxisBase.m_dZ);
  glVertex3d(v1.m_dX, v1.m_dY, v1.m_dZ);
  glVertex3d(v2.m_dX, v2.m_dY, v2.m_dZ);
  glVertex3d(v4.m_dX, v4.m_dY, v4.m_dZ);
  glVertex3d(v3.m_dX, v3.m_dY, v3.m_dZ);
  glVertex3d(v1.m_dX, v1.m_dY, v1.m_dZ);
  glEnd();
 
  OpenGLHelpers::SetColorFull(0.2, 0.2, 0.2, 0);
  if(m_car.m_nHeliMode < 200) {
    // Draw blades retracted
    DrawRetractedBlades(vCenterUp, 0.2, 0.05, dAngle);
  } else if(m_car.m_nHeliMode < 300) {
    if(bSoundStarted) {
      SoundModule::StopHeliSound();
      bSoundStarted = false;      
    }
    // Draw blades opening/closing
    double dPhase = (double(m_car.m_nHeliMode) - 200.0) / 100.0;
    if(dPhase < 0.05) {
      dPhase = 0.05;
    }
    DrawRetractedBlades(vCenterUp, 0.2, dPhase, dAngle);
  } else if(m_car.m_nHeliMode < 400) {
    if(!bSoundStarted) {
      SoundModule::StartHeliSound();
      bSoundStarted = true;
    }
    double dPhase = (double(m_car.m_nHeliMode) - 300.0) / 100.0;
    SoundModule::SetHeliSoundPhase(dPhase, m_car.m_dHeliBladePower);
    // Draw blades picking up/slowing down speed
    DrawRetractedBlades(vCenterUp, 0.2, 1.0, dAngle);
    dAngle += (0.5 + Random(0.01)) * dPhase;
  } else {
    // Draw three faint triangles to represent rotor blades
    glCallList(DISPLIST_ROTORTXTR);
    glNormal3d(m_car.m_orientation.m_vUp.m_dX, 
               m_car.m_orientation.m_vUp.m_dY, 
               m_car.m_orientation.m_vUp.m_dZ);

    for(i = 0; i < 3; ++i) {
      if(!m_car.m_bHeliBladeOK[i]) {
        dAngle += ((120.0 / 360.0) * dPI2);
        continue;
      }
      glBegin(GL_TRIANGLE_STRIP);
      for(double f = 0; f < 5.9; f += 1.0) {
        vTmp = vCenterUp + m_car.m_orientation.m_vUp * 0.03 * f +
               m_car.m_orientation.m_vForward * 
               cos(dAngle + f * 0.2) * 0.2 + 
               m_car.m_orientation.m_vRight * 
               sin(dAngle + f * 0.2) * 0.2;
        glTexCoord2d(f / 5.0, 1.0);
        glVertex3d(vTmp.m_dX, vTmp.m_dY, vTmp.m_dZ);

        vTmp = vCenterUp + m_car.m_orientation.m_vUp * 0.03 * f +
               m_car.m_orientation.m_vForward * 
               cos(dAngle + f * 0.2) * 4.0 + 
               m_car.m_orientation.m_vRight * 
               sin(dAngle + f * 0.2) * 4.0;
        glTexCoord2d(f / 5.0, 0.0);      
        glVertex3d(vTmp.m_dX, vTmp.m_dY, vTmp.m_dZ);
      }
      glEnd();

      CheckHeliPartForDamage(BLADE, vTmp, i);

      dAngle += ((120.0 / 360.0) * dPI2);
    }
    dAngle += 0.5 + Random(0.01);
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
  }

  // Proceed to next step
  double dFactor = 60.0 / g_dRate;
  if(m_car.m_bHeliModeActivating && m_car.m_nHeliMode < 300) {
    m_car.m_dHeliMode += (2.0 * dFactor);
    m_car.m_nHeliMode = int(m_car.m_dHeliMode);
  } else if(m_car.m_bHeliModeActivating && m_car.m_nHeliMode < 400) {
    m_car.m_dHeliMode += (1.0 * dFactor);
    m_car.m_nHeliMode = int(m_car.m_dHeliMode);
  } else if(!m_car.m_bHeliModeActivating && m_car.m_nHeliMode > 0) {
    if(m_car.m_nHeliMode < 300) {
      m_car.m_dHeliMode -= (2.0 * dFactor);
      m_car.m_nHeliMode = int(m_car.m_dHeliMode);
    } else {
      m_car.m_dHeliMode -= (1.0 * dFactor);
      m_car.m_nHeliMode = int(m_car.m_dHeliMode);
    }
    if(m_car.m_nHeliMode <= 1) {
      m_car.m_dHeliMode = 0.0;
      m_car.m_nHeliMode = 0;
    }
  }
}


void BSimulation::DrawRetractedBlades(BVector& rvCenter, 
                                      double dRad, 
                                      double dPhase, 
                                      double dAngle) {
  double dBladeLen = 4.0 - dRad;
  double dPartLen = dBladeLen / 4.0;
  BVector vUp  = m_car.m_orientation.m_vUp * cos(dPhase * g_cdPI / 2.0) * dPartLen;
  for(int i = 0; i < 3; ++i) {
    if(!m_car.m_bHeliBladeOK[i]) {
      dAngle += ((120.0 / 360.0) * dPI2);
      continue;
    }
    BVector vOut = (m_car.m_orientation.m_vForward * 
                    cos(dAngle) * dPartLen + 
                    m_car.m_orientation.m_vRight * 
                    sin(dAngle) * dPartLen) * 
                   sin(dPhase * g_cdPI / 2.0);
    BVector v1 = m_car.m_orientation.m_vForward * 
                 cos(dAngle - 1) * dRad + 
                 m_car.m_orientation.m_vRight * 
                 sin(dAngle - 1) * dRad;
    BVector v2 = m_car.m_orientation.m_vForward * 
                 cos(dAngle + 1) * dRad + 
                 m_car.m_orientation.m_vRight * 
                 sin(dAngle + 1) * dRad;
    glBegin(GL_TRIANGLE_STRIP);
    bool bGoUp = true;
    BVector vPos = rvCenter;
    for(int j = 0; j < 5; ++j) {
      BVector vNormal;
      if(bGoUp) { 
        vNormal = vOut + vUp;
      } else {
        vNormal = vOut - vUp;
      }
      vNormal.ToUnitLength();
      glNormal3d(vNormal.m_dX, vNormal.m_dY, vNormal.m_dZ);
      glVertex3d(vPos.m_dX + v1.m_dX, 
                 vPos.m_dY + v1.m_dY, 
                 vPos.m_dZ + v1.m_dZ);
      glNormal3d(vNormal.m_dX, vNormal.m_dY, vNormal.m_dZ);
      glVertex3d(vPos.m_dX + v2.m_dX, 
                 vPos.m_dY + v2.m_dY, 
                 vPos.m_dZ + v2.m_dZ);

      if((j == 1) || (j == 4)) {
        CheckHeliPartForDamage(BLADE, vPos, i);
      }

      if(bGoUp) { 
        vPos += vUp;
      } else {
        vPos -= vUp;
      }
      vPos += vOut;
      bGoUp = !bGoUp;
    }
    glEnd();
    dAngle += ((120.0 / 360.0) * dPI2);
  }
}








void BSimulation::DrawJet() {
  static bool bSoundStarted = false;
  int i;

  BVector vLinen[10];

  // Draw hatch covers in correct phase
  glShadeModel(GL_FLAT);
  double dHatchPhase;
  if(m_car.m_nJetMode > 50) {
    dHatchPhase = 1.0;
  } else {
    dHatchPhase = double(m_car.m_nJetMode) / 50.0; 
  }
  dHatchPhase;

  // Draw upper hatch
  glCallList(DISPLIST_CARTXTR);
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 128.0f);
  BVector v1 = m_car.m_pPoint[5].m_vLocation * 0.8 +
               m_car.m_pPoint[14].m_vLocation * 0.2;
  BVector v2 = m_car.m_pPoint[5].m_vLocation * 0.2 +
               m_car.m_pPoint[14].m_vLocation * 0.8;
  BVector v3 = m_car.m_pPoint[6].m_vLocation * 0.8 +
               m_car.m_pPoint[15].m_vLocation * 0.2;
  double dHatch1Len = (v3 - v1).Length();
  BVector vTmp = (v3 - v1) * 0.3;
  v1 += vTmp;
  v2 += vTmp;

  vLinen[1] = v3;
  vLinen[2] = v1;
  vLinen[4] = m_car.m_pPoint[6].m_vLocation * 0.2 +
              m_car.m_pPoint[15].m_vLocation * 0.8;
  vLinen[5] = v2;

  dHatch1Len *= 0.7;
  vTmp.ToUnitLength();
  glNormal3d(vTmp.m_dX, -vTmp.m_dZ, vTmp.m_dY);
  vTmp = m_car.m_orientation.m_vForward * -dHatch1Len * cos(dHatchPhase - g_cdPI / 4.0) +
         m_car.m_orientation.m_vUp * dHatch1Len * sin(dHatchPhase - g_cdPI / 4.0);
  OpenGLHelpers::SetColorFull(0.5, 0.5, 0.5, 0);

  glBegin(GL_TRIANGLE_STRIP);
  glTexCoord2d(0.4, 0.3);
  glVertex3d(v1.m_dX, v1.m_dY, v1.m_dZ);
  glTexCoord2d(2.0 - 0.4, 0.3);
  glVertex3d(v2.m_dX, v2.m_dY, v2.m_dZ);
  v3 = v1 + vTmp;
  glTexCoord2d(0.4, 1.0);
  glVertex3d(v3.m_dX, v3.m_dY, v3.m_dZ);
  v3 = v2 + vTmp;
  glTexCoord2d(2.0 - 0.4, 1.0);
  glVertex3d(v3.m_dX, v3.m_dY, v3.m_dZ);
  glEnd();

  BVector v11 = v1;
  BVector v12 = v2;
  BVector v13 = v1 + vTmp;
  BVector v14 = v2 + vTmp;

  // Draw lower hatch
  v1 = m_car.m_pPoint[7].m_vLocation * 0.8 +
       m_car.m_pPoint[16].m_vLocation * 0.2;
  v2 = m_car.m_pPoint[7].m_vLocation * 0.2 +
       m_car.m_pPoint[16].m_vLocation * 0.8;
  v3 = m_car.m_pPoint[6].m_vLocation * 0.8 +
       m_car.m_pPoint[15].m_vLocation * 0.2;
  double dHatch2Len = (v3 - v1).Length();
  vTmp = (v3 - v1) * 0.3;
  v1 += vTmp;
  v2 += vTmp;

  vLinen[0] = v1;
  vLinen[3] = v2;

  dHatch2Len *= 0.7;
  vTmp.ToUnitLength();
  glNormal3d(vTmp.m_dX, -vTmp.m_dZ, vTmp.m_dY);
  vTmp = m_car.m_orientation.m_vForward * -dHatch2Len * cos(-dHatchPhase*2.0 + asin(1.0/5.0) + g_cdPI / 2.0) +
         m_car.m_orientation.m_vUp * dHatch2Len * sin(-dHatchPhase*2.0 + asin(1.0/5.0) + g_cdPI / 2.0);
  OpenGLHelpers::SetColorFull(0.5, 0.5, 0.5, 0);
  glBegin(GL_TRIANGLE_STRIP);
  glTexCoord2d(0.4, 0.3 * 0.7);
  glVertex3d(v1.m_dX, v1.m_dY, v1.m_dZ);
  glTexCoord2d(2.0 - 0.4, 0.3 * 0.7);
  glVertex3d(v2.m_dX, v2.m_dY, v2.m_dZ);
  v3 = v1 + vTmp;
  glTexCoord2d(0.4, 0.0);
  glVertex3d(v3.m_dX, v3.m_dY, v3.m_dZ);
  v3 = v2 + vTmp;
  glTexCoord2d(2.0 - 0.4, 0.0);
  glVertex3d(v3.m_dX, v3.m_dY, v3.m_dZ);
  glEnd();

  BVector v21 = v1;
  BVector v22 = v2;
  BVector v23 = v1 + vTmp;
  BVector v24 = v2 + vTmp;

  glDisable(GL_TEXTURE_2D);

  // Draw hatch insides
  OpenGLHelpers::SetColorFull(0.1, 0.1, 0.1, 0);
  // OpenGLHelpers::SetColorFull(0.3, 0.3, 0.3, 0);
  BVector vTiny = (v13 - v11).CrossProduct(v12 - v11);
  vTiny.ToUnitLength();
  vTiny = vTiny * 0.05;
  glBegin(GL_TRIANGLE_STRIP);
  glVertex3d(v11.m_dX, v11.m_dY, v11.m_dZ);
  glVertex3d(v12.m_dX, v12.m_dY, v12.m_dZ);
  glVertex3d(v11.m_dX + vTiny.m_dX, v11.m_dY + vTiny.m_dY, v11.m_dZ + vTiny.m_dZ);
  glVertex3d(v12.m_dX + vTiny.m_dX, v12.m_dY + vTiny.m_dY, v12.m_dZ + vTiny.m_dZ);
  glVertex3d(v13.m_dX + vTiny.m_dX, v13.m_dY + vTiny.m_dY, v13.m_dZ + vTiny.m_dZ);
  glVertex3d(v14.m_dX + vTiny.m_dX, v14.m_dY + vTiny.m_dY, v14.m_dZ + vTiny.m_dZ);
  glVertex3d(v13.m_dX, v13.m_dY, v13.m_dZ);
  glVertex3d(v14.m_dX, v14.m_dY, v14.m_dZ);
  glEnd();
  glBegin(GL_TRIANGLE_STRIP);
  glVertex3d(v11.m_dX, v11.m_dY, v11.m_dZ);
  glVertex3d(v13.m_dX, v13.m_dY, v13.m_dZ);
  glVertex3d(v11.m_dX + vTiny.m_dX, v11.m_dY + vTiny.m_dY, v11.m_dZ + vTiny.m_dZ);
  glVertex3d(v13.m_dX + vTiny.m_dX, v13.m_dY + vTiny.m_dY, v13.m_dZ + vTiny.m_dZ);
  glEnd();
  glBegin(GL_TRIANGLE_STRIP);
  glVertex3d(v12.m_dX, v12.m_dY, v12.m_dZ);
  glVertex3d(v14.m_dX, v14.m_dY, v14.m_dZ);
  glVertex3d(v12.m_dX + vTiny.m_dX, v12.m_dY + vTiny.m_dY, v12.m_dZ + vTiny.m_dZ);
  glVertex3d(v14.m_dX + vTiny.m_dX, v14.m_dY + vTiny.m_dY, v14.m_dZ + vTiny.m_dZ);
  glEnd();

  OpenGLHelpers::SetColorFull(0.1, 0.1, 0.1, 0);
  // OpenGLHelpers::SetColorFull(0.3, 0.3, 0.3, 0);
  vTiny = (v22 - v21).CrossProduct(v23 - v21);
  vTiny.ToUnitLength();
  vTiny = vTiny * 0.05;
  glBegin(GL_TRIANGLE_STRIP);
  glVertex3d(v21.m_dX, v21.m_dY, v21.m_dZ);
  glVertex3d(v22.m_dX, v22.m_dY, v22.m_dZ);
  glVertex3d(v21.m_dX + vTiny.m_dX, v21.m_dY + vTiny.m_dY, v21.m_dZ + vTiny.m_dZ);
  glVertex3d(v22.m_dX + vTiny.m_dX, v22.m_dY + vTiny.m_dY, v22.m_dZ + vTiny.m_dZ);
  glVertex3d(v23.m_dX + vTiny.m_dX, v23.m_dY + vTiny.m_dY, v23.m_dZ + vTiny.m_dZ);
  glVertex3d(v24.m_dX + vTiny.m_dX, v24.m_dY + vTiny.m_dY, v24.m_dZ + vTiny.m_dZ);
  glVertex3d(v23.m_dX, v23.m_dY, v23.m_dZ);
  glVertex3d(v24.m_dX, v24.m_dY, v24.m_dZ);
  glEnd();
  glBegin(GL_TRIANGLE_STRIP);
  glVertex3d(v21.m_dX, v21.m_dY, v21.m_dZ);
  glVertex3d(v23.m_dX, v23.m_dY, v23.m_dZ);
  glVertex3d(v21.m_dX + vTiny.m_dX, v21.m_dY + vTiny.m_dY, v21.m_dZ + vTiny.m_dZ);
  glVertex3d(v23.m_dX + vTiny.m_dX, v23.m_dY + vTiny.m_dY, v23.m_dZ + vTiny.m_dZ);
  glEnd();
  glBegin(GL_TRIANGLE_STRIP);
  glVertex3d(v22.m_dX, v22.m_dY, v22.m_dZ);
  glVertex3d(v24.m_dX, v24.m_dY, v24.m_dZ);
  glVertex3d(v22.m_dX + vTiny.m_dX, v22.m_dY + vTiny.m_dY, v22.m_dZ + vTiny.m_dZ);
  glVertex3d(v24.m_dX + vTiny.m_dX, v24.m_dY + vTiny.m_dY, v24.m_dZ + vTiny.m_dZ);
  glEnd();

  // Draw base cowl
  glShadeModel(GL_SMOOTH);
  OpenGLHelpers::SetColorFull(0.25, 0.25, 0.25, 0);
  BVector vStart;
  BVector vTranslate = m_car.m_orientation.m_vUp * 0.1 + 
                       m_car.m_orientation.m_vForward * 
                       (1.0 - double(m_car.m_nJetMode - 50.0) / 50.0);
  if(m_car.m_nJetMode < 50) {
    vTranslate = m_car.m_orientation.m_vUp * 0.1 + 
                 m_car.m_orientation.m_vForward;
  }
  vStart = (m_car.m_pPoint[6].m_vLocation + 
            m_car.m_pPoint[15].m_vLocation) * 0.5 +
            m_car.m_orientation.m_vForward * 0.4 +
            vTranslate;

  vLinen[6] = vStart + m_car.m_orientation.m_vForward * 0.4 - m_car.m_orientation.m_vUp * 0.45 - m_car.m_orientation.m_vRight * 0.45;
  vLinen[7] = vStart + m_car.m_orientation.m_vForward * 0.4 + m_car.m_orientation.m_vUp * 0.45 - m_car.m_orientation.m_vRight * 0.45;
  vLinen[8] = vStart + m_car.m_orientation.m_vForward * 0.4 - m_car.m_orientation.m_vUp * 0.45 + m_car.m_orientation.m_vRight * 0.45;
  vLinen[9] = vStart + m_car.m_orientation.m_vForward * 0.4 + m_car.m_orientation.m_vUp * 0.45 + m_car.m_orientation.m_vRight * 0.45;

  BVector vEnd = vStart + m_car.m_orientation.m_vForward * -0.8;
  BVector vStrip[42];
  glBegin(GL_TRIANGLE_STRIP);
  for(i = 0; i < 21; ++i) {
    vStrip[i * 2] = 
      vStart + m_car.m_orientation.m_vForward * 0.4 +
      m_car.m_orientation.m_vUp      * cos(double(i) * dPI2 / 20.0) * 0.42 + 
      m_car.m_orientation.m_vRight   * sin(double(i) * dPI2 / 20.0) * 0.42;
    vStrip[i * 2 + 1] = 
      vEnd + 
      m_car.m_orientation.m_vUp      * cos(double(i) * dPI2 / 20.0) * 0.3 + 
      m_car.m_orientation.m_vRight   * sin(double(i) * dPI2 / 20.0) * 0.3;
    BVector vNormal = vStrip[i * 2] - vStart + m_car.m_orientation.m_vForward * 0.05;
    vNormal.ToUnitLength();
    glNormal3d(vNormal.m_dX, vNormal.m_dY, vNormal.m_dZ);
    glVertex3d(vStrip[i * 2].m_dX,     vStrip[i * 2].m_dY,     vStrip[i * 2].m_dZ);
    vNormal = vStrip[i * 2 + 1] - vEnd + m_car.m_orientation.m_vForward * -0.05;
    glNormal3d(vNormal.m_dX, vNormal.m_dY, vNormal.m_dZ);
    glVertex3d(vStrip[i * 2 + 1].m_dX, vStrip[i * 2 + 1].m_dY, vStrip[i * 2 + 1].m_dZ);    
  }
  glEnd();

  // Draw inner chrome cowl
  OpenGLHelpers::SetColorFull(0.5, 0.5, 0.5, 0);
  GLfloat fLight[4];
  fLight[0] = 1.0f;
  fLight[1] = 1.0f;
  fLight[2] = 1.0f;
  fLight[3] = 1.0f;
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, fLight);
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 100.0f);
  vStart = (m_car.m_pPoint[6].m_vLocation + 
            m_car.m_pPoint[15].m_vLocation) * 0.5 +
            m_car.m_orientation.m_vForward * 0.2 +
            vTranslate;
  vEnd = vStart + m_car.m_orientation.m_vForward * -0.8;
  glBegin(GL_TRIANGLE_STRIP);
  for(i = 0; i < 21; ++i) {
    vStrip[i * 2] = 
      vStart + 
      m_car.m_orientation.m_vUp      * cos(double(i) * dPI2 / 20.0) * 0.35 + 
      m_car.m_orientation.m_vRight   * sin(double(i) * dPI2 / 20.0) * 0.35;
    vStrip[i * 2 + 1] = 
      vEnd + 
      m_car.m_orientation.m_vUp      * cos(double(i) * dPI2 / 20.0) * 0.25 + 
      m_car.m_orientation.m_vRight   * sin(double(i) * dPI2 / 20.0) * 0.25;
    BVector vNormal = vStrip[i * 2] - vStart + m_car.m_orientation.m_vForward * 0.05;
    vNormal.ToUnitLength();
    glNormal3d(vNormal.m_dX, vNormal.m_dY, vNormal.m_dZ);
    glVertex3d(vStrip[i * 2].m_dX,     vStrip[i * 2].m_dY,     vStrip[i * 2].m_dZ);
    vNormal = vStrip[i * 2 + 1] - vEnd + m_car.m_orientation.m_vForward * -0.05;
    glNormal3d(vNormal.m_dX, vNormal.m_dY, vNormal.m_dZ);
    glVertex3d(vStrip[i * 2 + 1].m_dX, vStrip[i * 2 + 1].m_dY, vStrip[i * 2 + 1].m_dZ);
  }
  glEnd();
  // OpenGLHelpers::TriangleStrip(vStrip, 42);

  // Draw inner jet glow
  double dJetGlow;
  if(m_car.m_nJetMode <= 50) {
    dJetGlow = 0.0;
  } else {
    dJetGlow = double(m_car.m_nJetMode - 50) / 50.0; 
  }
  OpenGLHelpers::SetColorFull(0.65 * dJetGlow, 0.6 * dJetGlow, 1.0 * dJetGlow, 0);
  vStart = (m_car.m_pPoint[6].m_vLocation + 
            m_car.m_pPoint[15].m_vLocation) * 0.5 +
            m_car.m_orientation.m_vForward * -0.4 +
            vTranslate;
  vStrip[0] = vStart;
  for(i = 0; i < 21; ++i) {
    vStrip[i + 1] = 
      vStart + 
      m_car.m_orientation.m_vUp      * cos(double(i) * dPI2 / 20.0) * 0.25 + 
      m_car.m_orientation.m_vRight   * sin(double(i) * dPI2 / 20.0) * 0.25;
  }
  OpenGLHelpers::TriangleFan(vStrip, 22);

  // Draw linen
  OpenGLHelpers::SetColorFull(0.1, 0.1, 0.1, 0);
  vStrip[0] = vLinen[1];
  vStrip[1] = vLinen[6];
  vStrip[2] = vLinen[2];
  vStrip[3] = vLinen[7];
  vStrip[4] = vLinen[5];
  vStrip[5] = vLinen[9];
  vStrip[6] = vLinen[4];
  vStrip[7] = vLinen[8];
  OpenGLHelpers::TriangleStrip(vStrip, 8);
  vStrip[0] = vLinen[6];
  vStrip[1] = vLinen[7];
  vStrip[2] = vLinen[8];
  vStrip[3] = vLinen[9];
  OpenGLHelpers::TriangleStrip(vStrip, 4);
  vStrip[0] = vLinen[1];
  vStrip[1] = vLinen[6];
  vStrip[2] = vLinen[0];
  vStrip[3] = vLinen[8];
  vStrip[4] = vLinen[3];
  vStrip[5] = vLinen[4];
  OpenGLHelpers::TriangleStrip(vStrip, 6);

  // Draw jet flames
  if(m_car.m_nJetMode > 99) {
    if(!bSoundStarted) {
      SoundModule::StartJetSound();
      bSoundStarted = true;
    }
    double dJetLen = 0.5;
    vStart = (m_car.m_pPoint[6].m_vLocation + 
              m_car.m_pPoint[15].m_vLocation) * 0.5 +
              m_car.m_orientation.m_vForward * -(0.4 + Random(0.3)) +
              vTranslate;
    if(m_car.m_dAccelerationFactor > 0.99) {
      if(m_car.m_bAccelerating) {
        if(dJetLen < 3.0) {
          dJetLen = 3.0;
        }
        vEnd = vStart + m_car.m_orientation.m_vForward * -dJetLen;
      } else {
        dJetLen = 0.5;
        vEnd = vStart + m_car.m_orientation.m_vForward * -dJetLen;
      }
    } else {
      if(m_car.m_bAccelerating) {
        if(dJetLen < 3.0) {
          dJetLen = 0.5 + m_car.m_dAccelerationFactor * 3.0;
        }
        vEnd = vStart + m_car.m_orientation.m_vForward * -dJetLen;
      } else {
        dJetLen = 0.5;
        vEnd = vStart + m_car.m_orientation.m_vForward * -dJetLen;
      }
    }

    SoundModule::SetJetSoundPhase((dJetLen - 0.5) / 2.5);

    for(i = 0; i < 21; ++i) {
      vStrip[i * 2] = 
        vStart + 
        m_car.m_orientation.m_vUp      * cos(double(i) * dPI2 / 20.0) * 0.25 + 
        m_car.m_orientation.m_vRight   * sin(double(i) * dPI2 / 20.0) * 0.25;
      vStrip[i * 2 + 1] = 
        vEnd + 
        m_car.m_orientation.m_vUp      * cos(double(i) * dPI2 / 20.0) * 0.20 + 
        m_car.m_orientation.m_vRight   * sin(double(i) * dPI2 / 20.0) * 0.20;
    }

    glPushAttrib(GL_ENABLE_BIT | GL_TEXTURE_2D);
    glCallList(DISPLIST_JETTXTR);
    glBegin(GL_TRIANGLE_STRIP);
    for(i = 0; i < 42; ++i) {
      if((i % 2) == 0) {
        BVector vNormal = vStrip[i / 2] - vStart;
        vNormal.ToUnitLength();
        glNormal3d(vNormal.m_dX, vNormal.m_dY, vNormal.m_dZ);
      }
      if((i % 2) == 3) {
        glTexCoord2d(0.0, 0.0);
      }
      if((i % 2) == 2) {
        glTexCoord2d(0.5, 0.0);
      }
      if((i % 2) == 1) {
        glTexCoord2d(0.0, 0.5);
      }
      if((i % 2) == 0) {
        glTexCoord2d(0.5, 0.5);
      }
      glVertex3d(vStrip[i].m_dX, vStrip[i].m_dY, vStrip[i].m_dZ);
    }
    glEnd();
    // Fade out
    vStart = vEnd;
    if(m_car.m_bAccelerating) {
      vEnd = vStart + m_car.m_orientation.m_vForward * -dJetLen;
    } else {
      vEnd = vStart + m_car.m_orientation.m_vForward * -dJetLen;
    }
    for(i = 0; i < 21; ++i) {
      vStrip[i * 2] = 
        vStart + 
        m_car.m_orientation.m_vUp      * cos(double(i) * dPI2 / 20.0) * 0.20 + 
        m_car.m_orientation.m_vRight   * sin(double(i) * dPI2 / 20.0) * 0.20;
      vStrip[i * 2 + 1] = vEnd;
    }
    glBegin(GL_TRIANGLE_STRIP);
    for(i = 0; i < 42; ++i) {
      if((i % 2) == 0) {
        BVector vNormal = vStrip[i / 2] - vStart;
        vNormal.ToUnitLength();
        glNormal3d(vNormal.m_dX, vNormal.m_dY, vNormal.m_dZ);
      }
      if((i % 2) == 0) {
        glTexCoord2d(1.0, 1.0);
      }
      if((i % 2) == 1) {
        glTexCoord2d(0.5, 1.0);
      }
      if((i % 2) == 2) {
        glTexCoord2d(1.0, 0.5);
      }
      if((i % 2) == 3) {
        glTexCoord2d(0.5, 0.5);
      }
      glVertex3d(vStrip[i].m_dX, vStrip[i].m_dY, vStrip[i].m_dZ);
    }
    glEnd();
    glPopAttrib();
  } else {
    if(bSoundStarted) {
      SoundModule::StopJetSound();
      bSoundStarted = false;
    }
  }

  // Proceed to next step
  double dFactor = 60.0 / g_dRate;
  if(m_car.m_bJetModeActivating && m_car.m_nJetMode < 100) {
    m_car.m_dJetMode += 2.0 * dFactor;
    m_car.m_nJetMode = int(m_car.m_dJetMode);
  } else if(!m_car.m_bJetModeActivating && m_car.m_nJetMode > 0) {
    m_car.m_dJetMode -= 2.0 * dFactor;
    m_car.m_nJetMode = int(m_car.m_dJetMode);
    if(m_car.m_nJetMode <= 1) {
      m_car.m_dJetMode = 0.0;
      m_car.m_nJetMode = 0;
    }
  }
}



//*****************************************************************************
// PHYSICS SIMULATION
//*****************************************************************************

void BSimulation::UpdateCar() {
  // Determine car orientation
  m_car.m_orientation.m_vForward = (m_car.m_pPoint[0].m_vLocation - m_car.m_pPoint[7].m_vLocation) +
                                   (m_car.m_pPoint[9].m_vLocation - m_car.m_pPoint[16].m_vLocation) * 0.5;
  m_car.m_orientation.m_vForward.ToUnitLength();
  m_car.m_orientation.m_vUp      = (m_car.m_pPoint[4].m_vLocation - m_car.m_pPoint[8].m_vLocation) +
                                   (m_car.m_pPoint[13].m_vLocation - m_car.m_pPoint[17].m_vLocation) * 0.5;
  m_car.m_orientation.m_vUp.ToUnitLength();
  m_car.m_orientation.m_vRight = m_car.m_orientation.m_vUp.CrossProduct(m_car.m_orientation.m_vForward);
  m_car.m_orientation.m_vRight.ToUnitLength();
 

  // Determine car vector and speed
  m_car.m_vector = (m_car.m_pPoint[0].m_vector + 
                    m_car.m_pPoint[9].m_vector +
                    m_car.m_pPoint[16].m_vector +
                    m_car.m_pPoint[7].m_vector) * 0.25;
  m_car.m_dSpeed = m_car.m_vector.Length();

  // Place wheels
  if(m_bSteeringAidOn) {
    m_car.m_dSteeringAid = 1.0 - (m_car.m_dSpeedKmh / 400.0);
    if(m_car.m_dSteeringAid < 0.1) {
      m_car.m_dSteeringAid = 0.1;
    }
  } else {
    m_car.m_dSteeringAid = 1.0;
  }
  for(int w = 0; w < m_car.m_nWheels; ++w) {
    m_car.m_pWheel[w].m_vLocation = m_car.m_pPoint[m_car.m_pWheel[w].m_nSuspPoint].m_vLocation +
                                    m_car.m_orientation.m_vUp * 
                                    -m_car.m_pWheel[w].m_dDistFromSuspPoint +
                                    m_car.m_orientation.m_vUp * 
                                    m_car.m_pWheel[w].m_dSuspension;
    m_car.m_pWheel[w].m_orientation = m_car.m_orientation;
    if(!m_car.m_pWheel[w].m_bRear) {
      // Apply steering to front wheels
      m_car.m_pWheel[w].m_orientation.m_vForward += m_car.m_pWheel[w].m_orientation.m_vRight * m_car.m_dTurn * m_car.m_dSteeringAid;
      m_car.m_pWheel[w].m_orientation.m_vForward.ToUnitLength();
      m_car.m_pWheel[w].m_orientation.m_vRight = m_car.m_pWheel[w].m_orientation.m_vUp.CrossProduct(m_car.m_pWheel[w].m_orientation.m_vForward);
      m_car.m_pWheel[w].m_orientation.m_vRight.ToUnitLength();
    }
  }
}


void BSimulation::ApplyAirDragAndGravity() {
  // Add a counterforce to apply airdrag
  BVector vDrag = m_car.m_vector * -m_car.m_dSpeed * m_car.m_dSpeed * 0.05;
  m_car.m_pPoint[0].m_vector  += vDrag;
  m_car.m_pPoint[4].m_vector  += vDrag;
  m_car.m_pPoint[7].m_vector  += vDrag;
  m_car.m_pPoint[9].m_vector  += vDrag;
  m_car.m_pPoint[13].m_vector  += vDrag;
  m_car.m_pPoint[16].m_vector += vDrag;
  // Add constant z-vector to all points to apply gravity
  static BVector vGravity(0, 0, g_cdGravityZ);
  for(int i = 0; i < m_car.m_nPoints; ++i) {
    m_car.m_pPoint[i].m_vector += vGravity;
  }
} 

void BSimulation::ApplySteering() {

  // Check for joystick info
  if(true) { // Always support keyboard as well
    // Use keyboard turning
    // If turning, do so
    if(m_car.m_bTurningLeft) {
      // Turn left
      if(m_car.m_dTurn > -0.7)  {
        m_car.m_dTurn -= m_dTurnFactor;
      }
    } else if(m_car.m_bTurningRight) {
      // Turn right
      if(m_car.m_dTurn < 0.7)  {
        m_car.m_dTurn += m_dTurnFactor;
      }
    } else {
      // Center wheels
      if(m_car.m_dTurn > m_dTurnFactor) {
        m_car.m_dTurn -= m_dTurnFactor;
      } else if(m_car.m_dTurn < -m_dTurnFactor) {
        m_car.m_dTurn += m_dTurnFactor;
      } else {
        m_car.m_dTurn = 0.0;
      }
    }
  }
} 

void BSimulation::ApplyGroundEffectToAPoint(int i, 
                                            BVector& vNormal, 
                                            double depth, 
                                            double dGroundFriction, 
                                            double dBaseDepth) {
  // apply friction correction
  BVector vDir = m_car.m_pPoint[i].m_vector;
  vDir.ToUnitLength();
  double dLossFactor = fabs(vNormal.ScalarProduct(vDir));

  // Apply thermodynamic loss
  double dThermoLoss = 0.99 - (ThermoDynamicLoss() * 
                              dLossFactor * 
                              m_car.m_pPoint[i].m_dFriction *
                              dGroundFriction * 
                              2.5);
  m_car.m_pPoint[i].m_vector = m_car.m_pPoint[i].m_vector * dThermoLoss;

  if(vNormal.m_dZ < -0.95) {
    vNormal.Set(0, 0, -1);
  }

  // "cannot be under ground" correction
  if(dBaseDepth > 0.0) {
    BVector vAntigravity = vNormal * 
                           dBaseDepth * 
                           GroundHardnessAt(m_car.m_pPoint[i].m_vLocation) * 0.5;
    m_car.m_pPoint[i].m_vector = m_car.m_pPoint[i].m_vector + vAntigravity;
  }
}

void BSimulation::ApplyGroundEffect() {
  int i;
  for(i = 0; i < m_car.m_nPoints; ++i) {
    double depth, dBaseDepth, dGroundFriction;
    BVector vNormal;
    // Apply base ground effect
    if((depth = PointUnderGround(m_car.m_pPoint[i].m_vLocation, vNormal, dGroundFriction, dBaseDepth)) > 0.0) {
      ApplyGroundEffectToAPoint(i, vNormal, depth, dGroundFriction, dBaseDepth);
      m_bHitDetected = true;
      if(dBaseDepth * m_car.m_pPoint[i].m_vector.Length() > m_dHitSpeed) {
        m_dHitSpeed = dBaseDepth * m_car.m_pPoint[i].m_vector.Length();
      }
    }
    // Apply collision to objects effect
    for(int o = 0; o < GetGround()->m_nObjects; ++o) {
      if((depth = GetGround()->m_pObjects[o].PointIsInsideObject(m_car.m_pPoint[i].m_vLocation, vNormal, dGroundFriction, dBaseDepth)) > 0.0) {
        ApplyGroundEffectToAPoint(i, vNormal, depth, dGroundFriction, dBaseDepth);
        m_bHitDetected = true;
        if(dBaseDepth * m_car.m_pPoint[i].m_vector.Length() > m_dHitSpeed) {
          m_dHitSpeed = dBaseDepth * m_car.m_pPoint[i].m_vector.Length();
        }
      }
    }
	}

  bool bWheelsTouchGround = false;
	for(i = 0; i < m_car.m_nWheels; ++i) {
    if(WheelTouchesGround(m_car.m_pWheel[i])) {
      ApplyWheelForces(m_car.m_pWheel[i]);
      bWheelsTouchGround = true;
    }    
    if(ApplyWheelForcesFromObjects(m_car.m_pWheel[i])) {
      bWheelsTouchGround = true;
    }
    m_car.m_pWheel[i].m_dSuspension *= 0.4; // Try to return wheel to rest position
	}

  // Update car engine sound
  static int nEngineBaseVol = 55;
  double dIdealRPM1 = m_car.m_dSpeed * 200000.0;
  double dIdealRPM2 = m_car.m_dSpeed * m_car.m_pWheel[0].m_dTTT * 200000.0;
  double dIdealRPM = dIdealRPM1 * 0.3 + dIdealRPM2 * 0.7;
  if(bWheelsTouchGround) {
    // Try to reach ideal
    m_car.m_dRPM += (dIdealRPM - m_car.m_dRPM) / 10.0;
  } else {
    if(((m_car.m_bAccelerating && (m_car.m_dAccelerationFactor > 0.1)) || 
        (m_car.m_bReversing && (m_car.m_dReversingFactor > 0.1))) && 
       (m_car.m_dRPM < 60000.0)) {
      m_car.m_dRPM += 200.0;
    } else {
      m_car.m_dRPM *= 0.99;
    }
  }
  SoundModule::SetEngineSoundRPM(int(m_car.m_dRPM));
} 

bool BSimulation::WheelTouchesGround(BWheel& rWheel) {
  // NOT READY 888 Use more sample points to determine 
  // wheel touching.
  rWheel.m_vLocSample = rWheel.m_pPoints[rWheel.m_nSuspPoint].m_vLocation + 
                        m_car.m_orientation.m_vUp * 
                        -rWheel.m_dDistFromSuspPoint +
                        m_car.m_orientation.m_vUp * 
                        -rWheel.m_dRadius;
  if((rWheel.m_dDepth = PointUnderGround(rWheel.m_vLocSample, 
                                         rWheel.m_vGroundNormal, 
                                         rWheel.m_dGroundFriction, 
                                         rWheel.m_dBaseDepth)) > 0.0) {
    // Calculate TTT
    BVector vDir = rWheel.m_pPoints[rWheel.m_nBasePoint1].m_vector + 
                   rWheel.m_pPoints[rWheel.m_nBasePoint2].m_vector;
    vDir.ToUnitLength();

    double dTTT1 = fabs(vDir.ScalarProduct(rWheel.m_orientation.m_vForward));
    double dTTT2 = 1.0 - fabs(vDir.ScalarProduct(rWheel.m_orientation.m_vUp));
    rWheel.m_dTTT = dTTT1 * dTTT2;
    if(rWheel.m_dTTT > 0.85) {
      rWheel.m_dTTT = 1.0;
    }
    return true;
  } else {
    return false;
  }
}

bool BSimulation::ApplyWheelForcesFromObjects(BWheel& rWheel) {
  // NOT READY 888 Use more sample points to determine 
  // wheel touching.
  bool bApplied = false;
  for(int o = 0; o < GetGround()->m_nObjects; ++o) {
    if((rWheel.m_dDepth = GetGround()->m_pObjects[o].PointIsInsideObject(rWheel.m_vLocSample, 
                                                                         rWheel.m_vGroundNormal, 
                                                                         rWheel.m_dGroundFriction, 
                                                                         rWheel.m_dBaseDepth)) > 0.0) {
      // Calculate TTT
      BVector vDir = rWheel.m_pPoints[rWheel.m_nBasePoint1].m_vector + 
                     rWheel.m_pPoints[rWheel.m_nBasePoint2].m_vector;
      vDir.ToUnitLength();

      double dTTT1 = fabs(vDir.ScalarProduct(rWheel.m_orientation.m_vForward));
      double dTTT2 = 1.0 - fabs(vDir.ScalarProduct(rWheel.m_orientation.m_vUp));
      rWheel.m_dTTT = dTTT1 * dTTT2;
      if(rWheel.m_dTTT > 0.85) {
        rWheel.m_dTTT = 1.0;
      }
      ApplyWheelForces(rWheel);
      bApplied = true;
    }
  }
  return bApplied;
}


static double ViscosityDamped(double dSusp, double dForce) {
  // If force is small, allow it all
  // If force is strong, damp it to absord the shock
  double dForceFactor;
  if(fabs(dSusp) > 0.4) {
    dForceFactor = 0.0;
  } else {
    dForceFactor = fabs(dSusp) / 0.4;
    dForceFactor = 1.0 - dForceFactor;
  }
  
  return dSusp + (dForce * dForceFactor);
}

void BSimulation::ApplyWheelForces(BWheel& rWheel) {
  rWheel.m_bInGround = true;
  // BVector vGroundNormal;
  // double dGroundFriction, dBaseDepth;
  // (void) PointUnderGround(rWheel.m_vLocSample, vGroundNormal, dGroundFriction, dBaseDepth);
  BVector vSpeed = rWheel.m_pPoints[rWheel.m_nBasePoint1].m_vector + 
                   rWheel.m_pPoints[rWheel.m_nBasePoint2].m_vector;
  vSpeed.ToUnitLength();
  double dThermoLoss = fabs(vSpeed.ScalarProduct(rWheel.m_vGroundNormal));
  dThermoLoss *= dThermoLoss;
  if(rWheel.m_dDepth > 0.015) {
    rWheel.m_dDepth -= 0.015;
    // First apply ground force as with any point touching the ground
    // 1) calculate f = Scalar(wheel.up, ground.normal)
    double dSuspFactor = fabs(rWheel.m_orientation.m_vUp.ScalarProduct(rWheel.m_vGroundNormal)); 
    // dSuspFactor = dSuspFactor * dSuspFactor * dSuspFactor;

    // 2) Modify wheel.susp with f * wheel.depth
    if(rWheel.m_dBaseDepth > 0.0) {
      rWheel.m_dSuspension = ViscosityDamped(rWheel.m_dSuspension, dSuspFactor * rWheel.m_dBaseDepth);
    }

    // Apply thermonynamic loss
    m_car.m_pPoint[rWheel.m_nBasePoint1].m_vector = m_car.m_pPoint[rWheel.m_nBasePoint1].m_vector *
                                                    (1.0 - 0.1 * dThermoLoss);
    m_car.m_pPoint[rWheel.m_nBasePoint2].m_vector = m_car.m_pPoint[rWheel.m_nBasePoint2].m_vector *
                                                    (1.0 - 0.1 * dThermoLoss);

    // 3) add ((1.0 - f) * wheel.depth) * ground.normal to base points 1 and 2
    if(rWheel.m_vGroundNormal.m_dZ < -0.85) {
      rWheel.m_vGroundNormal.Set(0, 0, -1);
    }

    m_car.m_pPoint[rWheel.m_nBasePoint1].m_vector += rWheel.m_vGroundNormal *
                                                     ((1.0 - dSuspFactor) * rWheel.m_dDepth * 0.025);
    m_car.m_pPoint[rWheel.m_nBasePoint2].m_vector += rWheel.m_vGroundNormal *
                                                     ((1.0 - dSuspFactor) * rWheel.m_dDepth * 0.025);

    // 4) Modify suspPoint's vector with wheel.up * 
    //    (wheel.suspension_wants_to_be_at_rest)
    //    (maybe all 3 points, suspPoint, basepoint1 and basepoint2)
    BVector vSuspForce = rWheel.m_orientation.m_vUp * 
                        (rWheel.m_dSuspension * dSuspFactor * 0.02);
    m_car.m_pPoint[rWheel.m_nSuspPoint].m_vector  += vSuspForce * 0.5;
    m_car.m_pPoint[rWheel.m_nBasePoint1].m_vector += vSuspForce * 0.25;
    m_car.m_pPoint[rWheel.m_nBasePoint2].m_vector += vSuspForce * 0.25;
    rWheel.m_dDepth += 0.015;
  }

  // Then frictions (brake and wheel orientation based)
  double dFrictionFactor = 0.001;
  if(m_car.m_bABS) {
    dFrictionFactor = g_cdABSBrakeFriction * rWheel.m_dFriction * rWheel.m_dGroundFriction;
    dFrictionFactor = BreakProfile(dFrictionFactor);
  } else if(m_car.m_bBreaking) {
    dFrictionFactor = g_cdBrakesFriction * rWheel.m_dFriction * rWheel.m_dGroundFriction;
    dFrictionFactor = BreakProfile(dFrictionFactor);
  }

  // Break wheel if too strong force
  /*
  if(((1.0 - rWheel.m_dTTT) * rWheel.m_dFriction * dGroundFriction > 0.035) && 
    (rWheel.m_pPoints[rWheel.m_nBasePoint1].m_vector.Length() > 0.2)) {
    rWheel.m_bBroken = true;
    rWheel.m_dRadius *= 0.85;
    rWheel.m_dFriction -= 0.1;
  }
  */

  dFrictionFactor = std::max(dFrictionFactor, (1.0 - rWheel.m_dTTT) * rWheel.m_dFriction * rWheel.m_dGroundFriction);

  // Apply ground oriented friction
  double dLossFactor = 1.0 - (dFrictionFactor * 0.05);
  rWheel.m_pPoints[rWheel.m_nBasePoint1].m_vector = (rWheel.m_pPoints[rWheel.m_nBasePoint1].m_vector) * 
                                                     dLossFactor;
  rWheel.m_pPoints[rWheel.m_nBasePoint2].m_vector = (rWheel.m_pPoints[rWheel.m_nBasePoint2].m_vector) * 
                                                     dLossFactor;

  // Then acceleration
  if(m_car.m_bAccelerating || m_car.m_bReversing) { // 4WD
    rWheel.m_pPoints[rWheel.m_nBasePoint1].m_vector += rWheel.m_orientation.m_vForward * 
                                                       rWheel.m_dGroundFriction *
                                                       rWheel.m_dFriction *
                                                       m_dAccelerationFactor *
                                                       (m_car.m_bAccelerating ? m_car.m_dAccelerationFactor : -m_car.m_dReversingFactor);
    rWheel.m_pPoints[rWheel.m_nBasePoint2].m_vector += rWheel.m_orientation.m_vForward * 
                                                       rWheel.m_dGroundFriction *
                                                       rWheel.m_dFriction *
                                                       m_dAccelerationFactor *
                                                       (m_car.m_bAccelerating ? m_car.m_dAccelerationFactor : -m_car.m_dReversingFactor);
  }

  // Apply turning
  if(!m_car.m_bBreaking ) { // && !rWheel.m_bRear
    BVector vWheelVector = (rWheel.m_pPoints[rWheel.m_nBasePoint1].m_vector +
                            rWheel.m_pPoints[rWheel.m_nBasePoint2].m_vector) * 0.5;
    BVector vIdeal = rWheel.m_orientation.m_vForward;
    if((vIdeal.ScalarProduct(vWheelVector) < 0.0) && 
       !m_car.m_bAccelerating) {
      vIdeal = vIdeal * -1.0;
    }
    vIdeal.ToUnitLength();

    BVector vReality = vWheelVector;
    double  dRealLen = vReality.Length();
    vReality.ToUnitLength();

    double dEffect = rWheel.m_dTTT * rWheel.m_dFriction * rWheel.m_dGroundFriction * 0.05;

    rWheel.m_pPoints[rWheel.m_nBasePoint1].m_vector = vIdeal * dRealLen * dEffect +
                                                      rWheel.m_pPoints[rWheel.m_nBasePoint1].m_vector *
                                                      (1.0 - dEffect);  
  }

  // Record hit point for trails
  if(dFrictionFactor > 0.001) {
    ++rWheel.m_nGroundHits;
  }
}

double BSimulation::BreakProfile(double dFrictionFactor) {
  // Break friction shall be speed dependent
  if((m_car.m_dSpeed * 1500) < 60) {
    double dFactor = ((1.0 / ((m_car.m_dSpeed * 1500) + 1.0)) - (1.0 / 61.0)) * (1.0 / (1.0 - (1.0 / 61.0)));
    return dFrictionFactor + (dFactor * (1.0 - dFrictionFactor));
  } else {
    return dFrictionFactor;
  }
}

void BSimulation::UpdateCarPointsUsingStrings() {
  for(int i = 0; i < m_car.m_nStrings; ++i) {
    BString *pString = &(m_car.m_pString[i]);
    BBodyPoint *p1 = &(m_car.m_pPoint[pString->m_nP1]);
    BBodyPoint *p2 = &(m_car.m_pPoint[pString->m_nP2]);
    BVector vNew1 = p1->m_vLocation + p1->m_vector;
    BVector vNew2 = p2->m_vLocation + p2->m_vector;

    double dLenFix = (pString->m_dIdealLength - (vNew1 - vNew2).Length());
    double dLengthFix1 = dLenFix * pString->m_dFactor1;
    double dLengthFix2 = dLenFix * pString->m_dFactor2;
    BVector vFix1 = vNew1 - vNew2;
    vFix1.ToUnitLength();
    p1->m_vector += vFix1 * dLengthFix1;
    p2->m_vector += vFix1 * -dLengthFix2;
	}
}

void BSimulation::MoveCarPoints() {
  for(int i = 0; i < m_car.m_nPoints; ++i) {
    m_car.m_pPoint[i].m_vLocation += m_car.m_pPoint[i].m_vector;
  }
} 



double BSimulation::PointInsideObject(BVector& rvPoint, BVector& rvNormal, double &rdFriction, double &rdBaseDepth) {
  double depth;
  for(int o = 0; o < GetGround()->m_nObjects; ++o) {
    if((depth = GetGround()->m_pObjects[o].PointIsInsideObject(rvPoint, rvNormal, rdFriction, rdBaseDepth)) > 0.0) {
      return depth;
    }
  }
  return -1.0;
}


double BSimulation::PointUnderGround(BVector& rvPoint, BVector& rvNormal, double &rdFriction, double &rdBaseDepth) {
  // Find corresponding triangle and check depth from there
  if((rvPoint.m_dX > 0.0) && (rvPoint.m_dY > 0.0)) {
    static double dX, dY, dXRes, dYRes;
    dX = rvPoint.m_dX / g_cdGroundScale;
    dY = rvPoint.m_dY / g_cdGroundScale;
    if((dX < g_cnGroundXSize - 1) && (dY < g_cnGroundYSize - 1)) {
      // Square has been found. Check which triangle to use
      int x = int(dX);
      int y = int(dY);
      dXRes = dX - double(x);
      dYRes = dY - double(y);
      BGround *pGround = GetGround();
      if(dXRes > dYRes) {
        // Check against triangle 1
        rvNormal = pGround->m_sqrHeightMapNormals[y * g_cnGroundXSize + x].m_vNormal1;
        // calculate depth
        double d1 = pGround->m_dHeightMap[y * g_cnGroundXSize + x];
        double d2 = pGround->m_dHeightMap[y * g_cnGroundXSize + (x + 1)];
        double d4 = pGround->m_dHeightMap[(y + 1) * g_cnGroundXSize + (x + 1)];
        double dRet = rvPoint.m_dZ - (d1 + dXRes * (d2 - d1) + dYRes * (d4 - d2));
        //dRet = SmoothedDepthAt(x, y, 1, dX, dY, rvPoint, dRet);
        rdFriction = pGround->m_sqrHeightMapNormals[y * g_cnGroundXSize + x].m_dFriction1;
        rdBaseDepth = rvNormal.ScalarProduct(BVector(0, 0, -dRet));
        if(pGround->m_sqrHeightMapNormals[y * g_cnGroundXSize + x].m_bWater1) {
          return rvPoint.m_dZ - g_cdWaterLevel;
        } else {
          return rdBaseDepth + pGround->m_sqrHeightMapNormals[y * g_cnGroundXSize + x].m_dSurfaceDepth1;
        }
      } else {
        // Check against triangle 2
        rvNormal = pGround->m_sqrHeightMapNormals[y * g_cnGroundXSize + x].m_vNormal2;
        // calculate depth
        double d1 = pGround->m_dHeightMap[y * g_cnGroundXSize + x];
        double d3 = pGround->m_dHeightMap[(y + 1) * g_cnGroundXSize + x];
        double d4 = pGround->m_dHeightMap[(y + 1) * g_cnGroundXSize + (x + 1)];
        double dRet = rvPoint.m_dZ - (d1 + dYRes * (d3 - d1) + dXRes * (d4 - d3));
        //dRet = SmoothedDepthAt(x, y, 2, dX, dY, rvPoint, dRet);
        rdFriction = pGround->m_sqrHeightMapNormals[y * g_cnGroundXSize + x].m_dFriction2;
        rdBaseDepth = rvNormal.ScalarProduct(BVector(0, 0, -dRet));
        if(pGround->m_sqrHeightMapNormals[y * g_cnGroundXSize + x].m_bWater2) {
          return rvPoint.m_dZ - g_cdWaterLevel;
        } else {
          return rdBaseDepth + pGround->m_sqrHeightMapNormals[y * g_cnGroundXSize + x].m_dSurfaceDepth2;
        }
      }
    }
  }
  // No square hit, use water values
  rdFriction = 0.2; 
  rdBaseDepth = rvPoint.m_dZ;
  rvNormal.Set(0.0, 0.0, -1.0);
  return rvPoint.m_dZ - g_cdWaterLevel;
}

double BSimulation::SmoothedDepthAt(int x, 
                                    int y, 
                                    int nTriangle, 
                                    double dX, 
                                    double dY,
                                    BVector &rvPoint,
                                    double dFlatDepth) {
  BGround *pGround = GetGround();
  BVector p1, p2, p3;
  BVector v1, v2, v3;
  double dScewRatio = 1.0;
  if(nTriangle == 1) {
    // corner points
    p1.Set(x * g_cdGroundScale, 
           y * g_cdGroundScale, 
           pGround->m_dHeightMap[y * g_cnGroundXSize + x]);
    p2.Set((x+1) * g_cdGroundScale, 
           y * g_cdGroundScale, 
           pGround->m_dHeightMap[y * g_cnGroundXSize + x+1]);
    p3.Set((x+1) * g_cdGroundScale, 
           (y+1) * g_cdGroundScale, 
           pGround->m_dHeightMap[(y+1) * g_cnGroundXSize + x+1]);

    // corner vectors
    v1 = pGround->m_sqrHeightMapNormals[y * g_cnGroundXSize + x].m_vNormalVis;
    v2 = pGround->m_sqrHeightMapNormals[y * g_cnGroundXSize + x+1].m_vNormalVis;
    v3 = pGround->m_sqrHeightMapNormals[(y+1) * g_cnGroundXSize + x+1].m_vNormalVis;

    // Scew
    dScewRatio = pGround->m_sqrHeightMapNormals[y * g_cnGroundXSize + x].m_vNormal1.ScalarProduct(BVector(0, 0, -1));
  } else {
    // corner points
    p1.Set(x * g_cdGroundScale, 
           y * g_cdGroundScale, 
           pGround->m_dHeightMap[y * g_cnGroundXSize + x]);
    p2.Set(x * g_cdGroundScale, 
           (y+1) * g_cdGroundScale, 
           pGround->m_dHeightMap[(y+1) * g_cnGroundXSize + x]);
    p3.Set((x+1) * g_cdGroundScale, 
           (y+1) * g_cdGroundScale, 
           pGround->m_dHeightMap[(y+1) * g_cnGroundXSize + x+1]);

    // corner vectors
    v1 = pGround->m_sqrHeightMapNormals[y * g_cnGroundXSize + x].m_vNormalVis;
    v2 = pGround->m_sqrHeightMapNormals[y * g_cnGroundXSize + x+1].m_vNormalVis;
    v3 = pGround->m_sqrHeightMapNormals[(y+1) * g_cnGroundXSize + x+1].m_vNormalVis;

    // Scew
    dScewRatio = pGround->m_sqrHeightMapNormals[y * g_cnGroundXSize + x].m_vNormal1.ScalarProduct(BVector(0, 0, -1));
  }  

  // Determine smoothed depth value
  double d1, d2, d3;
  BVector vFlat(rvPoint.m_dX, rvPoint.m_dY, dFlatDepth);
  d1 = (p1 - vFlat).Length();
  d2 = (p2 - vFlat).Length();
  d3 = (p3 - vFlat).Length();
  double dSqrt5 = sqrt(2 * g_cdGroundScale * g_cdGroundScale);
  if(d1 > dSqrt5) {
    d1 = dSqrt5;
  }
  if(d2 > dSqrt5) {
    d2 = dSqrt5;
  }
  if(d3 > dSqrt5) {
    d3 = dSqrt5;
  }
  d1 *= 1.0 / dSqrt5;
  d2 *= 1.0 / dSqrt5;
  d3 *= 1.0 / dSqrt5;

  BVector v1ToP = vFlat - p1;
  double dLen = v1ToP.Length();
  v1ToP.ToUnitLength();
  BVector vTmp = v1ToP.CrossProduct(v1);
  v1ToP = v1.CrossProduct(vTmp);
  v1ToP.ToUnitLength();
  v1ToP = v1ToP * dLen;

  BVector v2ToP = vFlat - p2;
  dLen = v2ToP.Length();
  v2ToP.ToUnitLength();
  vTmp = v2ToP.CrossProduct(v2);
  v2ToP = v2.CrossProduct(vTmp);
  v2ToP.ToUnitLength();
  v2ToP = v2ToP * dLen;

  BVector v3ToP = vFlat - p3;
  dLen = v3ToP.Length();
  v3ToP.ToUnitLength();
  vTmp = v3ToP.CrossProduct(v3);
  v3ToP = v3.CrossProduct(vTmp);
  v3ToP.ToUnitLength();
  v3ToP = v3ToP * dLen;

  return dFlatDepth + 
       ((cos(d1*3.141592654)+1.0)/2.0) * v1ToP.m_dZ * dScewRatio + 
       ((cos(d2*3.141592654)+1.0)/2.0) * v2ToP.m_dZ * dScewRatio +
       ((cos(d3*3.141592654)+1.0)/2.0) * v3ToP.m_dZ * dScewRatio;
}


double BSimulation::PointUnderGroundShadow(BVector& rvPoint, BVector& rvNormal) {
  // Find corresponding triangle and check depth from there  
  double dRet = rvPoint.m_dZ;
  rvNormal.Set(0.0, 0.0, -1.0);
  if((rvPoint.m_dX > 0.0) && (rvPoint.m_dY > 0.0)) {
    static double dX, dY, dXRes, dYRes;
    dX = rvPoint.m_dX / g_cdGroundScale;
    dY = rvPoint.m_dY / g_cdGroundScale;
    if((dX < g_cnGroundXSize - 1) && (dY < g_cnGroundYSize - 1)) {
      // Square has been found. Check which triangle to use
      int x = int(dX);
      int y = int(dY);
      dXRes = dX - double(x);
      dYRes = dY - double(y);
      BGround *pGround = GetGround();
      if(dXRes > dYRes) {
        // Check against triangle 1
        rvNormal = pGround->m_sqrHeightMapNormals[y * g_cnGroundXSize + x].m_vNormal1;
        // calculate depth
        double d1 = pGround->m_dHeightMap[y * g_cnGroundXSize + x];
        double d2 = pGround->m_dHeightMap[y * g_cnGroundXSize + (x + 1)];
        double d4 = pGround->m_dHeightMap[(y + 1) * g_cnGroundXSize + (x + 1)];
        dRet = rvPoint.m_dZ - (d1 + dXRes * (d2 - d1) + dYRes * (d4 - d2));
      } else {
        // Check against triangle 2
        rvNormal = pGround->m_sqrHeightMapNormals[y * g_cnGroundXSize + x].m_vNormal2;
        // calculate depth
        double d1 = pGround->m_dHeightMap[y * g_cnGroundXSize + x];
        double d3 = pGround->m_dHeightMap[(y + 1) * g_cnGroundXSize + x];
        double d4 = pGround->m_dHeightMap[(y + 1) * g_cnGroundXSize + (x + 1)];
        dRet = rvPoint.m_dZ - (d1 + dYRes * (d3 - d1) + dXRes * (d4 - d3));
      }
    }
  }

  // check if there's an object that the shadow should be cast on
  bool bChanged = false;
  double depth = PointUnderObjectsShadow(dRet, rvPoint, bChanged);

  return depth;
}

double BSimulation::PointUnderObjectsShadow(double& rdCandidate, BVector& rvPoint, bool& rbChanged) {
  BObject *pCastObj;
  rbChanged = false;
  static BVector vUp(0, 0, -1);
  double dSmallestDepth = 999999.9;
  for(int o = 0; o < GetGround()->m_nObjects; ++o) {
    BObject *pObj = &(GetGround()->m_pObjects[o]);
    if((rvPoint - pObj->m_vCenter).Length() < pObj->m_dRadius) {
      for(int nFace = 0; nFace < pObj->m_nFaces; ++nFace) {
        if(pObj->m_face[nFace].m_vNormal.m_dZ < -0.01) { // Up facing surface
          double depth = pObj->m_face[nFace].m_vNormal.ScalarProduct(rvPoint - pObj->m_face[nFace].m_vPoint[0]);
          double dProjDepth = (pObj->m_face[nFace].m_vNormal * depth).ScalarProduct(vUp);
          if(fabs(dProjDepth) < fabs(dSmallestDepth)) {
            dSmallestDepth = dProjDepth;
            pCastObj = pObj;
          }
        }
      }
    }
  }
  if((fabs(dSmallestDepth) < fabs(rdCandidate)) && 
     !pCastObj->m_boundary.PointIsOutside(rvPoint.m_dX, rvPoint.m_dY)) {
    rbChanged = true;
    return -dSmallestDepth;
  } else {
    return rdCandidate;
  }
}


double BSimulation::Friction(BVector& rPoint) {
  return 0.5; // NOT READY Use correct ground friction!
}

double BSimulation::Friction(BBodyPoint& rPoint) {
  return rPoint.m_dFriction * 0.5; // NOT READY Use correct ground friction!
}

double BSimulation::GroundHardnessAt(BVector& rvLocation) {
  return 0.5; // NOT READY Use correct ground hardness!
}




//*****************************************************************************
// CAMERA SETUP
//*****************************************************************************


extern double g_dRate;


void BSimulation::SetUpCamera() {
  // Always point towards the camera, always try to stay behind car
  static int nMode = 0;
  const static double cdTransitionSpeedMax = 0.1;
  static double dTransitionSpeedMax = cdTransitionSpeedMax;
  static double dTransitionSpeed = 0.0;
  BVector vUp(0, 0, -1);
  BVector vForward;

  m_car.m_vLocation = (m_car.m_pPoint[0].m_vLocation + 
                       m_car.m_pPoint[9].m_vLocation + 
                       m_car.m_pPoint[7].m_vLocation + 
                       m_car.m_pPoint[16].m_vLocation +
                       m_car.m_pPoint[1].m_vLocation + 
                       m_car.m_pPoint[10].m_vLocation + 
                       m_car.m_pPoint[6].m_vLocation + 
                       m_car.m_pPoint[15].m_vLocation) * (1.0 / 8.0);
  BVector location = m_camera.m_vLocation;
  if(m_camera.m_locMode == BCamera::FIXED) {
    m_camera.m_vLocation = m_camera.m_vFixLocation;
    vForward = (m_car.m_vLocation - m_camera.m_vLocation);
    if(dTransitionSpeedMax != cdTransitionSpeedMax) {
      dTransitionSpeed = 0.0;
      dTransitionSpeedMax = cdTransitionSpeedMax;
    }
  } else {
    if(m_camera.m_locMode == BCamera::FOLLOW) {
      BVector vTowardsBack;
      if(false /*m_bLiftingUp*/) {
        if(nMode != 0) {
          dTransitionSpeed = 0.0;
        }
        nMode = 0;
        // View from side to help lifting
        vTowardsBack  = (m_car.m_pPoint[9].m_vLocation - m_car.m_pPoint[0].m_vLocation) * 4;
        m_camera.m_vLocation = m_car.m_vLocation + vTowardsBack;
        m_camera.m_vLocation.m_dZ = m_car.m_vLocation.m_dZ - 2.5;        
      } else if((m_car.m_dSpeed * 1500.0) < 30.0) {
        // If stopped, place camera behind car, on left
        if(nMode != 1) {
          dTransitionSpeed = 0.0;
        }
        nMode = 1;
        vTowardsBack  = (m_car.m_pPoint[7].m_vLocation - m_car.m_pPoint[0].m_vLocation) * 1.5;
        // vTowardsBack += m_car.m_orientation.m_vRight * -2.0;
        m_camera.m_vLocation = m_car.m_vLocation + vTowardsBack;
        m_camera.m_vLocation.m_dZ = m_car.m_vLocation.m_dZ - 2.5;
      } else {
        if(nMode != 2) {
          dTransitionSpeed = 0.0;
        }
        nMode = 2;
        // If moving, place camera behind car, on car's trail
        BVector vDir = (m_car.m_pPoint[0].m_vector +
                        m_car.m_pPoint[9].m_vector +
                        m_car.m_pPoint[7].m_vector +
                        m_car.m_pPoint[16].m_vector) * 0.25;
        BVector vLoc = (m_car.m_pPoint[0].m_vLocation +
                        m_car.m_pPoint[9].m_vLocation +
                        m_car.m_pPoint[7].m_vLocation +
                        m_car.m_pPoint[16].m_vLocation) * 0.25;
        vTowardsBack  = vDir * -(1.0 / vDir.Length()) * 4.5;
        m_camera.m_vLocation = vLoc + vTowardsBack;
        m_camera.m_vLocation.m_dZ = vLoc.m_dZ - 1.5;
      }
      vForward = (m_car.m_vLocation + m_car.m_vector * 10.0 - m_camera.m_vLocation);
      if(dTransitionSpeedMax != cdTransitionSpeedMax) {
        dTransitionSpeed = 0.0;
        dTransitionSpeedMax = cdTransitionSpeedMax;
      }
    } else if(m_camera.m_locMode == BCamera::OVERVIEW) {
      // Place camera high above
      if(m_camera.m_bInitLoc) {
        m_camera.m_bInitLoc = false;
        BVector vTowardsBack = (m_car.m_pPoint[7].m_vLocation - 
                                m_car.m_pPoint[0].m_vLocation);
        vTowardsBack.m_dZ = 0.0;
        vTowardsBack.ToUnitLength();
        m_camera.m_vOverview = vTowardsBack * 40.0 + BVector(0, 0, -40.0);
      }
      m_camera.m_vLocation = m_car.m_vLocation + m_camera.m_vOverview;
      vForward = (m_car.m_vLocation - m_camera.m_vLocation);
      if(dTransitionSpeedMax != cdTransitionSpeedMax) {
        dTransitionSpeed = 0.0;
        dTransitionSpeedMax = cdTransitionSpeedMax;
      }
    } else if(m_camera.m_locMode == BCamera::INCAR) {
      // Place camera over hood
      BVector vLoc = (m_car.m_pPoint[3].m_vLocation +
                      m_car.m_pPoint[4].m_vLocation +
                      m_car.m_pPoint[12].m_vLocation +
                      m_car.m_pPoint[13].m_vLocation) * 0.25 +
                     (m_car.m_pPoint[1].m_vLocation -
                      m_car.m_pPoint[0].m_vLocation) * 0.05;
      m_camera.m_vLocation = vLoc;
      vUp = (m_car.m_pPoint[1].m_vLocation - m_car.m_pPoint[0].m_vLocation);
      vUp.ToUnitLength();
      vForward = (m_car.m_pPoint[0].m_vLocation - m_car.m_pPoint[7].m_vLocation);
      vForward.ToUnitLength();
      dTransitionSpeedMax = 1.0;
      dTransitionSpeed = 1.0;
    } else if(m_camera.m_locMode == BCamera::ONSIDE) {
      // Place camera on left side of car
      BVector vLoc = m_car.m_pPoint[8].m_vLocation -
                     m_car.m_orientation.m_vRight * 0.5;
      m_camera.m_vLocation = vLoc;
      //vUp = (m_car.m_pPoint[1].m_vLocation - m_car.m_pPoint[0].m_vLocation);
      //vUp.ToUnitLength();
      vForward = m_car.m_orientation.m_vForward + vUp * 0.2;
      vForward.ToUnitLength();
      dTransitionSpeedMax = 1.0;
      dTransitionSpeed = 1.0;
    }    
  }

  double dCalibXSpeed = dTransitionSpeed; // pow(dTransitionSpeed, 1 / (g_dRate / 60.0));

  m_camera.m_vLocation = location * (1.0 - dCalibXSpeed) + 
                         m_camera.m_vLocation * dCalibXSpeed;
  if(dTransitionSpeed < dTransitionSpeedMax) {
    dTransitionSpeed += 0.01;
  }

  BOrientation orientation = m_camera.m_orientation;
  m_camera.m_orientation.m_vForward = vForward;
  m_camera.m_orientation.m_vUp = vUp;
  m_camera.m_orientation.m_vRight = m_camera.m_orientation.m_vForward.CrossProduct(m_camera.m_orientation.m_vUp);
  m_camera.m_orientation.Normalize();

  if(m_camera.m_locMode == BCamera::INCAR) {
    m_camera.m_orientation.m_vForward = m_camera.m_orientation.m_vForward * 0.4 + orientation.m_vForward * 0.6;
    m_camera.m_orientation.m_vUp      = m_camera.m_orientation.m_vUp      * 0.4 + orientation.m_vUp      * 0.6;
    m_camera.m_orientation.m_vRight   = m_camera.m_orientation.m_vRight   * 0.4 + orientation.m_vRight   * 0.6;
  } else {
    m_camera.m_orientation.m_vForward = m_camera.m_orientation.m_vForward * dTransitionSpeed + orientation.m_vForward * (1.0 - dTransitionSpeed);
    m_camera.m_orientation.m_vUp      = m_camera.m_orientation.m_vUp      * 0.4              + orientation.m_vUp      * 0.6;
    m_camera.m_orientation.m_vRight   = m_camera.m_orientation.m_vRight   * dTransitionSpeed + orientation.m_vRight   * (1.0 - dTransitionSpeed);
  }
  m_camera.m_orientation.Normalize();
}




//*****************************************************************************
// EFFECTS: TRAILS
//*****************************************************************************


void BSimulation::UpdateTrails() {
  bool bSkidding = false;
  for(int w = 0; w < m_car.m_nWheels; ++w) {
    BWheel *pWheel = &(m_car.m_pWheel[w]);
    if(m_car.m_dSpeed > 0.01) {
      if(pWheel->m_nGroundHits > 0) { // 5 = 15/3, 15 = number of updates between visualization
        bSkidding = true;
        int nPrev = pWheel->m_nTrailHead;

        // Insert new trail point
        if(pWheel->m_nTrailPoints < g_cnMaxTrailPoints) {
          ++pWheel->m_nTrailPoints;
        }
        pWheel->m_nTrailHead = (pWheel->m_nTrailHead + 1) % g_cnMaxTrailPoints;
        BVector vNormal;
        double dTmp, dBaseDepth;
        double depth = PointUnderGround(pWheel->m_vLocSample, vNormal, dTmp, dBaseDepth);
        bool bChanged = false;
        depth = PointUnderObjectsShadow(depth, pWheel->m_vLocSample, bChanged);
        BVector vLoc;
        if(bChanged) {
          vLoc = pWheel->m_vLocSample + vNormal * depth;
        } else {
          vLoc = pWheel->m_vLocSample + vNormal * dBaseDepth;
        }
        BVector v1 = vLoc + pWheel->m_orientation.m_vRight * -0.1;
        BVector v2 = vLoc + pWheel->m_orientation.m_vRight * 0.1;
        double  d = PointUnderGround(v1, vNormal, dTmp, dBaseDepth);
        d = PointUnderObjectsShadow(d, v1, bChanged);
        if(bChanged) {
          v1 += vNormal * d;
        } else {
          v1 += vNormal * dBaseDepth;
        }
        d = PointUnderGround(v2, vNormal, dTmp, dBaseDepth);
        d = PointUnderObjectsShadow(d, v2, bChanged);
        if(bChanged) {
          v2 += vNormal * d;
        } else {
          v2 += vNormal * dBaseDepth;
        }
        pWheel->m_pTrailPoint[pWheel->m_nTrailHead].m_vLocation[0] = v1;
        pWheel->m_pTrailPoint[pWheel->m_nTrailHead].m_vLocation[1] = v2;
        pWheel->m_pTrailPoint[pWheel->m_nTrailHead].m_bStart = false;
        pWheel->m_pTrailPoint[pWheel->m_nTrailHead].m_bEnd   = false;
        pWheel->m_pTrailPoint[pWheel->m_nTrailHead].m_dStrength = double(pWheel->m_nGroundHits) / m_nPhysicsSteps;

        if(pWheel->m_nTrailPoints > 1) {
          if(pWheel->m_pTrailPoint[nPrev].m_bEnd) {
            pWheel->m_pTrailPoint[pWheel->m_nTrailHead].m_bStart = true;
          }
        }

      } else {
        if(pWheel->m_nTrailPoints) {
          // Mark end of this trail
          pWheel->m_pTrailPoint[pWheel->m_nTrailHead].m_bEnd = true;
        }
      }      
    } else {
      if(pWheel->m_nTrailPoints) {
        // Mark end of this trail
        pWheel->m_pTrailPoint[pWheel->m_nTrailHead].m_bEnd = true;
      }
    }
    pWheel->m_nGroundHits = 0;
  }

  // Update skid sound volume
  if(bSkidding) {
    if(m_car.m_dSpeedKmh < 400.0) {
      SoundModule::SetSkidSoundVolume(int(m_car.m_dSpeedKmh / 4.0));
    } else {
      SoundModule::SetSkidSoundVolume(100);
    }
  } else {
    SoundModule::SetSkidSoundVolume(0);
  }
}



void BSimulation::DrawShadowAndTrails() {
  static BVector vShadow[5];
  static int i;

  glEnable(GL_BLEND);
  glDisable(GL_DEPTH_TEST);
  glBlendFunc(GL_ZERO, GL_ONE_MINUS_SRC_COLOR);

  // Shadow
  OpenGLHelpers::SetColorFull(0.3, 0.3, 0.3, 0);
  GLfloat fLight[4];
  fLight[0] = 0.0f;
  fLight[1] = 0.0f;
  fLight[2] = 0.0f;
  fLight[3] = 0.0f;
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, fLight);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, fLight);

  vShadow[0] = m_car.m_pPoint[0].m_vLocation;
  vShadow[1] = m_car.m_pPoint[7].m_vLocation;
  vShadow[2] = m_car.m_pPoint[9].m_vLocation;
  vShadow[3] = m_car.m_pPoint[16].m_vLocation;
  for(i = 0; i < 4; ++i) {
    vShadow[i].m_dZ = vShadow[i].m_dZ - PointUnderGroundShadow(vShadow[i], vShadow[4]);
  }
  BVector vCenter = (vShadow[0] + vShadow[1] + vShadow[2] + vShadow[3]) * 0.25;
  vShadow[0] = (vShadow[0] - vCenter) * 1.3 + vCenter;
  vShadow[1] = (vShadow[1] - vCenter) * 1.3 + vCenter;
  vShadow[2] = (vShadow[2] - vCenter) * 1.3 + vCenter;
  vShadow[3] = (vShadow[3] - vCenter) * 1.3 + vCenter;

  glEnable(GL_TEXTURE_2D);
  glCallList(DISPLIST_SHADOWTXTR);
  glBegin(GL_TRIANGLE_STRIP);
  glTexCoord2d(0.0, 0.0);
  glVertex3d(vShadow[0].m_dX, vShadow[0].m_dY, vShadow[0].m_dZ);
  glTexCoord2d(0.0, 1.0);
  glVertex3d(vShadow[1].m_dX, vShadow[1].m_dY, vShadow[1].m_dZ);
  glTexCoord2d(1.0, 0.0);
  glVertex3d(vShadow[2].m_dX, vShadow[2].m_dY, vShadow[2].m_dZ);
  glTexCoord2d(1.0, 1.0);
  glVertex3d(vShadow[3].m_dX, vShadow[3].m_dY, vShadow[3].m_dZ);
  glEnd();
  glDisable(GL_TEXTURE_2D);

  // Trails
  DrawTrails();

  glEnable(GL_DEPTH_TEST);
  glDisable(GL_BLEND);
}

void BSimulation::DrawTrails() {
  //OpenGLHelpers::SetColorFull(1, 1, 1, 0);
  static BVector vTrail[4];
  // Loop through all wheels and draw their trails
  for(int w = 0; w < m_car.m_nWheels; ++w) {
    BWheel *pWheel = &(m_car.m_pWheel[w]);
    int nPrev = pWheel->m_nTrailHead;
    for(int i = pWheel->m_nTrailPoints - 1; i >= 0; --i) {
      int nTrailPoint = ((pWheel->m_nTrailHead + g_cnMaxTrailPoints) - i) % g_cnMaxTrailPoints;

      if(pWheel->m_pTrailPoint[nTrailPoint].m_bStart || 
         (i == pWheel->m_nTrailPoints - 1)) {
        if(pWheel->m_pTrailPoint[nTrailPoint].m_bEnd) {
          // Draw a spot
          vTrail[0] = pWheel->m_pTrailPoint[nTrailPoint].m_vLocation[1];
          vTrail[1] = pWheel->m_pTrailPoint[nTrailPoint].m_vLocation[0];
          vTrail[2] = pWheel->m_pTrailPoint[nTrailPoint].m_vLocation[1] + BVector(0.2, 0, 0);
          vTrail[3] = pWheel->m_pTrailPoint[nTrailPoint].m_vLocation[0] + BVector(0.2, 0, 0);
          OpenGLHelpers::SetColorFull(0.2, 0.2, 0.2, 0);
          OpenGLHelpers::TriangleStrip(vTrail, 4);
        }
        nPrev = nTrailPoint;
        continue;
      }

      // Draw rectangle from previous to current
      vTrail[0] = pWheel->m_pTrailPoint[nPrev].m_vLocation[1];
      vTrail[1] = pWheel->m_pTrailPoint[nPrev].m_vLocation[0];
      vTrail[2] = pWheel->m_pTrailPoint[nTrailPoint].m_vLocation[1];
      vTrail[3] = pWheel->m_pTrailPoint[nTrailPoint].m_vLocation[0];
      double dColor = 0.05 + pWheel->m_pTrailPoint[nTrailPoint].m_dStrength * 0.2;
      OpenGLHelpers::SetColorFull(dColor, dColor, dColor, 0);
      OpenGLHelpers::TriangleStrip(vTrail, 4);
      nPrev = nTrailPoint;
    }
  }
}


//*****************************************************************************
// RECORDING
//*****************************************************************************

void BSimulation::StartRecording() {
  m_fp = fopen("sound.txt", "w");
}

void BSimulation::StopRecording() {
  fclose(m_fp);
  m_fp = 0;
}

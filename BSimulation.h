//
// Simulation
//

#pragma once

#include "BaseClasses.h"
#include "BCar.h"
#include "BGround.h"
#include "BCamera.h"
//#include "ControllerModule.h"

class BSimulation {

  GLubyte m_pTxtrJet[(8 * 8 * 3) * 2];
  GLubyte m_pTxtrBlade[(8 * 8 * 3) * 2];
  GLubyte m_pTxtrShadow[(8 * 8 * 3) * 2];

  BCar    m_car;
  BGround m_ground;
  BCamera m_camera;

  FILE   *m_fp;

  void ApplyGroundEffectToAPoint(int i, 
                                 BVector& vNormal, 
                                 double depth, 
                                 double dGroundFriction, 
                                 double dBaseDepth);

public:

  static const double g_cdBrakesFriction;
  static const double g_cdABSBrakeFriction;
  static const double g_cdMaxSpeed;
  static const double g_cdAccelerationFactor;
  static const double g_cdGravityZ;
  static const double g_cdTurnFactor;

  int m_nDispWidth;
  int m_nDispHeight;
  int m_nDispBits;
  int m_nDispHz;

  int                  m_nController;
  //BControllerState     m_controllerstate; // Access to a controller, such as a joystick or a wheel

  bool   m_bLiftingUp; // Is the car being lifted up by a virtual crane
  double m_dLiftZ;     // Z coordinate of the car when the lifting was started
  bool   m_bRaining;   // Debug code to the ground depth
  bool   m_bDisplayInfo; // Show fps, speed, altitude etc.
  bool   m_bPaused;
  bool   m_bSteeringAidOn;
  bool   m_bCalibrateSimulationSpeed;

  bool   m_bHitDetected;
  double m_dHitSpeed;

  bool   m_bRecordTrail;

  double m_dAccelerationFactor;
  double m_dTurnFactor;
  int    m_nPhysicsStepsBetweenRender;
  double m_dPhysicsFraction;
  int    m_nPhysicsSteps;

  int    m_nSkyDetail;
  int    m_nDistantDetail;
  int    m_nWaterDetail;
  int    m_nColorMode;
  int    m_nScreenFormat;
  int    m_nTextureSmoothness; 

  enum THeliPart {LEFTHATCH, RIGHTHATCH, COVER, BLADE};
  double m_dRotorVolFactor;

  BSimulation();
  ~BSimulation();

  BCar    *GetCar()    {return &m_car;}
  BGround *GetGround() {return &m_ground;}
  BCamera *GetCamera() {return &m_camera;}

  void StartRecording();
  void StopRecording();

  //void PrePaint(CDC *pDC);
  void PrePaint();
  void PreProcessVisualization();
  //void Paint(CDC *pDC);
  void Paint();
  void DrawShadowAndTrails();
  void DrawTrails();
  void DrawRotor();
  void DrawRetractedBlades(BVector& rvCenter, 
                           double dRad, 
                           double dPhase, 
                           double dAngle);
  void CheckHeliPartForDamage(THeliPart part, BVector vPoint, int nInfo = 0);
  void DamageHeliPart(THeliPart part, int nInfo = 0);
  void DrawJet();

  void SetUpCamera();
  void UpdateCar();
  void UpdateHeliControls();
  void ToggleHeli();
  void ToggleJet();
  void SwitchCameraMode();
  void UpdateJetControls();
  void UpdateTrails();
  void ApplyAirDragAndGravity();
  bool WheelTouchesGround(BWheel& rWheel);
  bool ApplyWheelForcesFromObjects(BWheel& rWheel);
  void ApplyWheelForces(BWheel& rWheel);
  double BreakProfile(double dFrictionFactor);
	void ApplySteering();
	void ApplyGroundEffect();
	void MoveCarPoints();
	void UpdateCarPointsUsingStrings();

  inline double ThermoDynamicLoss() {return 0.4;} // 0 = lo loss, 1 = total loss

  double PointUnderGround(BVector& rvPoint, BVector& rvNormal, double &rdFriction, double &rdBaseDepth);
  double PointInsideObject(BVector& rvPoint, BVector& rvNormal, double &rdFriction, double &rdBaseDepth);
  double SmoothedDepthAt(int x, 
                         int y, 
                         int nTriangle, 
                         double dX, 
                         double dY,
                         BVector &rPoint,
                         double dFlatDepth);
  double PointUnderGroundShadow(BVector& rvPoint, BVector& rvNormal);
  double PointUnderObjectsShadow(double& rdCandidate, BVector& rvPoint, bool& rbChanged);
  double Friction(BBodyPoint& rPoint);
  double Friction(BVector& rPoint);
  double GroundHardnessAt(BVector& rvLocation);
};

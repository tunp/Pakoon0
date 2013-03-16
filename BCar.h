//
// Car
//

#pragma once

#include "BaseClasses.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <string>

class BCar {

  BVector m_vMassCenter;

  GLubyte m_dTXTRCar[256 * 256 * 3 * 2];
  // GLubyte m_dTXTRWheel[16 * 16 * 3 * 2];
  GLubyte m_dTXTRWheel[128 * 128 * 3 * 2];

  double LengthAlongZ(BVector &rv);
  void   RotateAroundZ(BOrientation &rOrientation, double dAngle);
  double LengthAlongX(BVector &rv);
  void   RotateAroundX(BOrientation &rOrientation, double dAngle);
  double AngleBetweenVectorsAlongX(const BVector &v1, const BVector &v2, double dSign);
  double AngleBetweenVectorsAlongY(const BVector &v1, const BVector &v2, double dSign);
  double AngleBetweenVectorsAlongZ(const BVector &v1, const BVector &v2, double dSign);

public:
  BOrientation m_orientation;   // orientation of the car
  double       m_dSpeed;        // current speed (meters/one update step)
  double       m_dSpeedKmh;     // current speed (in km/h)
  BVector      m_vLocation;     // vector describing center of mass location
  BVector      m_vHomeLocation; // Initial location
  BVector      m_vector;        // vector describing next position
  int          m_nPoints;       // Number of body points
  BBodyPoint  *m_pPoint;        // Points making the car body
  BVector     *m_pPointOrig;    // Original points making the car body
  BVector     *m_pPointVis;     // Visualization points making the car body
  int          m_nStrings;      // Number of body strings
  BString     *m_pString;       // Strings holding the body together
  int          m_nWheels;       // Number of wheels
  BWheel      *m_pWheel;        // Wheels of the car
  bool         m_bBreaking;     // Breaks are on?
  bool         m_bABS;          // ABS is on?
  bool         m_bHandBreaking; // Handbreak is on?
  bool         m_bAccelerating; // Acceleration pedal is down?
  double       m_dAccelerationFactor;
  bool         m_bReversing;    // Acceleration pedal is down and gear is on R?
  double       m_dReversingFactor;
  bool         m_bTurningLeft;  // guess
  bool         m_bTurningRight; // guess
  double       m_dTurn;         // Amount of front wheel turn at the moment (1.0 = full right, -1.0 = full left, 0.0 = centered)
  bool         m_bWireframe;    // Draw as wireframe
  double       m_dHeliMode;     // Heli mode phase
  int          m_nHeliMode;     // Helicopter mode phase
  bool         m_bHeliModeActivating; // Going to heli mode or coming out?
  bool         m_bHeliLifting;  
  bool         m_bHeliForwarding;
  bool         m_bHeliBacking;
  bool         m_bHeliRighting;  
  bool         m_bHeliLefting;
  double       m_dHeliLift;
  double       m_dHeliForward;
  double       m_dHeliRight;
  bool         m_bHeliBladeOK[3];
  bool         m_bHeliCoverOK;
  bool         m_bHeliHatchesOK;
  double       m_dHeliLeftHatchOffsetAngle;
  double       m_dHeliRightHatchOffsetAngle;
  double       m_dHeliBladePower;
  double       m_dSteeringAid;

  double       m_dJetMode;           // Jet mode phase
  int          m_nJetMode;           // Jet mode phase
  bool         m_bJetModeActivating; // Going to jet mode or coming out?
  double       m_dRPM; // Engine rounds per minute or something like that
  
  BCar();
  virtual ~BCar();

  void LoadCarFromFile(std::string sFilename);
  void LoadTextures();
  void InitAll();
  void DeleteAll();

  void    Move(BVector vRelMove, bool bAutoHeight = true);
  BVector GetHomeLocation() {return m_vHomeLocation;}
  void    Paint(int m_nPhysicsSteps);
  void    RotateCarToRightOrientation();
  void    PreProcessVisualization();
  void    RenderWheelBlade(double dLen, 
                           double dWide, 
                           double dNarrow, 
                           double dWideRound,
                           double dNarrowRound,
                           double dDepthOut,
                           double dDepthIn);
};

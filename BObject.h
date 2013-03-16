//
// Object
//

#pragma once

#include "BaseClasses.h"
#include <GL/gl.h>
#include <GL/glu.h>

class BFace {
public:
  int     m_nPoints;
  BVector m_vPoint[5];
  BVector m_vNormal;

  BFace() {m_nPoints = 0;}
  void SetPoints(int nPoints, BVector *pvPoints);
  void CalculateNormal();
};

class BObject {
public:
  int       m_nFaces;
  BFace     m_face[6];
  BVector   m_vOrigin;
  BVector   m_vCenter;
  BVector   m_vScale;
  double    m_dRadius;
  double    m_dRed;
  double    m_dGreen;
  double    m_dBlue;
  double    m_dFriction;
  BBoundary m_boundary;

  BObject() {m_nFaces = 0;}
  void   Setup();
  double PointIsInsideObject(BVector& rvPoint, 
                             BVector& rvNormal, 
                             double& rdGroundFriction, 
                             double& rdBaseDepth);
  void CreateXYBoundary();
};

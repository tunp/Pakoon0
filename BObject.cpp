//
// Object
//

#include "BObject.h"

#include "OpenGLHelpers.h"

#include <algorithm>

extern bool g_cbBlackAndWhite;

void BFace::SetPoints(int nPoints, BVector *pvPoints) {
  //ASSERT(nPoints >= 3);
  for(int i = 0; i < std::min(nPoints, 5); ++i) {
    m_vPoint[i] = pvPoints[i];
  }
  m_nPoints = std::min(nPoints, 5);
}

void BFace::CalculateNormal() {
  // Calculate normal for the face
  BVector v1, v2;
  v1 = m_vPoint[1] - m_vPoint[0];
  v2 = m_vPoint[2] - m_vPoint[1];
  m_vNormal = v1.CrossProduct(v2);
  m_vNormal.ToUnitLength();
}

void BObject::Setup() {
  // Calculate center and place points according to scale and origin
  int nPoints = 0;
  BVector vCenter(0, 0, 0);
  static bool bDone = false;
  int nFace;
  int i;
  if(!bDone) {
    for(nFace = 0; nFace < m_nFaces; ++nFace) {
      for(i = 0; i < m_face[nFace].m_nPoints; ++i) {
        m_face[nFace].m_vPoint[i].Set(m_vOrigin.m_dX + m_face[nFace].m_vPoint[i].m_dX * m_vScale.m_dX,
                                      m_vOrigin.m_dY + m_face[nFace].m_vPoint[i].m_dY * m_vScale.m_dY,
                                      m_vOrigin.m_dZ + m_face[nFace].m_vPoint[i].m_dZ * m_vScale.m_dZ);
        ++nPoints;
        vCenter += m_face[nFace].m_vPoint[i];
      }
      m_face[nFace].CalculateNormal();
    }
  }
  m_vCenter = vCenter * (1.0 / double(nPoints));
  m_dRadius = 0.1;
  for(nFace = 0; nFace < m_nFaces; ++nFace) {
    for(i = 0; i < m_face[nFace].m_nPoints; ++i) {
      if((m_vCenter - m_face[nFace].m_vPoint[i]).Length() > m_dRadius) {
        m_dRadius = (m_vCenter - m_face[nFace].m_vPoint[i]).Length();
      }
    }
  }
  CreateXYBoundary();
}

double BObject::PointIsInsideObject(BVector& rvPoint, 
                                    BVector& rvNormal, 
                                    double& rdGroundFriction, 
                                    double& rdBaseDepth) {
  // Point is inside object if it's on the inside of each face
  // If inside, return the normal of the face whose's surface is closest to
  // the point

  int    nHitFace = 0;
  double dMinDepth = 9999.0;
  for(int nFace = 0; nFace < m_nFaces; ++nFace) {
    BVector vTmp;
    vTmp = rvPoint - m_face[nFace].m_vPoint[0];
    double dDepth = -m_face[nFace].m_vNormal.ScalarProduct(vTmp);
    if(dDepth > 0.0) {
      if(dDepth < dMinDepth) {
        nHitFace = nFace;
        dMinDepth = dDepth;
      }
    } else {
      return -1.0; // Not inside object
    }
  }
  rvNormal = m_face[nHitFace].m_vNormal;
  rdGroundFriction = m_dFriction;
  rdBaseDepth = dMinDepth;
  return dMinDepth;
}


void BObject::CreateXYBoundary() {
  m_boundary.Empty();
  static B2DPoint points[100];
  int    nXMin, nXMax, nYMin, nYMax;
  double dXMin = 99999.9, dXMax = -99999.9, dYMin = 99999.9, dYMax = -99999.9;
  int nPoints = 0;
  // Find all object points
  int nFace;
  int i;
  for(nFace = 0; nFace < m_nFaces; ++nFace) {
    for(i = 0; i < m_face[nFace].m_nPoints; ++i) {
      points[nPoints].m_dX = m_face[nFace].m_vPoint[i].m_dX;
      points[nPoints].m_dY = m_face[nFace].m_vPoint[i].m_dY;

      if(points[nPoints].m_dX < dXMin) {
        nXMin = nPoints;
        dXMin = points[nPoints].m_dX;
      }

      if(points[nPoints].m_dX > dXMax) {
        nXMax = nPoints;
        dXMax = points[nPoints].m_dX;
      }

      if(points[nPoints].m_dY < dYMin) {
        nYMin = nPoints;
        dYMin = points[nPoints].m_dY;
      }

      if(points[nPoints].m_dY > dYMax) {
        nYMax = nPoints;
        dYMax = points[nPoints].m_dY;
      }

      ++nPoints;
    }
  }

  BBoundaryPoint *pos;
  pos = m_boundary.AddAfter(m_boundary.Head(), points[nXMin]);
  pos = m_boundary.AddAfter(pos, points[nYMax]);
  pos = m_boundary.AddAfter(pos, points[nXMax]);
  pos = m_boundary.AddAfter(pos, points[nYMin]);

  for(i = 0; i < nPoints; ++i) {
    if(pos = m_boundary.PointIsOutside(points[i])) { // pos = start point of line outside which p is
      BBoundaryPoint *pNew = m_boundary.AddAfter(pos, points[i]);
      BBoundaryPoint *pLoop = pNew->m_pNext;
      while(pLoop && (pLoop != pNew)) {
        if(m_boundary.PointIsLeftOfLine(pos, pLoop->m_dX, pLoop->m_dY)) {
          pLoop = m_boundary.Delete(pLoop);
        } else {
          pLoop = pLoop->m_pNext;
        }
      }
    }
  }
}

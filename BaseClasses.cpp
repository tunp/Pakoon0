//
// Base classes for the Getaway
//

#include "BaseClasses.h"

//**********************************************************************


#define VDot(a, b, c) {a=(b).dX*(c).dX+(b).dY*(c).dY+(b).dZ*(c).dZ;}

//**********************************************************************

BString::BString() {
  m_nP1 = 0;
  m_nP2 = 0;
  m_dIdealLength = 1.0;
  m_dRigidity = 1.0;
}

//**********************************************************************

BBodyPoint::BBodyPoint() {
  m_dFriction = 1.0;
  m_dMass = 1.0;
}


BBoundaryPoint *BBoundary::AlreadyInserted(B2DPoint& rP) {
  if(m_pHead) {
    BBoundaryPoint *pLoop = m_pHead;
    do {
      if((fabs(pLoop->m_dX - rP.m_dX) < 0.001) && 
         (fabs(pLoop->m_dY - rP.m_dY) < 0.001)) {
        return pLoop;
      }
      pLoop = pLoop->m_pNext;
    } while(pLoop != m_pHead);
  } 
  return 0;
}

void BBoundary::Empty() {
  while(m_pHead) {
    BBoundaryPoint *p = m_pHead;
    if(m_pHead->m_pNext != m_pHead) {
      m_pHead = m_pHead->m_pNext;
    } else {
      m_pHead = 0;
    }    
    delete p;
  }
}

bool BBoundary::PointIsLeftOfLine(BBoundaryPoint *pStart, double dX, double dY) {
  static BVector vUp(0, 0, -1);
  BVector vTest(dX - pStart->m_dX, dY - pStart->m_dY, 0);
  BVector vTmp(pStart->m_pNext->m_dX - pStart->m_dX, 
               pStart->m_pNext->m_dY - pStart->m_dY, 
               0);
  vTmp.ToUnitLength();
  BVector vRef = vUp.CrossProduct(vTmp);
  return vRef.ScalarProduct(vTest) < 0.0;
}

BBoundaryPoint *BBoundary::AddAfter(BBoundaryPoint *pAfter, B2DPoint& rNew2D) {
  BBoundaryPoint *pOld;
  if(!(pOld = AlreadyInserted(rNew2D))) {
    BBoundaryPoint *pNew = new BBoundaryPoint;
    pNew->m_dX = rNew2D.m_dX;
    pNew->m_dY = rNew2D.m_dY;
    if(pAfter) {
      BBoundaryPoint *pTmp = pAfter->m_pNext;
      pAfter->m_pNext = pNew;
      pNew->m_pNext = pTmp;
    } else {
      m_pHead = pNew;
      m_pHead->m_pNext = m_pHead;
    }
    return pNew;
  } else {
    return pOld;
  }
}

BBoundaryPoint *BBoundary::PointIsOutside(B2DPoint& rP) {
  return PointIsOutside(rP.m_dX, rP.m_dY);
}

BBoundaryPoint *BBoundary::PointIsOutside(double dX, double dY) {
  if(m_pHead) {
    BBoundaryPoint *pLoop = m_pHead;
    do {
      if(PointIsLeftOfLine(pLoop, dX, dY)) {
        return pLoop;
      }
      pLoop = pLoop->m_pNext;
    } while(pLoop != m_pHead);
  }
  return 0;
}

BBoundaryPoint *BBoundary::Delete(BBoundaryPoint *p) {
  if(m_pHead) {
    if(p->m_pNext == p) {
      // Last one
      delete p;
      m_pHead = 0;
      return 0;
    }
    BBoundaryPoint *pTmp = p;
    while(pTmp->m_pNext != p) {
      pTmp = pTmp->m_pNext;
    }
    pTmp->m_pNext = p->m_pNext;
    delete p;
    return pTmp->m_pNext;
  }
  return 0;
}

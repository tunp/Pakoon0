//
// Base classes for the Getaway
//

#pragma once

#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>
//#include <SDL/SDL_opengl.h>
#include <cstring>


const int g_cnMaxTrailPoints = 100;

class BIndexGuard {
  int m_nMax;
  int m_nMapTo;
public:
  BIndexGuard(int nMax, int nMapTo) {m_nMax = nMax; m_nMapTo = nMapTo;};
  inline int operator()(int nCandidate) {
    if((nCandidate < 0) || (nCandidate > m_nMax)) {
      return m_nMapTo;
    } else {
      return nCandidate;
    }
  };
};

class BVector {
public:
  double m_dX;
  double m_dY;
  double m_dZ;

  inline BVector() {m_dX = 0.0; m_dY = 0.0; m_dZ = 0.0;};
  inline BVector(double a, double b, double c) {m_dX = a; m_dY = b; m_dZ = c;};

  inline void    Set(double a, double b, double c) {m_dX = a; m_dY = b; m_dZ = c;};
  inline BVector operator+(BVector p) {
    BVector vRet;
    vRet.m_dX = m_dX + p.m_dX;
    vRet.m_dY = m_dY + p.m_dY;
    vRet.m_dZ = m_dZ + p.m_dZ;
    return vRet;
  };
  inline BVector& operator+=(BVector p) {
    m_dX += p.m_dX;
    m_dY += p.m_dY;
    m_dZ += p.m_dZ;
    return *this;
  };
  inline BVector operator-(BVector p) {
    BVector vRet;
    vRet.m_dX = m_dX - p.m_dX;
    vRet.m_dY = m_dY - p.m_dY;
    vRet.m_dZ = m_dZ - p.m_dZ;
    return vRet;
  };
  inline BVector& operator-=(BVector p) {
    m_dX -= p.m_dX;
    m_dY -= p.m_dY;
    m_dZ -= p.m_dZ;
    return *this;
  };
  inline BVector operator*(double d) {
    BVector vRet;
    vRet.m_dX = m_dX * d;
    vRet.m_dY = m_dY * d;
    vRet.m_dZ = m_dZ * d;
    return vRet;
  };
  inline double Length() {return sqrt((m_dX * m_dX) + (m_dY * m_dY) + (m_dZ * m_dZ));};
  inline BVector CrossProduct(BVector p) {
    BVector vRet;
    vRet.Set(m_dY * p.m_dZ - m_dZ * p.m_dY,
             m_dZ * p.m_dX - m_dX * p.m_dZ,
             m_dX * p.m_dY - m_dY * p.m_dX);
    return vRet;
  };
  inline double  ScalarProduct(BVector p) {
    return m_dX * p.m_dX + m_dY * p.m_dY + m_dZ * p.m_dZ;
  };
  inline void    ToUnitLength() {
    double dLen = Length();
    m_dX *= 1.0 / dLen;
    m_dY *= 1.0 / dLen;
    m_dZ *= 1.0 / dLen;
  };
};

class BOrientation {
public:

  BVector m_vForward;
  BVector m_vRight;
  BVector m_vUp;

  BOrientation() {m_vForward.Set(0, 1, 0);m_vRight.Set(1, 0, 0);m_vUp.Set(0, 0, -1);};

  void Normalize() {m_vForward.ToUnitLength(); m_vRight.ToUnitLength(); m_vUp.ToUnitLength();};
};

class BBodyPoint {
public:
  BVector m_vLocation;  // In worlds's coordinate system
  BVector m_vector;
  double  m_dFriction;
  double  m_dMass;

  BBodyPoint();
};

class BString {
  BBodyPoint *m_pPoints;
public:
  int     m_nP1;
  int     m_nP2;
  double  m_dIdealLength;
  double  m_dRigidity;
  double  m_dFactor1;
  double  m_dFactor2;

  BString();

  inline double Length() {return (m_pPoints[m_nP1].m_vLocation - m_pPoints[m_nP2].m_vLocation).Length();};
  void          SetPoints(BBodyPoint *pPoints) {m_pPoints = pPoints;};
};

class BTrailPoint {
public:
  BVector m_vLocation[2];
  bool    m_bStart;
  bool    m_bEnd;
  double  m_dStrength;
  BTrailPoint() {m_bEnd = m_bStart = false; m_dStrength = 0.0;}
};

class BWheel {
public:
  double       m_dTTT;      // Tendency To Turn (factor based on wheel orientation)
  BBodyPoint  *m_pPoints;
  BVector      m_vLocationOrig; // In "car's coordinate system"
  BVector      m_vLocation; // In worlds's coordinate system
  BVector      m_vLocSample; // In worlds's coordinate system
  BOrientation m_orientation;
  double       m_dDepth;
  BVector      m_vGroundNormal;
  double       m_dGroundFriction;
  double       m_dBaseDepth;
  double       m_dRadius;
  double       m_dDistFromSuspPoint;
  double       m_dSuspension;
  int          m_nSuspPoint;
  int          m_nBasePoint1;
  int          m_nBasePoint2;
  double       m_dFriction;
  bool         m_bRear;
  bool         m_bLeft;
  bool         m_bBroken;
  bool         m_bInGround;
  double       m_dAngle;
  double       m_dAngleStep;

  int          m_nGroundHits;
  int          m_nTrailPoints;
  int          m_nTrailHead;
  BTrailPoint  m_pTrailPoint[g_cnMaxTrailPoints];

  BWheel() {m_dAngle = m_dAngleStep = 0.0; 
            m_bInGround = false; 
            m_nTrailPoints = 0; 
            m_nTrailHead = -1; 
            m_nGroundHits = 0; 
            m_pTrailPoint[0].m_bStart = true;}
  void SetPoints(BBodyPoint *pPoints) {m_pPoints = pPoints;};
};


class B2DPoint {
public:
  double m_dX;
  double m_dY;
};

class BBoundaryPoint : public B2DPoint {
public:
  BBoundaryPoint *m_pNext;
  BBoundaryPoint() {m_pNext = 0;}
};

class BBoundary {
  BBoundaryPoint *m_pHead;
  BBoundaryPoint *AlreadyInserted(B2DPoint& rP);
public:
  BBoundary() {m_pHead = 0;}
  void            Empty();
  bool            PointIsLeftOfLine(BBoundaryPoint *pStart, double dX, double dY);
  BBoundaryPoint *Head() {return m_pHead;}
  BBoundaryPoint *AddAfter(BBoundaryPoint *pAfter, B2DPoint& rNew2D);
  BBoundaryPoint *PointIsOutside(B2DPoint& rP);
  BBoundaryPoint *PointIsOutside(double dX, double dY);
  BBoundaryPoint *Delete(BBoundaryPoint *p);
};

/*******************************************************************************
* This is just a quick and dirty class for using wglUseFontBitmaps. I don't like
* using it because it's slow, but for now that's all I have to use for fonts.
* It would be nice to test drawing to a normal Windows bitmap using GDI commands,
* then blitting the text to OpenGL somehow.
*******************************************************************************/
class GLFont
{
protected:
	int m_nListBase;
	float m_fXPos;
	float m_fYPos;

public:
	//GLFont(HDC hDC=NULL)
	GLFont()
	{
		m_nListBase = -1;
		m_fXPos = 0;
		m_fYPos = 0;
		//if(hDC)
		//{
		//	m_nListBase = glGenLists(256);
		//	wglUseFontBitmaps(hDC, 0, 255, m_nListBase);
		//}
	}
	~GLFont()	{ Cleanup(); }
	//void Init(HDC hDC)
	void Init()
	{
		Cleanup();
		//m_nListBase = glGenLists(256);
		//wglUseFontBitmaps(hDC, 0, 255, m_nListBase);
	}
	void Cleanup()
	{
		if(m_nListBase != -1)
		{
			glDeleteLists(m_nListBase, 256);
			m_nListBase = -1;
		}
	}
	void SetPosition(int x, int y)
	{
		m_fXPos = (float)x;
		m_fYPos = (float)y;
	}
	void Begin(int nWidth, int nHeight)
	{
		glDisable(GL_LIGHTING);
		glDisable(GL_DEPTH_TEST);
		glPushMatrix();
		glLoadIdentity();
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluOrtho2D(0, nWidth, 0, nHeight);
	}
	void Print(const char *pszMessage)
	{
		glRasterPos2f(m_fXPos, m_fYPos);
		glListBase(m_nListBase);
		glCallLists(strlen(pszMessage), GL_UNSIGNED_BYTE, pszMessage);
	}
	void End()
	{
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
		glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
	}
};


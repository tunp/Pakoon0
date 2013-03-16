//
// Car
//

#include "BCar.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <cstdio>
#include "OpenGLHelpers.h"
//#include "resource.h"

#include <iostream>

using namespace std;

const double g_cdPI = 3.141592654;
extern bool g_cbBlackAndWhite;

BCar::BCar() {
  m_nPoints    = 0;
  m_pPoint     = 0;
  m_pPointVis  = 0;
  m_pPointOrig = 0;
  m_nStrings   = 0;
  m_pString    = 0;
  m_nWheels    = 0;
  m_pWheel     = 0;
  m_dRPM       = 0;
  InitAll();
  LoadCarFromFile("./World/CarJetHeli.gaw");
  // LoadTextures();
  m_vHomeLocation.Set(100, 60, -3.0);
  Move(m_vHomeLocation);
}

void BCar::InitAll() {
  m_dSpeed     = 0.0;
  m_bBreaking     = false;
  m_bABS          = false;
  m_bHandBreaking = false;
  m_bAccelerating = false;
  m_bReversing    = false;
  m_bTurningLeft  = false;
  m_bTurningRight = false;
  m_dTurn         = 0.0;
  m_dSpeedKmh = 0.0;
  m_bWireframe = false;
  m_nHeliMode = 0;
  m_bHeliModeActivating = false;
  m_bHeliLifting = false;
  m_bHeliForwarding = false;
  m_bHeliRighting = false;
  m_bHeliBacking = false;
  m_bHeliLefting = false;
  m_dHeliLift = 0.0;
  m_dHeliForward = 0.0;
  m_dHeliRight = 0.0;
  m_nJetMode = 0;
  m_bJetModeActivating = false;
  m_dAccelerationFactor = 1.0;
  m_dReversingFactor = 1.0;
  m_dHeliMode = 0.0;
  m_dJetMode = 0.0;

  m_bHeliBladeOK[0] = m_bHeliBladeOK[1] = m_bHeliBladeOK[2] = true;
  m_bHeliCoverOK = true;
  m_bHeliHatchesOK = true;
  m_dHeliLeftHatchOffsetAngle = 0.0;
  m_dHeliRightHatchOffsetAngle = 0.0;
  m_dHeliBladePower = 1.0;

  m_dSteeringAid = 1.0;
}

void BCar::DeleteAll() {
  if(m_pPoint) {
    delete [] m_pPoint;
    delete [] m_pPointVis;
    delete [] m_pPointOrig;
  }
  if(m_pString) {
    delete [] m_pString;
  }
  if(m_pWheel) {
    delete [] m_pWheel;
  }
}

BCar::~BCar() {
  DeleteAll();
}


void BCar::LoadTextures() {
  FILE *fp = fopen("./Textures/cartexture.raw", "rb");
  if(fp) {    
    for(int y = 0; y < 256; ++y) {
      for(int x = 0; x < 256; ++x) {
        unsigned char cTmp;
        fscanf(fp, "%c", &cTmp);
        m_dTXTRCar[y * 256 * 3 + x * 3] = cTmp;
        fscanf(fp, "%c", &cTmp);
        m_dTXTRCar[y * 256 * 3 + x * 3 + 1] = cTmp;
        fscanf(fp, "%c", &cTmp);
        m_dTXTRCar[y * 256 * 3 + x * 3 + 2] = cTmp;

        if(g_cbBlackAndWhite) {
          double r, g, b, bw;
          r = m_dTXTRCar[y * 256 * 3 + x * 3];
          g = m_dTXTRCar[y * 256 * 3 + x * 3 + 1];
          b = m_dTXTRCar[y * 256 * 3 + x * 3 + 2];
          bw = r*0.37f+g*0.42f+b*0.2f;
          m_dTXTRCar[y * 256 * 3 + x * 3] = GLubyte(bw);
          m_dTXTRCar[y * 256 * 3 + x * 3 + 1] = GLubyte(bw);
          m_dTXTRCar[y * 256 * 3 + x * 3 + 2] = GLubyte(bw);
        }
      }
    }
    fclose(fp);
  } else {
	  cout << "Cannot open cartexture.raw!" << endl;
    //AfxMessageBox("Cannot open cartexture.raw!");
  }

  OpenGLHelpers::CreateMipMaps(m_dTXTRCar, 256, 256, 3);

  fp = fopen("./Textures/wheeldetailed.raw", "rb");
  if(fp) {    
    for(int y = 0; y < 128; ++y) {
      for(int x = 0; x < 128; ++x) {
        unsigned char cTmp;
        fscanf(fp, "%c", &cTmp);
        m_dTXTRWheel[y * 128 * 3 + x * 3] = cTmp;
        fscanf(fp, "%c", &cTmp);
        m_dTXTRWheel[y * 128 * 3 + x * 3 + 1] = cTmp;
        fscanf(fp, "%c", &cTmp);
        m_dTXTRWheel[y * 128 * 3 + x * 3 + 2] = cTmp;

        if(g_cbBlackAndWhite) {
          double r, g, b, bw;
          r = m_dTXTRWheel[y * 128 * 3 + x * 3];
          g = m_dTXTRWheel[y * 128 * 3 + x * 3 + 1];
          b = m_dTXTRWheel[y * 128 * 3 + x * 3 + 2];
          bw = r*0.37f+g*0.42f+b*0.2f;
          m_dTXTRWheel[y * 128 * 3 + x * 3] = GLubyte(bw);
          m_dTXTRWheel[y * 128 * 3 + x * 3 + 1] = GLubyte(bw);
          m_dTXTRWheel[y * 128 * 3 + x * 3 + 2] = GLubyte(bw);
        }
      }
    }
    fclose(fp);
  } else {
	  cout << "Cannot open wheeldetailed.raw!" << endl;
    //AfxMessageBox("Cannot open wheeldetailed.raw!");
  }

  OpenGLHelpers::CreateMipMaps(m_dTXTRWheel, 128, 128, 3);

}


void BCar::LoadCarFromFile(std::string sFilename) {
  DeleteAll();
  InitAll();
  // Load points and strings from a file
  FILE *fp;
  fp = fopen(sFilename.c_str(), "r");
  if(fp) {
    char sLine[1024];
    int  i;

    // Read points
    fgets(sLine, 1023, fp);
    sscanf(sLine, "%d", &m_nPoints);
    m_pPoint = new BBodyPoint[m_nPoints];
    m_pPointVis = new BVector[m_nPoints];
    m_pPointOrig = new BVector[m_nPoints];
    for(i = 0; i < m_nPoints; ++i) {
      // Read in point
      fgets(sLine, 1023, fp);
      sscanf(sLine, 
             "%lf %lf %lf %lf %lf", 
             &(m_pPoint[i].m_vLocation.m_dX),
             &(m_pPoint[i].m_vLocation.m_dY),
             &(m_pPoint[i].m_vLocation.m_dZ),
             &(m_pPoint[i].m_dMass),
             &(m_pPoint[i].m_dFriction));

      m_pPointOrig[i] = m_pPoint[i].m_vLocation;
    }
    fclose(fp);

    // Prepare orig points for visualization
    BVector vMassCenter = (m_pPointOrig[0] + 
                           m_pPointOrig[9] + 
                           m_pPointOrig[7] + 
                           m_pPointOrig[16] +
                           m_pPointOrig[1] + 
                           m_pPointOrig[10] + 
                           m_pPointOrig[6] + 
                           m_pPointOrig[15]) * (1.0 / 8.0);
    for(i = 0; i < m_nPoints; ++i) {
      m_pPointOrig[i] += vMassCenter * -1.0;
    }
    m_vMassCenter = vMassCenter;

    // Create strings
    int j;
    m_nStrings = 0;
    for(i = 0; i < m_nPoints - 1; ++i) {
      for(j = i + 1;  j < m_nPoints; ++j) {
        ++m_nStrings;
      }
    }
    int s = 0;
    m_pString = new BString[m_nStrings];
    for(i = 0; i < m_nPoints - 1; ++i) {
      for(j = i + 1;  j < m_nPoints; ++j) {
        m_pString[s].m_nP1 = i;
        m_pString[s].m_nP2 = j;
        m_pString[s].m_dRigidity = 0.4;
        m_pString[s].m_dIdealLength;
        m_pString[s].m_dIdealLength = (m_pPoint[m_pString[s].m_nP1].m_vLocation - 
                                       m_pPoint[m_pString[s].m_nP2].m_vLocation).Length();
        m_pString[s].m_dFactor1 = (m_pPoint[m_pString[s].m_nP2].m_dMass / (m_pPoint[m_pString[s].m_nP1].m_dMass + m_pPoint[m_pString[i].m_nP2].m_dMass)) * m_pString[s].m_dRigidity;
        m_pString[s].m_dFactor2 = (m_pPoint[m_pString[s].m_nP1].m_dMass / (m_pPoint[m_pString[s].m_nP1].m_dMass + m_pPoint[m_pString[i].m_nP2].m_dMass)) * m_pString[s].m_dRigidity;
        m_pString[s].SetPoints(m_pPoint);
        ++s;
      }
    }

    // Create 4 wheels
    m_nWheels = 4;
    m_pWheel = new BWheel[4];

    m_pWheel[0].m_bRear = false;
    m_pWheel[0].m_bLeft = true;
    m_pWheel[0].m_bBroken = false;
    m_pWheel[0].m_dFriction = 0.9;
    m_pWheel[0].m_nSuspPoint = 2;
    m_pWheel[0].m_dRadius = 0.3;
    m_pWheel[0].m_dDistFromSuspPoint = 0.95;
    m_pWheel[0].m_dSuspension = 0.0;
    m_pWheel[0].m_nBasePoint1 = 0;
    m_pWheel[0].m_nBasePoint2 = 8;
    m_pWheel[0].SetPoints(m_pPoint);
    m_pWheel[0].m_vLocationOrig = m_pPointOrig[m_pWheel[0].m_nSuspPoint] + 
                                  BVector(0, 0, 1) * 
                                  m_pWheel[0].m_dDistFromSuspPoint;

    m_pWheel[1].m_bRear = false;
    m_pWheel[1].m_bLeft = false;
    m_pWheel[1].m_bBroken = false;
    m_pWheel[1].m_dFriction = 0.9;
    m_pWheel[1].m_nSuspPoint = 11;
    m_pWheel[1].m_dRadius = 0.3;
    m_pWheel[1].m_dDistFromSuspPoint = 0.95;
    m_pWheel[1].m_dSuspension = 0.0;
    m_pWheel[1].m_nBasePoint1 = 9;
    m_pWheel[1].m_nBasePoint2 = 17;
    m_pWheel[1].SetPoints(m_pPoint);
    m_pWheel[1].m_vLocationOrig = m_pPointOrig[m_pWheel[1].m_nSuspPoint] + 
                                  BVector(0, 0, 1) * 
                                  m_pWheel[1].m_dDistFromSuspPoint;

    m_pWheel[2].m_bRear = true;
    m_pWheel[2].m_bLeft = true;
    m_pWheel[2].m_bBroken = false;
    m_pWheel[2].m_dFriction = 0.9;
    m_pWheel[2].m_nSuspPoint = 5;
    m_pWheel[2].m_dRadius = 0.3;
    m_pWheel[2].m_dDistFromSuspPoint = 1.35;
    m_pWheel[2].m_dSuspension = 0.0;
    m_pWheel[2].m_nBasePoint1 = 7;
    m_pWheel[2].m_nBasePoint2 = 8;
    m_pWheel[2].SetPoints(m_pPoint);
    m_pWheel[2].m_vLocationOrig = m_pPointOrig[m_pWheel[2].m_nSuspPoint] + 
                                  BVector(0, 0, 1) * 
                                  m_pWheel[2].m_dDistFromSuspPoint;

    m_pWheel[3].m_bRear = true;
    m_pWheel[3].m_bLeft = false;
    m_pWheel[3].m_bBroken = false;
    m_pWheel[3].m_dFriction = 0.9;
    m_pWheel[3].m_nSuspPoint = 14;
    m_pWheel[3].m_dRadius = 0.3;
    m_pWheel[3].m_dDistFromSuspPoint = 1.35;
    m_pWheel[3].m_dSuspension = 0.0;
    m_pWheel[3].m_nBasePoint1 = 16;
    m_pWheel[3].m_nBasePoint2 = 17;
    m_pWheel[3].SetPoints(m_pPoint);
    m_pWheel[3].m_vLocationOrig = m_pPointOrig[m_pWheel[3].m_nSuspPoint] + 
                                  BVector(0, 0, 1) * 
                                  m_pWheel[3].m_dDistFromSuspPoint;
  } else {
	  cout << "Cannot open file!" << endl;
    //AfxMessageBox("Cannot open file!");
  }
}


void BCar::Move(BVector vRelMove, bool bAutoHeight) {
  for(int i = 0; i < m_nPoints; ++i) {
    m_pPoint[i].m_vLocation = m_pPoint[i].m_vLocation + vRelMove;
  }
}


void BCar::PreProcessVisualization() {
  int nTmp;
  BVector vTmp[44];
  // Create a display list of the car

  OpenGLHelpers::DefineMipMapTextures(256, 256, 3, GL_RGB, m_dTXTRCar, 0);
  glNewList(DISPLIST_CARTXTR, GL_COMPILE);
  glShadeModel(GL_FLAT);
  glEnable(GL_TEXTURE_2D);
  OpenGLHelpers::SetColorFull(1, 1, 1, 1);
  OpenGLHelpers::DefineMipMapTextures(256, 256, 3, GL_RGB, m_dTXTRCar, 0);
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 128.0f);
  glEndList();

  glNewList(DISPLIST_CARBODY, GL_COMPILE);

  //**********************
  // BOTTOM
  glShadeModel(GL_FLAT);
  OpenGLHelpers::SetColorFull(0.15, 0.15, 0.15, 0);
  GLfloat fLight[4];
  fLight[0] = 0.0f;
  fLight[1] = 0.0f;
  fLight[2] = 0.0f;
  fLight[3] = 0.0f;
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, fLight);
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0f);

  vTmp[0]  = m_pPointOrig[0]; // Bottom
  vTmp[1]  = m_pPointOrig[0] + BVector(0.3, 0, 0);
  vTmp[2]  = BVector(-1.0, 1.65, 0.3) - m_vMassCenter;
  vTmp[3]  = BVector(-0.7, 1.65, 0.3) - m_vMassCenter;
  vTmp[4]  = BVector(-1.0, 1.65, -0.05) - m_vMassCenter;
  vTmp[5]  = BVector(-0.7, 1.65, -0.05) - m_vMassCenter;
  vTmp[6]  = BVector(-1.0, 0.95, -0.05) - m_vMassCenter;
  vTmp[7]  = BVector(-0.7, 0.95, -0.05) - m_vMassCenter;
  vTmp[8]  = BVector(-1.0, 0.95, 0.3) - m_vMassCenter;
  vTmp[9]  = BVector(-0.7, 0.95, 0.3) - m_vMassCenter;
  vTmp[10] = BVector(-1.0, -0.85, 0.3) - m_vMassCenter;
  vTmp[11] = BVector(-0.7, -0.85, 0.3) - m_vMassCenter;
  vTmp[12] = BVector(-1.0, -0.85, -0.05) - m_vMassCenter;
  vTmp[13] = BVector(-0.7, -0.85, -0.05) - m_vMassCenter;
  vTmp[14] = BVector(-1.0, -1.55, -0.05) - m_vMassCenter;
  vTmp[15] = BVector(-0.7, -1.55, -0.05) - m_vMassCenter;
  vTmp[16] = BVector(-1.0, -1.55, 0.3) - m_vMassCenter;
  vTmp[17] = BVector(-0.7, -1.55, 0.3) - m_vMassCenter;
  vTmp[18] = m_pPointOrig[7];
  vTmp[19] = m_pPointOrig[7] + BVector(0.3, 0, 0);
  
  glBegin(GL_TRIANGLE_STRIP);
  for(nTmp = 0; nTmp < 20; ++nTmp) {
    //glTexCoord2d(vTmp[nTmp].m_dY, vTmp[nTmp].m_dZ);
    if((nTmp < 18) && ((nTmp & 2) == 0)) {
      BVector vNormal;
      vNormal.Set(0, -(vTmp[nTmp].m_dZ - vTmp[nTmp+2].m_dZ), vTmp[nTmp].m_dY - vTmp[nTmp+2].m_dY);
      vNormal.ToUnitLength();
      glNormal3d(vNormal.m_dX, vNormal.m_dY, vNormal.m_dZ);
    }
    glVertex3d(vTmp[nTmp].m_dX, vTmp[nTmp].m_dY, vTmp[nTmp].m_dZ);
  }
  glEnd();
  
  vTmp[0]  = BVector(-0.7, 1.65, 0.3) - m_vMassCenter;
  vTmp[1]  = BVector(-0.7, 1.65, -0.05) - m_vMassCenter;
  vTmp[2]  = BVector(-0.7, 0.95, 0.3) - m_vMassCenter;
  vTmp[3]  = BVector(-0.7, 0.95, -0.05) - m_vMassCenter;
  glBegin(GL_TRIANGLE_STRIP);
  for(nTmp = 0; nTmp < 4; ++nTmp) {
    //glTexCoord2d(vTmp[nTmp].m_dY, vTmp[nTmp].m_dZ);
    if((nTmp < 18) && ((nTmp & 2) == 0)) {
      BVector vNormal;
      vNormal.Set(0, -(vTmp[nTmp].m_dZ - vTmp[nTmp+2].m_dZ), vTmp[nTmp].m_dY - vTmp[nTmp+2].m_dY);
      vNormal.ToUnitLength();
      glNormal3d(vNormal.m_dX, vNormal.m_dY, vNormal.m_dZ);
    }
    glVertex3d(vTmp[nTmp].m_dX, vTmp[nTmp].m_dY, vTmp[nTmp].m_dZ);
  }
  glEnd();

  vTmp[0]  = BVector(0.7, 1.65, 0.3) - m_vMassCenter;
  vTmp[1]  = BVector(0.7, 1.65, -0.05) - m_vMassCenter;
  vTmp[2]  = BVector(0.7, 0.95, 0.3) - m_vMassCenter;
  vTmp[3]  = BVector(0.7, 0.95, -0.05) - m_vMassCenter;
  glBegin(GL_TRIANGLE_STRIP);
  for(nTmp = 0; nTmp < 4; ++nTmp) {
    //glTexCoord2d(vTmp[nTmp].m_dY, vTmp[nTmp].m_dZ);
    if((nTmp < 18) && ((nTmp & 2) == 0)) {
      BVector vNormal;
      vNormal.Set(0, -(vTmp[nTmp].m_dZ - vTmp[nTmp+2].m_dZ), vTmp[nTmp].m_dY - vTmp[nTmp+2].m_dY);
      vNormal.ToUnitLength();
      glNormal3d(vNormal.m_dX, vNormal.m_dY, vNormal.m_dZ);
    }
    glVertex3d(vTmp[nTmp].m_dX, vTmp[nTmp].m_dY, vTmp[nTmp].m_dZ);
  }
  glEnd();

  vTmp[0] = BVector(-0.7, -0.85, 0.3) - m_vMassCenter;
  vTmp[1] = BVector(-0.7, -0.85, -0.05) - m_vMassCenter;
  vTmp[2] = BVector(-0.7, -1.55, 0.3) - m_vMassCenter;
  vTmp[3] = BVector(-0.7, -1.55, -0.05) - m_vMassCenter;
  glBegin(GL_TRIANGLE_STRIP);
  for(nTmp = 0; nTmp < 4; ++nTmp) {
    //glTexCoord2d(vTmp[nTmp].m_dY, vTmp[nTmp].m_dZ);
    if((nTmp < 18) && ((nTmp & 2) == 0)) {
      BVector vNormal;
      vNormal.Set(0, -(vTmp[nTmp].m_dZ - vTmp[nTmp+2].m_dZ), vTmp[nTmp].m_dY - vTmp[nTmp+2].m_dY);
      vNormal.ToUnitLength();
      glNormal3d(vNormal.m_dX, vNormal.m_dY, vNormal.m_dZ);
    }
    glVertex3d(vTmp[nTmp].m_dX, vTmp[nTmp].m_dY, vTmp[nTmp].m_dZ);
  }
  glEnd();

  vTmp[0] = BVector(0.7, -0.85, 0.3) - m_vMassCenter;
  vTmp[1] = BVector(0.7, -0.85, -0.05) - m_vMassCenter;
  vTmp[2] = BVector(0.7, -1.55, 0.3) - m_vMassCenter;
  vTmp[3] = BVector(0.7, -1.55, -0.05) - m_vMassCenter;
  glBegin(GL_TRIANGLE_STRIP);
  for(nTmp = 0; nTmp < 4; ++nTmp) {
    //glTexCoord2d(vTmp[nTmp].m_dY, vTmp[nTmp].m_dZ);
    if((nTmp < 18) && ((nTmp & 2) == 0)) {
      BVector vNormal;
      vNormal.Set(0, -(vTmp[nTmp].m_dZ - vTmp[nTmp+2].m_dZ), vTmp[nTmp].m_dY - vTmp[nTmp+2].m_dY);
      vNormal.ToUnitLength();
      glNormal3d(vNormal.m_dX, vNormal.m_dY, vNormal.m_dZ);
    }
    glVertex3d(vTmp[nTmp].m_dX, vTmp[nTmp].m_dY, vTmp[nTmp].m_dZ);
  }
  glEnd();

  vTmp[0]  = m_pPointOrig[9];
  vTmp[1]  = m_pPointOrig[9] - BVector(0.3, 0, 0);
  vTmp[2]  = BVector( 1.0, 1.65, 0.3) - m_vMassCenter;
  vTmp[3]  = BVector( 0.7, 1.65, 0.3) - m_vMassCenter;
  vTmp[4]  = BVector( 1.0, 1.65, -0.05) - m_vMassCenter;
  vTmp[5]  = BVector( 0.7, 1.65, -0.05) - m_vMassCenter;
  vTmp[6]  = BVector( 1.0, 0.95, -0.05) - m_vMassCenter;
  vTmp[7]  = BVector( 0.7, 0.95, -0.05) - m_vMassCenter;
  vTmp[8]  = BVector( 1.0, 0.95, 0.3) - m_vMassCenter;
  vTmp[9]  = BVector( 0.7, 0.95, 0.3) - m_vMassCenter;
  vTmp[10] = BVector( 1.0, -0.85, 0.3) - m_vMassCenter;
  vTmp[11] = BVector( 0.7, -0.85, 0.3) - m_vMassCenter;
  vTmp[12] = BVector( 1.0, -0.85, -0.05) - m_vMassCenter;
  vTmp[13] = BVector( 0.7, -0.85, -0.05) - m_vMassCenter;
  vTmp[14] = BVector( 1.0, -1.55, -0.05) - m_vMassCenter;
  vTmp[15] = BVector( 0.7, -1.55, -0.05) - m_vMassCenter;
  vTmp[16] = BVector( 1.0, -1.55, 0.3) - m_vMassCenter;
  vTmp[17] = BVector( 0.7, -1.55, 0.3) - m_vMassCenter;
  vTmp[18] = m_pPointOrig[16];
  vTmp[19] = m_pPointOrig[16] - BVector(0.3, 0, 0);
  glBegin(GL_TRIANGLE_STRIP);
  for(nTmp = 0; nTmp < 20; ++nTmp) {
    //glTexCoord2d(vTmp[nTmp].m_dY, vTmp[nTmp].m_dZ);
    if((nTmp < 18) && ((nTmp & 2) == 0)) {
      BVector vNormal;
      vNormal.Set(0, -(vTmp[nTmp].m_dZ - vTmp[nTmp+2].m_dZ), vTmp[nTmp].m_dY - vTmp[nTmp+2].m_dY);
      vNormal.ToUnitLength();
      glNormal3d(vNormal.m_dX, vNormal.m_dY, vNormal.m_dZ);
    }
    glVertex3d(vTmp[nTmp].m_dX, vTmp[nTmp].m_dY, vTmp[nTmp].m_dZ);
  }
  glEnd();

  vTmp[0]  = m_pPointOrig[0] + BVector(0.3, 0, 0); // Bottom
  vTmp[1]  = m_pPointOrig[9] - BVector(0.3, 0, 0);
  vTmp[2] = m_pPointOrig[7] + BVector(0.3, 0, 0);
  vTmp[3] = m_pPointOrig[16] - BVector(0.3, 0, 0);
  glBegin(GL_TRIANGLE_STRIP);
  for(nTmp = 0; nTmp < 4; ++nTmp) {
    //glTexCoord2d(vTmp[nTmp].m_dY, vTmp[nTmp].m_dZ);
    if((nTmp < 18) && ((nTmp & 2) == 0)) {
      BVector vNormal;
      vNormal.Set(0, -(vTmp[nTmp].m_dZ - vTmp[nTmp+2].m_dZ), vTmp[nTmp].m_dY - vTmp[nTmp+2].m_dY);
      vNormal.ToUnitLength();
      glNormal3d(vNormal.m_dX, vNormal.m_dY, vNormal.m_dZ);
    }
    glVertex3d(vTmp[nTmp].m_dX, vTmp[nTmp].m_dY, vTmp[nTmp].m_dZ);
  }
  glEnd();



  glCallList(DISPLIST_CARTXTR);

  glNormal3d(-1, 0, 0);
  vTmp[0]  = m_pPointOrig[0]; // Left side
  vTmp[1]  = m_pPointOrig[1];
  vTmp[2]  = BVector(-1, 1.60, 0.1) - m_vMassCenter;
  vTmp[3]  = BVector(-1, 1.65, 0.3) - m_vMassCenter;
  glBegin(GL_POLYGON);
  for(nTmp = 0; nTmp < 4; ++nTmp) {
    glTexCoord2d(vTmp[nTmp].m_dY/2.0, vTmp[nTmp].m_dZ/2.0);
    glVertex3d(vTmp[nTmp].m_dX, vTmp[nTmp].m_dY, vTmp[nTmp].m_dZ);
  }
  glEnd();

  vTmp[0]  = m_pPointOrig[1];
  vTmp[1]  = m_pPointOrig[2];
  vTmp[2]  = BVector(-1, 1.45, -0.05) - m_vMassCenter;
  vTmp[3]  = BVector(-1, 1.60, 0.1) - m_vMassCenter;
  glBegin(GL_POLYGON);
  for(nTmp = 0; nTmp < 4; ++nTmp) {
    glTexCoord2d(vTmp[nTmp].m_dY/2.0, vTmp[nTmp].m_dZ/2.0);
    glVertex3d(vTmp[nTmp].m_dX, vTmp[nTmp].m_dY, vTmp[nTmp].m_dZ);
  }
  glEnd();

  vTmp[0]  = BVector(-1, 1.45, -0.05) - m_vMassCenter;
  vTmp[1]  = m_pPointOrig[2];
  vTmp[2]  = m_pPointOrig[3];
  vTmp[3]  = BVector(-1, 1.15, -0.05) - m_vMassCenter;
  glBegin(GL_POLYGON);
  for(nTmp = 0; nTmp < 4; ++nTmp) {
    glTexCoord2d(vTmp[nTmp].m_dY/2.0, vTmp[nTmp].m_dZ/2.0);
    glVertex3d(vTmp[nTmp].m_dX, vTmp[nTmp].m_dY, vTmp[nTmp].m_dZ);
  }
  glEnd();

  vTmp[0]  = BVector(-1, 1.15, -0.05) - m_vMassCenter;
  vTmp[1]  = m_pPointOrig[3];
  vTmp[2]  = m_pPointOrig[4];
  vTmp[3]  = BVector(-1, 1.00, 0.1) - m_vMassCenter;
  glBegin(GL_POLYGON);
  for(nTmp = 0; nTmp < 4; ++nTmp) {
    glTexCoord2d(vTmp[nTmp].m_dY/2.0, vTmp[nTmp].m_dZ/2.0);
    glVertex3d(vTmp[nTmp].m_dX, vTmp[nTmp].m_dY, vTmp[nTmp].m_dZ);
  }
  glEnd();

  vTmp[0] = BVector(-1, 0.95, 0.3) - m_vMassCenter;
  vTmp[1]  = BVector(-1, 1.00, 0.1) - m_vMassCenter;
  vTmp[2]  = m_pPointOrig[4];
  vTmp[3] = BVector(-1, -0.90, 0.1) - m_vMassCenter;
  vTmp[4] = BVector(-1, -0.85, 0.3) - m_vMassCenter;
  glBegin(GL_POLYGON);
  for(nTmp = 0; nTmp < 5; ++nTmp) {
    glTexCoord2d(vTmp[nTmp].m_dY/2.0, vTmp[nTmp].m_dZ/2.0);
    glVertex3d(vTmp[nTmp].m_dX, vTmp[nTmp].m_dY, vTmp[nTmp].m_dZ);
  }
  glEnd();

  vTmp[0] = BVector(-1, -0.90, 0.1) - m_vMassCenter;
  vTmp[1]  = m_pPointOrig[4];
  vTmp[2]  = m_pPointOrig[5];
  vTmp[3]  = BVector(-1, -1.05, -0.05) - m_vMassCenter;
  glBegin(GL_POLYGON);
  for(nTmp = 0; nTmp < 4; ++nTmp) {
    glTexCoord2d(vTmp[nTmp].m_dY/2.0, vTmp[nTmp].m_dZ/2.0);
    glVertex3d(vTmp[nTmp].m_dX, vTmp[nTmp].m_dY, vTmp[nTmp].m_dZ);
  }
  glEnd();

  vTmp[0]  = BVector(-1, -1.05, -0.05) - m_vMassCenter;
  vTmp[1]  = m_pPointOrig[5];
  vTmp[2]  = m_pPointOrig[6];
  vTmp[3]  = BVector(-1, -1.35, -0.05) - m_vMassCenter;
  glBegin(GL_POLYGON);
  for(nTmp = 0; nTmp < 4; ++nTmp) {
    glTexCoord2d(vTmp[nTmp].m_dY/2.0, vTmp[nTmp].m_dZ/2.0);
    glVertex3d(vTmp[nTmp].m_dX, vTmp[nTmp].m_dY, vTmp[nTmp].m_dZ);
  }
  glEnd();

  vTmp[0]  = BVector(-1, -1.35, -0.05) - m_vMassCenter;
  vTmp[1]  = m_pPointOrig[6];
  vTmp[2]  = BVector(-1, -1.50, 0.1) - m_vMassCenter;
  glBegin(GL_TRIANGLES);
  for(nTmp = 0; nTmp < 3; ++nTmp) {
    glTexCoord2d(vTmp[nTmp].m_dY/2.0, vTmp[nTmp].m_dZ/2.0);
    glVertex3d(vTmp[nTmp].m_dX, vTmp[nTmp].m_dY, vTmp[nTmp].m_dZ);
  }
  glEnd();

  vTmp[0]  = BVector(-1, -1.50, 0.1) - m_vMassCenter;
  vTmp[1]  = m_pPointOrig[6];
  vTmp[2]  = m_pPointOrig[7];
  vTmp[3]  = BVector(-1, -1.55, 0.3) - m_vMassCenter;
  glBegin(GL_POLYGON);
  for(nTmp = 0; nTmp < 4; ++nTmp) {
    glTexCoord2d(vTmp[nTmp].m_dY/2.0, vTmp[nTmp].m_dZ/2.0);
    glVertex3d(vTmp[nTmp].m_dX, vTmp[nTmp].m_dY, vTmp[nTmp].m_dZ);
  }
  glEnd();

  //**********************

  glNormal3d(-1, 0, 0);
  vTmp[0]  = m_pPointOrig[0+9]; // Right side
  vTmp[1]  = m_pPointOrig[1+9];
  vTmp[2]  = BVector(1, 1.60, 0.1) - m_vMassCenter;
  vTmp[3]  = BVector(1, 1.65, 0.3) - m_vMassCenter;
  glBegin(GL_POLYGON);
  for(nTmp = 0; nTmp < 4; ++nTmp) {
    glTexCoord2d(vTmp[nTmp].m_dY/2.0, vTmp[nTmp].m_dZ/2.0);
    glVertex3d(vTmp[nTmp].m_dX, vTmp[nTmp].m_dY, vTmp[nTmp].m_dZ);
  }
  glEnd();

  vTmp[0]  = m_pPointOrig[1+9];
  vTmp[1]  = m_pPointOrig[2+9];
  vTmp[2]  = BVector(1, 1.45, -0.05) - m_vMassCenter;
  vTmp[3]  = BVector(1, 1.60, 0.1) - m_vMassCenter;
  glBegin(GL_POLYGON);
  for(nTmp = 0; nTmp < 4; ++nTmp) {
    glTexCoord2d(vTmp[nTmp].m_dY/2.0, vTmp[nTmp].m_dZ/2.0);
    glVertex3d(vTmp[nTmp].m_dX, vTmp[nTmp].m_dY, vTmp[nTmp].m_dZ);
  }
  glEnd();

  vTmp[0]  = BVector(1, 1.45, -0.05) - m_vMassCenter;
  vTmp[1]  = m_pPointOrig[2+9];
  vTmp[2]  = m_pPointOrig[3+9];
  vTmp[3]  = BVector(1, 1.15, -0.05) - m_vMassCenter;
  glBegin(GL_POLYGON);
  for(nTmp = 0; nTmp < 4; ++nTmp) {
    glTexCoord2d(vTmp[nTmp].m_dY/2.0, vTmp[nTmp].m_dZ/2.0);
    glVertex3d(vTmp[nTmp].m_dX, vTmp[nTmp].m_dY, vTmp[nTmp].m_dZ);
  }
  glEnd();

  vTmp[0]  = BVector(1, 1.15, -0.05) - m_vMassCenter;
  vTmp[1]  = m_pPointOrig[3+9];
  vTmp[2]  = m_pPointOrig[4+9];
  vTmp[3]  = BVector(1, 1.00, 0.1) - m_vMassCenter;
  glBegin(GL_POLYGON);
  for(nTmp = 0; nTmp < 4; ++nTmp) {
    glTexCoord2d(vTmp[nTmp].m_dY/2.0, vTmp[nTmp].m_dZ/2.0);
    glVertex3d(vTmp[nTmp].m_dX, vTmp[nTmp].m_dY, vTmp[nTmp].m_dZ);
  }
  glEnd();

  vTmp[0] = BVector(1, 0.95, 0.3) - m_vMassCenter;
  vTmp[1]  = BVector(1, 1.00, 0.1) - m_vMassCenter;
  vTmp[2]  = m_pPointOrig[4+9];
  vTmp[3] = BVector(1, -0.90, 0.1) - m_vMassCenter;
  vTmp[4] = BVector(1, -0.85, 0.3) - m_vMassCenter;
  glBegin(GL_POLYGON);
  for(nTmp = 0; nTmp < 5; ++nTmp) {
    glTexCoord2d(vTmp[nTmp].m_dY/2.0, vTmp[nTmp].m_dZ/2.0);
    glVertex3d(vTmp[nTmp].m_dX, vTmp[nTmp].m_dY, vTmp[nTmp].m_dZ);
  }
  glEnd();

  vTmp[0] = BVector(1, -0.90, 0.1) - m_vMassCenter;
  vTmp[1]  = m_pPointOrig[4+9];
  vTmp[2]  = m_pPointOrig[5+9];
  vTmp[3]  = BVector(1, -1.05, -0.05) - m_vMassCenter;
  glBegin(GL_POLYGON);
  for(nTmp = 0; nTmp < 4; ++nTmp) {
    glTexCoord2d(vTmp[nTmp].m_dY/2.0, vTmp[nTmp].m_dZ/2.0);
    glVertex3d(vTmp[nTmp].m_dX, vTmp[nTmp].m_dY, vTmp[nTmp].m_dZ);
  }
  glEnd();

  vTmp[0]  = BVector(1, -1.05, -0.05) - m_vMassCenter;
  vTmp[1]  = m_pPointOrig[5+9];
  vTmp[2]  = m_pPointOrig[6+9];
  vTmp[3]  = BVector(1, -1.35, -0.05) - m_vMassCenter;
  glBegin(GL_POLYGON);
  for(nTmp = 0; nTmp < 4; ++nTmp) {
    glTexCoord2d(vTmp[nTmp].m_dY/2.0, vTmp[nTmp].m_dZ/2.0);
    glVertex3d(vTmp[nTmp].m_dX, vTmp[nTmp].m_dY, vTmp[nTmp].m_dZ);
  }
  glEnd();

  vTmp[0]  = BVector(1, -1.35, -0.05) - m_vMassCenter;
  vTmp[1]  = m_pPointOrig[6+9];
  vTmp[2]  = BVector(1, -1.50, 0.1) - m_vMassCenter;
  glBegin(GL_TRIANGLES);
  for(nTmp = 0; nTmp < 3; ++nTmp) {
    glTexCoord2d(vTmp[nTmp].m_dY/2.0, vTmp[nTmp].m_dZ/2.0);
    glVertex3d(vTmp[nTmp].m_dX, vTmp[nTmp].m_dY, vTmp[nTmp].m_dZ);
  }
  glEnd();

  vTmp[0]  = BVector(1, -1.50, 0.1) - m_vMassCenter;
  vTmp[1]  = m_pPointOrig[6+9];
  vTmp[2]  = m_pPointOrig[7+9];
  vTmp[3]  = BVector(1, -1.55, 0.3) - m_vMassCenter;
  glBegin(GL_POLYGON);
  for(nTmp = 0; nTmp < 4; ++nTmp) {
    glTexCoord2d(vTmp[nTmp].m_dY/2.0, vTmp[nTmp].m_dZ/2.0);
    glVertex3d(vTmp[nTmp].m_dX, vTmp[nTmp].m_dY, vTmp[nTmp].m_dZ);
  }
  glEnd();

  //**********************

  // OpenGLHelpers::SetColorFull(0.6, 0.1, 0.1, 0);
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 128.0f);
  vTmp[0]  = m_pPointOrig[0];  // Top
  vTmp[1]  = m_pPointOrig[9];
  vTmp[2]  = m_pPointOrig[1];
  vTmp[3]  = m_pPointOrig[10];
  vTmp[4]  = m_pPointOrig[2];
  vTmp[5]  = m_pPointOrig[11];
  vTmp[6]  = m_pPointOrig[3];
  vTmp[7]  = m_pPointOrig[12];
  vTmp[8]  = m_pPointOrig[4];
  vTmp[9]  = m_pPointOrig[13];
  /*
  vTmp[10] = m_pPointOrig[5];
  vTmp[11] = m_pPointOrig[14];
  vTmp[12] = m_pPointOrig[6];
  vTmp[13] = m_pPointOrig[15];
  vTmp[14] = m_pPointOrig[7];
  vTmp[15] = m_pPointOrig[16];  
  */
  glBegin(GL_TRIANGLE_STRIP);
  double dTexY = 0.0;
  for(nTmp = 0; nTmp < 10; ++nTmp) {
    if((nTmp > 1) && ((nTmp % 2) == 0)) {
      dTexY = dTexY + (vTmp[nTmp] - vTmp[nTmp-2]).Length() / 2.0;
    }
    glTexCoord2d(vTmp[nTmp].m_dX/2.0, dTexY);
    BVector vNormal;
    if((nTmp % 2) == 0) {
      if(nTmp < 14) {
        vNormal.Set(0, -(vTmp[nTmp].m_dZ - vTmp[nTmp+2].m_dZ), vTmp[nTmp].m_dY - vTmp[nTmp+2].m_dY);
        vNormal.ToUnitLength();
      } else {
        vNormal.Set(0, -1, 0);
      }
      glNormal3d(vNormal.m_dX, vNormal.m_dY, vNormal.m_dZ);
    }
    glVertex3d(vTmp[nTmp].m_dX, vTmp[nTmp].m_dY, vTmp[nTmp].m_dZ);
  }
  glEnd();

  glEndList();

  glNewList(DISPLIST_CARBODYNOHELI, GL_COMPILE);
  vTmp[0] = m_pPointOrig[4];
  vTmp[1] = m_pPointOrig[13];
  vTmp[2] = m_pPointOrig[5];
  vTmp[3] = m_pPointOrig[14];
  glBegin(GL_TRIANGLE_STRIP);
  dTexY = 0.0;
  for(nTmp = 0; nTmp < 4; ++nTmp) {
    glTexCoord2d(nTmp % 2 * 2, nTmp / 2);
    BVector vNormal;
    if((nTmp % 2) == 0) {
      if(nTmp < 14) {
        vNormal.Set(0, -(vTmp[nTmp].m_dZ - vTmp[nTmp+2].m_dZ), vTmp[nTmp].m_dY - vTmp[nTmp+2].m_dY);
        vNormal.ToUnitLength();
      } else {
        vNormal.Set(0, -1, 0);
      }
      glNormal3d(vNormal.m_dX, vNormal.m_dY, vNormal.m_dZ);
    }
    glVertex3d(vTmp[nTmp].m_dX, vTmp[nTmp].m_dY, vTmp[nTmp].m_dZ);
  }
  glEnd();
  glEndList();

  glNewList(DISPLIST_CARBODYNOJET, GL_COMPILE);
  vTmp[0] = m_pPointOrig[5];
  vTmp[1] = m_pPointOrig[14];
  vTmp[2] = m_pPointOrig[6];
  vTmp[3] = m_pPointOrig[15];
  glBegin(GL_TRIANGLE_STRIP);
  dTexY = 0.0;
  for(nTmp = 0; nTmp < 4; ++nTmp) {
    glTexCoord2d(nTmp % 2 * 2, nTmp / 2);
    BVector vNormal;
    if((nTmp % 2) == 0) {
      if(nTmp < 14) {
        vNormal.Set(0, -(vTmp[nTmp].m_dZ - vTmp[nTmp+2].m_dZ), vTmp[nTmp].m_dY - vTmp[nTmp+2].m_dY);
        vNormal.ToUnitLength();
      } else {
        vNormal.Set(0, -1, 0);
      }
      glNormal3d(vNormal.m_dX, vNormal.m_dY, vNormal.m_dZ);
    }
    glVertex3d(vTmp[nTmp].m_dX, vTmp[nTmp].m_dY, vTmp[nTmp].m_dZ);
  }
  glEnd();

  vTmp[0] = m_pPointOrig[6];
  vTmp[1] = m_pPointOrig[15];
  vTmp[2] = m_pPointOrig[7];
  vTmp[3] = m_pPointOrig[16];
  glBegin(GL_TRIANGLE_STRIP);
  dTexY = 0.0;
  for(nTmp = 0; nTmp < 4; ++nTmp) {
    glTexCoord2d(nTmp % 2 * 2, nTmp / 2 * 0.3);
    BVector vNormal;
    if((nTmp % 2) == 0) {
      if(nTmp < 14) {
        vNormal.Set(0, -(vTmp[nTmp].m_dZ - vTmp[nTmp+2].m_dZ), vTmp[nTmp].m_dY - vTmp[nTmp+2].m_dY);
        vNormal.ToUnitLength();
      } else {
        vNormal.Set(0, -1, 0);
      }
      glNormal3d(vNormal.m_dX, vNormal.m_dY, vNormal.m_dZ);
    }
    glVertex3d(vTmp[nTmp].m_dX, vTmp[nTmp].m_dY, vTmp[nTmp].m_dZ);
  }
  glEnd();

  glEndList();

  glNewList(DISPLIST_CARBODYJET, GL_COMPILE);
  // ************** LEFT SIDE
  vTmp[0] = m_pPointOrig[5];
  vTmp[1] = m_pPointOrig[5] * 0.8 + m_pPointOrig[14] * 0.2;
  vTmp[2] = m_pPointOrig[6];
  vTmp[3] = m_pPointOrig[6] * 0.8 + m_pPointOrig[15] * 0.2;
  glBegin(GL_TRIANGLE_STRIP);
  dTexY = 0.0;
  for(nTmp = 0; nTmp < 4; ++nTmp) {
    glTexCoord2d(nTmp % 2 * 0.4, nTmp / 2);
    BVector vNormal;
    if((nTmp % 2) == 0) {
      if(nTmp < 14) {
        vNormal.Set(0, -(vTmp[nTmp].m_dZ - vTmp[nTmp+2].m_dZ), vTmp[nTmp].m_dY - vTmp[nTmp+2].m_dY);
        vNormal.ToUnitLength();
      } else {
        vNormal.Set(0, -1, 0);
      }
      glNormal3d(vNormal.m_dX, vNormal.m_dY, vNormal.m_dZ);
    }
    glVertex3d(vTmp[nTmp].m_dX, vTmp[nTmp].m_dY, vTmp[nTmp].m_dZ);
  }
  glEnd();

  vTmp[0] = m_pPointOrig[6] * 0.2 + m_pPointOrig[15] * 0.8;
  vTmp[1] = m_pPointOrig[15];
  vTmp[2] = m_pPointOrig[7] * 0.2 + m_pPointOrig[16] * 0.8;
  vTmp[3] = m_pPointOrig[16];
  glBegin(GL_TRIANGLE_STRIP);
  dTexY = 0.0;
  for(nTmp = 0; nTmp < 4; ++nTmp) {
    glTexCoord2d(1.6 + nTmp % 2 * 0.4, nTmp / 2 * 0.3);
    BVector vNormal;
    if((nTmp % 2) == 0) {
      if(nTmp < 14) {
        vNormal.Set(0, -(vTmp[nTmp].m_dZ - vTmp[nTmp+2].m_dZ), vTmp[nTmp].m_dY - vTmp[nTmp+2].m_dY);
        vNormal.ToUnitLength();
      } else {
        vNormal.Set(0, -1, 0);
      }
      glNormal3d(vNormal.m_dX, vNormal.m_dY, vNormal.m_dZ);
    }
    glVertex3d(vTmp[nTmp].m_dX, vTmp[nTmp].m_dY, vTmp[nTmp].m_dZ);
  }
  glEnd();

  // ************** RIGHT SIDE AND LEFTOVERS
  vTmp[0] = m_pPointOrig[5] * 0.2 + m_pPointOrig[14] * 0.8;
  vTmp[1] = m_pPointOrig[14];
  vTmp[2] = m_pPointOrig[6] * 0.2 + m_pPointOrig[15] * 0.8;
  vTmp[3] = m_pPointOrig[15];
  glBegin(GL_TRIANGLE_STRIP);
  dTexY = 0.0;
  for(nTmp = 0; nTmp < 4; ++nTmp) {
    glTexCoord2d(1.6 + nTmp % 2 * 0.4, nTmp / 2);
    BVector vNormal;
    if((nTmp % 2) == 0) {
      if(nTmp < 14) {
        vNormal.Set(0, -(vTmp[nTmp].m_dZ - vTmp[nTmp+2].m_dZ), vTmp[nTmp].m_dY - vTmp[nTmp+2].m_dY);
        vNormal.ToUnitLength();
      } else {
        vNormal.Set(0, -1, 0);
      }
      glNormal3d(vNormal.m_dX, vNormal.m_dY, vNormal.m_dZ);
    }
    glVertex3d(vTmp[nTmp].m_dX, vTmp[nTmp].m_dY, vTmp[nTmp].m_dZ);
  }
  glEnd();

  vTmp[0] = m_pPointOrig[5] * 0.8 + m_pPointOrig[14] * 0.2;
  vTmp[1] = m_pPointOrig[5] * 0.2 + m_pPointOrig[14] * 0.8;
  vTmp[2] = vTmp[0] + (m_pPointOrig[6] - m_pPointOrig[5]) * 0.3;
  vTmp[3] = vTmp[1] + (m_pPointOrig[6] - m_pPointOrig[5]) * 0.3;
  glBegin(GL_TRIANGLE_STRIP);
  dTexY = 0.0;
  for(nTmp = 0; nTmp < 4; ++nTmp) {
    glTexCoord2d(0.4 + nTmp % 2 * 1.2, nTmp / 2 * 0.3);
    BVector vNormal;
    if((nTmp % 2) == 0) {
      if(nTmp < 14) {
        vNormal.Set(0, -(vTmp[nTmp].m_dZ - vTmp[nTmp+2].m_dZ), vTmp[nTmp].m_dY - vTmp[nTmp+2].m_dY);
        vNormal.ToUnitLength();
      } else {
        vNormal.Set(0, -1, 0);
      }
      glNormal3d(vNormal.m_dX, vNormal.m_dY, vNormal.m_dZ);
    }
    glVertex3d(vTmp[nTmp].m_dX, vTmp[nTmp].m_dY, vTmp[nTmp].m_dZ);
  }
  glEnd();

  vTmp[0] = m_pPointOrig[6];
  vTmp[1] = m_pPointOrig[6] * 0.8 + m_pPointOrig[15] * 0.2;
  vTmp[2] = m_pPointOrig[7];
  vTmp[3] = m_pPointOrig[7] * 0.8 + m_pPointOrig[16] * 0.2;
  glBegin(GL_TRIANGLE_STRIP);
  dTexY = 0.0;
  for(nTmp = 0; nTmp < 4; ++nTmp) {
    glTexCoord2d(nTmp % 2 * 0.4, nTmp / 2 * 0.3);
    BVector vNormal;
    if((nTmp % 2) == 0) {
      if(nTmp < 14) {
        vNormal.Set(0, -(vTmp[nTmp].m_dZ - vTmp[nTmp+2].m_dZ), vTmp[nTmp].m_dY - vTmp[nTmp+2].m_dY);
        vNormal.ToUnitLength();
      } else {
        vNormal.Set(0, -1, 0);
      }
      glNormal3d(vNormal.m_dX, vNormal.m_dY, vNormal.m_dZ);
    }
    glVertex3d(vTmp[nTmp].m_dX, vTmp[nTmp].m_dY, vTmp[nTmp].m_dZ);
  }
  glEnd();

  vTmp[0] = m_pPointOrig[7] * 0.8 + m_pPointOrig[16] * 0.2;
  vTmp[1] = m_pPointOrig[7] * 0.2 + m_pPointOrig[16] * 0.8;
  vTmp[2] = vTmp[0] + (m_pPointOrig[6] - m_pPointOrig[7]) * 0.3;
  vTmp[3] = vTmp[1] + (m_pPointOrig[6] - m_pPointOrig[7]) * 0.3;
  glBegin(GL_TRIANGLE_STRIP);
  dTexY = 0.0;
  for(nTmp = 0; nTmp < 4; ++nTmp) {
    glTexCoord2d(0.4 + nTmp % 2 * 1.2, 0.3 - nTmp / 2 * 0.3 * 0.3);
    BVector vNormal;
    if((nTmp % 2) == 0) {
      if(nTmp < 14) {
        vNormal.Set(0, -(vTmp[nTmp].m_dZ - vTmp[nTmp+2].m_dZ), vTmp[nTmp].m_dY - vTmp[nTmp+2].m_dY);
        vNormal.ToUnitLength();
      } else {
        vNormal.Set(0, -1, 0);
      }
      glNormal3d(vNormal.m_dX, vNormal.m_dY, vNormal.m_dZ);
    }
    glVertex3d(vTmp[nTmp].m_dX, vTmp[nTmp].m_dY, vTmp[nTmp].m_dZ);
  }
  glEnd();

  glEndList();
  

  //**********************
  //** Wheel
  //**********************

  // Wheel blade
  glNewList(DISPLIST_CARWHEELBLADE, GL_COMPILE);
  RenderWheelBlade(m_pWheel[0].m_dRadius * 0.6, 
                   m_pWheel[0].m_dRadius * 0.5, 
                   m_pWheel[0].m_dRadius * 0.15, 
                   0.6, 
                   0.4, 
                   0.08, 
                   0.0);
  glEndList();

  // One (right) wheel resting on the X-axis
  // OpenGLHelpers::DefineMipMapTextures(16, 16, 3, GL_RGB, m_dTXTRWheel, 1);
  OpenGLHelpers::DefineMipMapTextures(128, 128, 3, GL_RGB, m_dTXTRWheel, 1);
  glNewList(DISPLIST_CARWHEEL, GL_COMPILE);
  glShadeModel(GL_SMOOTH);
  // sides
  glEnable(GL_TEXTURE_2D);
  OpenGLHelpers::SetColorFull(1, 1, 1, 1);
  // OpenGLHelpers::DefineMipMapTextures(16, 16, 3, GL_RGB, m_dTXTRWheel, 1);
  OpenGLHelpers::DefineMipMapTextures(128, 128, 3, GL_RGB, m_dTXTRWheel, 1);
  // OpenGLHelpers::SetColorFull(0.2, 0.2, 0.2, 0);
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 128.0f);
  BVector vNormals[22];
  int fan;
  //vTmp[0] = BVector(0.1, 0, 0);
  //vNormals[0] = BVector(1, 0, 0);
  glBegin(GL_TRIANGLE_STRIP);
  //glNormal3d(vNormals[0].m_dX, vNormals[0].m_dY, vNormals[0].m_dZ); 
  //glTexCoord2d(0.5, 0.5);
  //glVertex3d(vTmp[0].m_dX, vTmp[0].m_dY, vTmp[0].m_dZ);

  for(fan = 0; fan < 21; ++fan) {
    vTmp[fan] = BVector(0.1, 0, 0) +
                (BVector(0, 1, 0) * m_pWheel[0].m_dRadius * 0.9 *
                 cos((double(fan) / 20.0) * 2.0 * g_cdPI)) + 
                (BVector(0, 0, -1) * m_pWheel[0].m_dRadius * 0.9 *
                 sin((double(fan) / 20.0) * 2.0 * g_cdPI));
    vNormals[fan] = BVector(1, 0, 0) + vTmp[fan];
    vNormals[fan].ToUnitLength();

    glNormal3d(vNormals[fan].m_dX, vNormals[fan].m_dY, vNormals[fan].m_dZ);
    glTexCoord2d( vTmp[fan].m_dY / (m_pWheel[0].m_dRadius * 1.8) + 0.5, 
                 -vTmp[fan].m_dZ / (m_pWheel[0].m_dRadius * 1.8) + 0.5);
    glVertex3d(vTmp[fan].m_dX, vTmp[fan].m_dY, vTmp[fan].m_dZ);
    vTmp[fan] = BVector(0.1, 0, 0) +
                (BVector(0, 1, 0) * m_pWheel[0].m_dRadius * 0.65 *
                 cos((double(fan) / 20.0) * 2.0 * g_cdPI)) + 
                (BVector(0, 0, -1) * m_pWheel[0].m_dRadius * 0.65 *
                 sin((double(fan) / 20.0) * 2.0 * g_cdPI));
    glTexCoord2d( vTmp[fan].m_dY / (m_pWheel[0].m_dRadius * 1.8) + 0.5, 
                 -vTmp[fan].m_dZ / (m_pWheel[0].m_dRadius * 1.8) + 0.5);
    glVertex3d(vTmp[fan].m_dX, vTmp[fan].m_dY, vTmp[fan].m_dZ);
  }
  // OpenGLHelpers::TriangleFanWithNormals(vTmp, vNormals, 22);
  glEnd();
  glDisable(GL_TEXTURE_2D);

  OpenGLHelpers::SetColorFull(0.2, 0.2, 0.2, 0);
  glBegin(GL_TRIANGLE_STRIP);
  for(fan = 0; fan < 21; ++fan) {
    vTmp[fan] = BVector(-0.1, 0, 0) +
                (BVector(0, 1, 0) * m_pWheel[0].m_dRadius * 0.9 *
                 cos((double(fan) / 20.0) * 2.0 * g_cdPI)) + 
                (BVector(0, 0, -1) * m_pWheel[0].m_dRadius * 0.9 *
                 sin((double(fan) / 20.0) * 2.0 * g_cdPI));
    vNormals[fan] = BVector(-1, 0, 0) + vTmp[fan];
    vNormals[fan].ToUnitLength();

    glNormal3d(vNormals[fan].m_dX, vNormals[fan].m_dY, vNormals[fan].m_dZ);
    glVertex3d(vTmp[fan].m_dX, vTmp[fan].m_dY, vTmp[fan].m_dZ);
    vTmp[fan] = BVector(-0.1, 0, 0) +
                (BVector(0, 1, 0) * m_pWheel[0].m_dRadius * 0.65 *
                 cos((double(fan) / 20.0) * 2.0 * g_cdPI)) + 
                (BVector(0, 0, -1) * m_pWheel[0].m_dRadius * 0.65 *
                 sin((double(fan) / 20.0) * 2.0 * g_cdPI));
    glVertex3d(vTmp[fan].m_dX, vTmp[fan].m_dY, vTmp[fan].m_dZ);
  }
  glEnd();

  // Blades 
  glPushMatrix();
  glCallList(DISPLIST_CARWHEELBLADE);
  glRotated(60.0, 1, 0, 0);
  glCallList(DISPLIST_CARWHEELBLADE);
  glRotated(60.0, 1, 0, 0);
  glCallList(DISPLIST_CARWHEELBLADE);
  glRotated(60.0, 1, 0, 0);
  glCallList(DISPLIST_CARWHEELBLADE);
  glRotated(60.0, 1, 0, 0);
  glCallList(DISPLIST_CARWHEELBLADE);
  glRotated(60.0, 1, 0, 0);
  glCallList(DISPLIST_CARWHEELBLADE);
  glPopMatrix();

  // Traction surface TOP
  OpenGLHelpers::SetColorFull(0.2, 0.2, 0.2, 0);
  for(fan = 0; fan < 21; ++fan) {
    vTmp[fan * 2] = BVector(-0.08, 0, 0) +
                    (BVector(0, 1, 0) * m_pWheel[0].m_dRadius *
                     cos((double(fan) / 20.0) * 2.0 * g_cdPI)) + 
                    (BVector(0, 0, -1) * m_pWheel[0].m_dRadius *
                     sin((double(fan) / 20.0) * 2.0 * g_cdPI));
    vTmp[fan * 2 + 1] = BVector(0.08, 0, 0) +
                        (BVector(0, 1, 0) * m_pWheel[0].m_dRadius *
                         cos((double(fan) / 20.0) * 2.0 * g_cdPI)) + 
                        (BVector(0, 0, -1) * m_pWheel[0].m_dRadius *
                         sin((double(fan) / 20.0) * 2.0 * g_cdPI));
    vNormals[fan] = (BVector(0, 1, 0) * cos((double(fan) / 20.0) * 2.0 * g_cdPI)) + 
                    (BVector(0, 0, -1) * sin((double(fan) / 20.0) * 2.0 * g_cdPI));
    vNormals[fan].ToUnitLength();
  }
  OpenGLHelpers::TriangleStripWithNormals(vTmp, vNormals, 42);

  for(fan = 0; fan < 21; ++fan) {
    vTmp[fan * 2] = BVector(-0.1, 0, 0) +
                    (BVector(0, 1, 0) * m_pWheel[0].m_dRadius * 0.9 *
                     cos((double(fan) / 20.0) * 2.0 * g_cdPI)) + 
                    (BVector(0, 0, -1) * m_pWheel[0].m_dRadius * 0.9 *
                     sin((double(fan) / 20.0) * 2.0 * g_cdPI));
    vTmp[fan * 2 + 1] = BVector(-0.08, 0, 0) +
                        (BVector(0, 1, 0) * m_pWheel[0].m_dRadius *
                         cos((double(fan) / 20.0) * 2.0 * g_cdPI)) + 
                        (BVector(0, 0, -1) * m_pWheel[0].m_dRadius *
                         sin((double(fan) / 20.0) * 2.0 * g_cdPI));
    vNormals[fan] = vTmp[fan * 2];
    vNormals[fan].ToUnitLength();
  }
  OpenGLHelpers::TriangleStripWithNormals(vTmp, vNormals, 42);

  for(fan = 0; fan < 21; ++fan) {
    vTmp[fan * 2] = BVector(0.08, 0, 0) +
                    (BVector(0, 1, 0) * m_pWheel[0].m_dRadius *
                     cos((double(fan) / 20.0) * 2.0 * g_cdPI)) + 
                    (BVector(0, 0, -1) * m_pWheel[0].m_dRadius *
                     sin((double(fan) / 20.0) * 2.0 * g_cdPI));
    vTmp[fan * 2 + 1] = BVector(0.1, 0, 0) +
                        (BVector(0, 1, 0) * m_pWheel[0].m_dRadius * 0.9 *
                         cos((double(fan) / 20.0) * 2.0 * g_cdPI)) + 
                        (BVector(0, 0, -1) * m_pWheel[0].m_dRadius * 0.9 *
                         sin((double(fan) / 20.0) * 2.0 * g_cdPI));
    vNormals[fan] = vTmp[fan * 2];
    vNormals[fan].ToUnitLength();
  }
  OpenGLHelpers::TriangleStripWithNormals(vTmp, vNormals, 42);

  // Traction surface INSIDE
  OpenGLHelpers::SetColorFull(0.2, 0.2, 0.2, 0);
  for(fan = 0; fan < 21; ++fan) {
    vTmp[fan * 2] = BVector(-0.09, 0, 0) +
                    (BVector(0, 1, 0) * m_pWheel[0].m_dRadius * 0.6 *
                     cos((double(fan) / 20.0) * 2.0 * g_cdPI)) + 
                    (BVector(0, 0, -1) * m_pWheel[0].m_dRadius * 0.6 *
                     sin((double(fan) / 20.0) * 2.0 * g_cdPI));
    vTmp[fan * 2 + 1] = BVector(0.09, 0, 0) +
                        (BVector(0, 1, 0) * m_pWheel[0].m_dRadius * 0.6 *
                         cos((double(fan) / 20.0) * 2.0 * g_cdPI)) + 
                        (BVector(0, 0, -1) * m_pWheel[0].m_dRadius * 0.6 *
                         sin((double(fan) / 20.0) * 2.0 * g_cdPI));
    vNormals[fan] = ((BVector(0, 1, 0) * cos((double(fan) / 20.0) * 2.0 * g_cdPI)) + 
                     (BVector(0, 0, -1) * sin((double(fan) / 20.0) * 2.0 * g_cdPI))) * 
                    -1.0;
    vNormals[fan].ToUnitLength();
  }
  OpenGLHelpers::TriangleStripWithNormals(vTmp, vNormals, 42);

  for(fan = 0; fan < 21; ++fan) {
    BVector vFoo = (BVector(0, 1, 0) * m_pWheel[0].m_dRadius * 0.8 *
                    cos((double(fan) / 20.0) * 2.0 * g_cdPI)) + 
                   (BVector(0, 0, -1) * m_pWheel[0].m_dRadius * 0.8 *
                    sin((double(fan) / 20.0) * 2.0 * g_cdPI));
    vTmp[fan * 2] = BVector(-0.1, 0, 0) +
                    (BVector(0, 1, 0) * m_pWheel[0].m_dRadius * 0.65 *
                     cos((double(fan) / 20.0) * 2.0 * g_cdPI)) + 
                    (BVector(0, 0, -1) * m_pWheel[0].m_dRadius * 0.65 *
                     sin((double(fan) / 20.0) * 2.0 * g_cdPI));
    vTmp[fan * 2 + 1] = BVector(-0.09, 0, 0) +
                        (BVector(0, 1, 0) * m_pWheel[0].m_dRadius * 0.6 *
                         cos((double(fan) / 20.0) * 2.0 * g_cdPI)) + 
                        (BVector(0, 0, -1) * m_pWheel[0].m_dRadius * 0.6 *
                         sin((double(fan) / 20.0) * 2.0 * g_cdPI));
    vNormals[fan] = vTmp[fan * 2] - vFoo;
    vNormals[fan].ToUnitLength();
  }
  OpenGLHelpers::TriangleStripWithNormals(vTmp, vNormals, 42);

  for(fan = 0; fan < 21; ++fan) {
    BVector vFoo = (BVector(0, 1, 0) * m_pWheel[0].m_dRadius * 0.8 *
                    cos((double(fan) / 20.0) * 2.0 * g_cdPI)) + 
                   (BVector(0, 0, -1) * m_pWheel[0].m_dRadius * 0.8 *
                    sin((double(fan) / 20.0) * 2.0 * g_cdPI));
    vTmp[fan * 2] = BVector(0.09, 0, 0) +
                    (BVector(0, 1, 0) * m_pWheel[0].m_dRadius * 0.6 *
                     cos((double(fan) / 20.0) * 2.0 * g_cdPI)) + 
                    (BVector(0, 0, -1) * m_pWheel[0].m_dRadius * 0.6 *
                     sin((double(fan) / 20.0) * 2.0 * g_cdPI));
    vTmp[fan * 2 + 1] = BVector(0.1, 0, 0) +
                        (BVector(0, 1, 0) * m_pWheel[0].m_dRadius * 0.65 *
                         cos((double(fan) / 20.0) * 2.0 * g_cdPI)) + 
                        (BVector(0, 0, -1) * m_pWheel[0].m_dRadius * 0.65 *
                         sin((double(fan) / 20.0) * 2.0 * g_cdPI));
    vNormals[fan] = vTmp[fan * 2] - vFoo;
    vNormals[fan].ToUnitLength();
  }
  OpenGLHelpers::TriangleStripWithNormals(vTmp, vNormals, 42);

  glEndList();

  // One (right) wheel resting on the X-axis
  glNewList(DISPLIST_CARWHEELBROKEN, GL_COMPILE);
  glShadeModel(GL_SMOOTH);
  // sides
  OpenGLHelpers::SetColorFull(0.2, 0.2, 0.2, 0);
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 128.0f);
  vTmp[0] = BVector(0.1, 0, 0);
  vNormals[0] = BVector(1, 0, 0);
  for(fan = 0; fan < 21; ++fan) {
    vTmp[fan + 1] = BVector(0.15, 0, 0) +
                    (BVector(0, 1, 0) * m_pWheel[0].m_dRadius * 0.85 * 
                     cos((double(fan) / 20.0) * 2.0 * g_cdPI)) + 
                    (BVector(0, 0, -1) * m_pWheel[0].m_dRadius * 0.85 *
                     sin((double(fan) / 20.0) * 2.0 * g_cdPI));
    vNormals[fan + 1] = BVector(1, 0, 0) + vTmp[fan + 1];
    vNormals[fan + 1].ToUnitLength();
  }
  OpenGLHelpers::TriangleFanWithNormals(vTmp, vNormals, 22);
  OpenGLHelpers::SetColorFull(0.2, 0.2, 0.2, 0);
  vTmp[0] = BVector(-0.1, 0, 0);
  vNormals[0] = BVector(-1, 0, 0);
  for(fan = 0; fan < 21; ++fan) {
    vTmp[fan + 1] = BVector(-0.15, 0, 0) +
                    (BVector(0, 1, 0) * m_pWheel[0].m_dRadius * 0.85 *
                     cos((double(fan) / 20.0) * 2.0 * g_cdPI)) + 
                    (BVector(0, 0, -1) * m_pWheel[0].m_dRadius * 0.85 *
                     sin((double(fan) / 20.0) * 2.0 * g_cdPI));
    vNormals[fan + 1] = BVector(-2, 0, 0) + vTmp[fan + 1];
    vNormals[fan + 1].ToUnitLength();
  }
  OpenGLHelpers::TriangleFanWithNormals(vTmp, vNormals, 22);

  // Hub caps (missing)
  OpenGLHelpers::SetColorFull(0.3, 0.3, 0.3, 0);
  vTmp[0] = BVector(0.16, 0, 0);
  vNormals[0] = BVector(1, 0, 0);
  for(fan = 0; fan < 21; ++fan) {
    vTmp[fan + 1] = BVector(0.16, 0, 0) +
                    (BVector(0, 1, 0) * m_pWheel[0].m_dRadius * 0.7 *
                     cos((double(fan) / 20.0) * 2.0 * g_cdPI)) + 
                    (BVector(0, 0, -1) * m_pWheel[0].m_dRadius * 0.7 *
                     sin((double(fan) / 20.0) * 2.0 * g_cdPI));
    vNormals[fan + 1] = BVector(1, 0, 0) + vTmp[fan + 1];
    vNormals[fan + 1].ToUnitLength();
  }
  OpenGLHelpers::TriangleFanWithNormals(vTmp, vNormals, 22);

  // Traction surface
  OpenGLHelpers::SetColorFull(0.2, 0.2, 0.2, 0);
  for(fan = 0; fan < 21; ++fan) {
    vTmp[fan * 2] = BVector(-0.15, 0, 0) +
                    (BVector(0, 1, 0) * m_pWheel[0].m_dRadius * 0.85 *
                     cos((double(fan) / 20.0) * 2.0 * g_cdPI)) + 
                    (BVector(0, 0, -1) * m_pWheel[0].m_dRadius * 0.85 *
                     sin((double(fan) / 20.0) * 2.0 * g_cdPI));
    vTmp[fan * 2 + 1] = BVector(0.15, 0, 0) +
                        (BVector(0, 1, 0) * m_pWheel[0].m_dRadius * 0.85 *
                         cos((double(fan) / 20.0) * 2.0 * g_cdPI)) + 
                        (BVector(0, 0, -1) * m_pWheel[0].m_dRadius * 0.85 *
                         sin((double(fan) / 20.0) * 2.0 * g_cdPI));
    vNormals[fan] = (BVector(0, 1, 0) * cos((double(fan) / 20.0) * 2.0 * g_cdPI)) + 
                    (BVector(0, 0, -1) * sin((double(fan) / 20.0) * 2.0 * g_cdPI));
    vNormals[fan].ToUnitLength();
  }
  OpenGLHelpers::TriangleStripWithNormals(vTmp, vNormals, 42);

  // Grooves
  //OpenGLHelpers::SetColorFull(0, 0, 0, 0);
  //OpenGLHelpers::Lines(vTmp, 42);
  glEndList();


}


void BCar::RenderWheelBlade(double dLen, 
                            double dWide, 
                            double dNarrow, 
                            double dWideRound,
                            double dNarrowRound,
                            double dDepthOut,
                            double dDepthIn) {
//  End profile:
//   _______
//  / /   \ \
//  I I   I I
//  I I   I I
//  I I   I I

  // OpenGLHelpers::SetColorFull(0.5, 0.5, 0.7, 0);
  OpenGLHelpers::SetColorFull(0.35, 0.35, 0.35, 0);
  glBegin(GL_TRIANGLE_STRIP);

  glNormal3d(0, 0, -1);
  glVertex3d(dDepthIn, 0, -dWide / 2.0);
  glNormal3d(0, 0, -1);
  glVertex3d(dDepthIn, dLen, -dNarrow / 2.0);

  glNormal3d(0.7, 0, -0.7);
  glVertex3d(dDepthOut * dWideRound, 0, -dWide / 2.0);
  glNormal3d(0.7, 0, -0.7);
  glVertex3d(dDepthOut * dNarrowRound, dLen, -dNarrow / 2.0);

  glNormal3d(1, 0, 0);
  glVertex3d(dDepthOut, 0, -dWide * dWideRound / 2.0);
  glNormal3d(1, 0, 0);
  glVertex3d(dDepthOut, dLen, -dNarrow * dNarrowRound / 2.0);

  glNormal3d(1, 0, 0);
  glVertex3d(dDepthOut, 0, dWide * dWideRound / 2.0);
  glNormal3d(1, 0, 0);
  glVertex3d(dDepthOut, dLen, dNarrow * dNarrowRound / 2.0);

  glNormal3d(0.7, 0, 0.7);
  glVertex3d(dDepthOut * dWideRound, 0, dWide / 2.0);
  glNormal3d(0.7, 0, 0.7);
  glVertex3d(dDepthOut * dNarrowRound, dLen, dNarrow / 2.0);

  glNormal3d(0, 0, 1);
  glVertex3d(dDepthIn, 0, dWide / 2.0);
  glNormal3d(0, 0, 1);
  glVertex3d(dDepthIn, dLen, dNarrow / 2.0);

  glEnd();

  // Back surface
  OpenGLHelpers::SetColorFull(0.2, 0.2, 0.2, 0);
  BVector vTmp[44];
  BVector vNormals[44];
  vTmp[0] = BVector(dDepthIn, 0, 0);
  vNormals[0] = BVector(-1, 0, 0);
  for(int fan = 0; fan < 21; ++fan) {
    vTmp[fan + 1] = BVector(dDepthIn, 0, 0) +
                    (BVector(0, 1, 0) * m_pWheel[0].m_dRadius * 0.6 *
                     cos((double(fan) / 20.0) * 2.0 * g_cdPI)) + 
                    (BVector(0, 0, -1) * m_pWheel[0].m_dRadius * 0.6 *
                     sin((double(fan) / 20.0) * 2.0 * g_cdPI));
    vNormals[fan + 1] = BVector(-2, 0, 0) + vTmp[fan + 1];
    vNormals[fan + 1].ToUnitLength();
  }
  OpenGLHelpers::TriangleFanWithNormals(vTmp, vNormals, 22);

  /*
  // Hub caps
  OpenGLHelpers::SetColorFull(0.5, 0.5, 0.7, 0);
  vTmp[0] = BVector(0.11, 0, 0);
  vNormals[0] = BVector(1, 0, 0);
  for(fan = 0; fan < 21; ++fan) {
    vTmp[fan + 1] = BVector(0.1, 0, 0) +
                    (BVector(0, 1, 0) * m_pWheel[0].m_dRadius * 0.65 *
                     cos((double(fan) / 20.0) * 2.0 * g_cdPI)) + 
                    (BVector(0, 0, -1) * m_pWheel[0].m_dRadius * 0.65 *
                     sin((double(fan) / 20.0) * 2.0 * g_cdPI));
    vNormals[fan + 1] = BVector(1, 0, 0) + vTmp[fan + 1];
    vNormals[fan + 1].ToUnitLength();
  }
  OpenGLHelpers::TriangleFanWithNormals(vTmp, vNormals, 22);
  */
}


void BCar::Paint(int m_nPhysicsSteps) {
  int i;

  if(!m_bWireframe) {
    // Find out car center
    BVector vCenter = (m_pPoint[0].m_vLocation + 
                       m_pPoint[9].m_vLocation + 
                       m_pPoint[7].m_vLocation + 
                       m_pPoint[16].m_vLocation +
                       m_pPoint[1].m_vLocation + 
                       m_pPoint[10].m_vLocation + 
                       m_pPoint[6].m_vLocation + 
                       m_pPoint[15].m_vLocation) * (1.0 / 8.0);

    // Draw body
    glPushMatrix();
    glTranslated(vCenter.m_dX, vCenter.m_dY, vCenter.m_dZ);
    RotateCarToRightOrientation();
    glCallList(DISPLIST_CARBODY);
    if(m_nHeliMode || !m_bHeliHatchesOK) {
      // glCallList(DISPLIST_CARBODYHELI);
    } else {
      glCallList(DISPLIST_CARBODYNOHELI);
    }
    if(m_nJetMode) {
      glCallList(DISPLIST_CARBODYJET);
    } else {
      glCallList(DISPLIST_CARBODYNOJET);
    }
    // Draw wheels
    // glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 100.0);
    for(i = 0; i < m_nWheels; ++i) {
      glPushMatrix();
      glTranslated(m_pWheel[i].m_vLocationOrig.m_dX, 
                   m_pWheel[i].m_vLocationOrig.m_dY, 
                   m_pWheel[i].m_vLocationOrig.m_dZ - m_pWheel[i].m_dSuspension);

      // Rotate according to wheel angle (i.e. wheel revolves)
      if(m_bBreaking && !m_bAccelerating && !m_bReversing) {
        m_pWheel[i].m_dAngleStep = 0.0;
      } else if(m_pWheel[i].m_bInGround) {
        // Update revolving speed
        double dDistance = m_pWheel[i].m_orientation.m_vForward.ScalarProduct(m_pPoint[m_pWheel[i].m_nBasePoint1].m_vector) * m_nPhysicsSteps;
        m_pWheel[i].m_dAngleStep = dDistance / m_pWheel[i].m_dRadius;
      } else if(m_bAccelerating) {
        m_pWheel[i].m_dAngleStep += 0.4;
      } else if(m_bReversing) {
        m_pWheel[i].m_dAngleStep -= 0.4;
      }
      m_pWheel[i].m_dAngle += m_pWheel[i].m_dAngleStep;

      if(!m_pWheel[i].m_bRear) {
        // Rotate according to turn
        glRotated(m_dTurn * m_dSteeringAid * 45.0, 0, 0, -1);
      }
      if(m_pWheel[i].m_bLeft) {
        // Rotate to make left wheel
        glRotated(180.0, 0, 0, -1);
        glRotated(m_pWheel[i].m_dAngle / g_cdPI * 180.0, -1, 0, 0);
      } else {
        glRotated(-m_pWheel[i].m_dAngle / g_cdPI * 180.0, -1, 0, 0);
      }
      if(m_pWheel[i].m_bBroken) {
        glCallList(DISPLIST_CARWHEELBROKEN);
      } else {
        glCallList(DISPLIST_CARWHEEL);
      }
      glPopMatrix();
      m_pWheel[i].m_bInGround = false;
    }
    glPopMatrix();
    // glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0);
  } else {
    // Draw as wireframe
    OpenGLHelpers::SetColorFull(0, 0, 0.5, 0);
    for(i = 0; i < m_nStrings; ++i) {
      OpenGLHelpers::Line(m_pPoint[m_pString[i].m_nP1].m_vLocation,
                          m_pPoint[m_pString[i].m_nP2].m_vLocation);
    }
  }
}


double BCar::AngleBetweenVectorsAlongX(const BVector &v1, const BVector &v2, double dSign) {
  BVector vX1 = v1;
  vX1.m_dX = 0.0;
  vX1.ToUnitLength();
  BVector vX2 = v2;
  vX2.m_dX = 0.0;
  vX2.ToUnitLength();
  double dAngle = acos(vX1.ScalarProduct(vX2));
  if(dSign < 0.0) {
    dAngle = -dAngle;
  }
  return dAngle;
}

double BCar::AngleBetweenVectorsAlongY(const BVector &v1, const BVector &v2, double dSign) {
  BVector vY1 = v1;
  vY1.m_dY = 0.0;
  vY1.ToUnitLength();
  BVector vY2 = v2;
  vY2.m_dY = 0.0;
  vY2.ToUnitLength();
  double dAngle = acos(vY1.ScalarProduct(vY2));
  if(dSign < 0.0) {
    dAngle = -dAngle;
  }
  return dAngle;
}

double BCar::AngleBetweenVectorsAlongZ(const BVector &v1, const BVector &v2, double dSign) {
  BVector vZ1 = v1;
  vZ1.m_dZ = 0.0;
  vZ1.ToUnitLength();
  BVector vZ2 = v2;
  vZ2.m_dX = 0.0;
  vZ2.ToUnitLength();
  double dAngle = acos(vZ1.ScalarProduct(vZ2));
  if(dSign < 0.0) {
    dAngle = -dAngle;
  }
  return dAngle;
}


double BCar::LengthAlongZ(BVector &rv) {
  return sqrt(rv.m_dX * rv.m_dX + rv.m_dY * rv.m_dY);
}

void BCar::RotateAroundZ(BOrientation &rOrientation, double dAngle) {
  double dA1, dA2, dA3;
  double dL1, dL2, dL3;
  dA1 = AngleBetweenVectorsAlongZ(rOrientation.m_vForward, BVector(0, 1, 0), rOrientation.m_vForward.m_dX);
  dA2 = AngleBetweenVectorsAlongZ(rOrientation.m_vUp, BVector(0, 1, 0), rOrientation.m_vUp.m_dX);
  dA3 = AngleBetweenVectorsAlongZ(rOrientation.m_vRight, BVector(0, 1, 0), rOrientation.m_vRight.m_dX);
  dL1 = LengthAlongZ(rOrientation.m_vForward);
  dL2 = LengthAlongZ(rOrientation.m_vUp);
  dL3 = LengthAlongZ(rOrientation.m_vRight);
  rOrientation.m_vForward.m_dX = sin(dA1 + dAngle) * dL1;
  rOrientation.m_vForward.m_dY = cos(dA1 + dAngle) * dL1;
  rOrientation.m_vUp.m_dX = sin(dA2 + dAngle) * dL2;
  rOrientation.m_vUp.m_dY = cos(dA2 + dAngle) * dL2;
  rOrientation.m_vRight.m_dX = sin(dA3 + dAngle) * dL3;
  rOrientation.m_vRight.m_dY = cos(dA3 + dAngle) * dL3;
}

double BCar::LengthAlongX(BVector &rv) {
  return sqrt(rv.m_dY * rv.m_dY + rv.m_dZ * rv.m_dZ);
}

void BCar::RotateAroundX(BOrientation &rOrientation, double dAngle) {
  double dA1, dA2, dA3;
  double dL1, dL2, dL3;
  dA1 = AngleBetweenVectorsAlongX(rOrientation.m_vForward, BVector(0, 1, 0), -rOrientation.m_vForward.m_dZ);
  dA2 = AngleBetweenVectorsAlongX(rOrientation.m_vUp, BVector(0, 1, 0), -rOrientation.m_vUp.m_dZ);
  dA3 = AngleBetweenVectorsAlongX(rOrientation.m_vRight, BVector(0, 1, 0), -rOrientation.m_vRight.m_dZ);
  dL1 = LengthAlongX(rOrientation.m_vForward);
  dL2 = LengthAlongX(rOrientation.m_vUp);
  dL3 = LengthAlongX(rOrientation.m_vRight);
  rOrientation.m_vForward.m_dZ = -sin(dA1 + dAngle) * dL1;
  rOrientation.m_vForward.m_dY = cos(dA1 + dAngle) * dL1;
  rOrientation.m_vUp.m_dZ = -sin(dA2 + dAngle) * dL2;
  rOrientation.m_vUp.m_dY = cos(dA2 + dAngle) * dL2;
  rOrientation.m_vRight.m_dZ = -sin(dA3 + dAngle) * dL3;
  rOrientation.m_vRight.m_dY = cos(dA3 + dAngle) * dL3;
}

static double RadToDeg(double dRad) {
  return dRad / 3.141592654 * 180.0;
}

void BCar::RotateCarToRightOrientation() {
  BOrientation orientation = m_orientation;
  double dAlpha, dBeta, dGamma;

  // Alpha component
  dAlpha = AngleBetweenVectorsAlongZ(orientation.m_vForward, BVector(0, 1, 0), orientation.m_vForward.m_dX);
  RotateAroundZ(orientation, -dAlpha);

  // Beta component
  dBeta = AngleBetweenVectorsAlongX(orientation.m_vForward, BVector(0, 1, 0), -orientation.m_vForward.m_dZ);
  RotateAroundX(orientation, -dBeta);

  // Gamma component
  dGamma = AngleBetweenVectorsAlongY(orientation.m_vUp, BVector(0, 0, -1), orientation.m_vUp.m_dX);

  // Rotate car according to orientation
  glRotated(-RadToDeg(dAlpha), 0, 0, 1);
  glRotated(-RadToDeg(dBeta),  1, 0, 0);
  glRotated(-RadToDeg(dGamma), 0, 1, 0);
}















//*****************************************************************************
// OLD STUFF
//*****************************************************************************

#if 0

// Original version, without display lists

void BCar::Paint() {
  static BVector vTmp[44];
  int i;

  // glEnable(GL_BLEND);
  // glBlendFunc(GL_ONE, GL_SRC_COLOR);

  if(!m_bWireframe) {
    // Prepare visualization points
    BVector vCenter = (m_pPoint[0].m_vLocation + 
                       m_pPoint[9].m_vLocation + 
                       m_pPoint[7].m_vLocation + 
                       m_pPoint[16].m_vLocation +
                       m_pPoint[1].m_vLocation + 
                       m_pPoint[10].m_vLocation + 
                       m_pPoint[6].m_vLocation + 
                       m_pPoint[15].m_vLocation) * (1.0 / 8.0);
    BOrientation orientation;
    orientation.m_vForward = (m_pPoint[0].m_vLocation - m_pPoint[7].m_vLocation);
    orientation.m_vRight   = (m_pPoint[16].m_vLocation - m_pPoint[7].m_vLocation);
    orientation.m_vUp = orientation.m_vForward.CrossProduct(orientation.m_vRight);
    orientation.Normalize();
    /*
    for(i = 0; i < m_nPoints; ++i) {
      m_pPointVis[i] = orientation.m_vRight   * m_pPointOrig[i].m_dX +
                       orientation.m_vForward * m_pPointOrig[i].m_dY -
                       orientation.m_vUp      * m_pPointOrig[i].m_dZ +
                       vCenter;
    }
    */

    // Draw body
    glTranslated(vCenter.m_dX, vCenter.m_dY, vCenter.m_dZ);
    glCallList(DISPLIST_CARBODY);
    glTranslated(-vCenter.m_dX, -vCenter.m_dY, -vCenter.m_dZ);
    /*
    OpenGLHelpers::SetColorFull(0.7, 0.2, 0.2, 0);
    vTmp[0]  = m_pPointVis[1];
    vTmp[1]  = m_pPointVis[0];
    vTmp[2]  = m_pPointVis[2];
    vTmp[3]  = m_pPointVis[8];
    vTmp[4]  = m_pPointVis[3];
    vTmp[5]  = m_pPointVis[7];
    vTmp[6]  = m_pPointVis[4];
    vTmp[7]  = m_pPointVis[6];
    vTmp[8]  = m_pPointVis[5];
    OpenGLHelpers::TriangleStrip(vTmp, 9);

    glNormal3d(orientation.m_vRight.m_dX, 
               orientation.m_vRight.m_dY,
               orientation.m_vRight.m_dZ);
    OpenGLHelpers::SetColorFull(0.7, 0.2, 0.2, 0);
    vTmp[0]  = m_pPointVis[1 + 9];
    vTmp[1]  = m_pPointVis[0 + 9];
    vTmp[2]  = m_pPointVis[2 + 9];
    vTmp[3]  = m_pPointVis[8 + 9];
    vTmp[4]  = m_pPointVis[3 + 9];
    vTmp[5]  = m_pPointVis[7 + 9];
    vTmp[6]  = m_pPointVis[4 + 9];
    vTmp[7]  = m_pPointVis[6 + 9];
    vTmp[8]  = m_pPointVis[5 + 9];
    OpenGLHelpers::TriangleStrip(vTmp, 9);

    OpenGLHelpers::SetColorFull(0.6, 0.1, 0.1, 0);
    vTmp[0]  = m_pPointVis[0];
    vTmp[1]  = m_pPointVis[9];
    vTmp[2]  = m_pPointVis[1];
    vTmp[3]  = m_pPointVis[10];
    vTmp[4]  = m_pPointVis[2];
    vTmp[5]  = m_pPointVis[11];
    vTmp[6]  = m_pPointVis[3];
    vTmp[7]  = m_pPointVis[12];
    vTmp[8]  = m_pPointVis[4];
    vTmp[9]  = m_pPointVis[13];
    vTmp[10] = m_pPointVis[5];
    vTmp[11] = m_pPointVis[14];
    vTmp[12] = m_pPointVis[6];
    vTmp[13] = m_pPointVis[15];
    vTmp[14] = m_pPointVis[7];
    vTmp[15] = m_pPointVis[16];
    OpenGLHelpers::TriangleStrip(vTmp, 16);

    OpenGLHelpers::SetColorFull(0.05, 0.05, 0.05, 0);
    vTmp[0] = m_pPointVis[7];
    vTmp[1] = m_pPointVis[16];
    vTmp[2] = m_pPointVis[8];
    vTmp[3] = m_pPointVis[17];
    vTmp[4] = m_pPointVis[0];
    vTmp[5] = m_pPointVis[9];
    OpenGLHelpers::TriangleStrip(vTmp, 6);
    */
  } else {
    // Draw as wireframe
    OpenGLHelpers::SetColorFull(0, 0, 0.5, 0);
    for(i = 0; i < m_nStrings; ++i) {
      OpenGLHelpers::Line(m_pPoint[m_pString[i].m_nP1].m_vLocation,
                          m_pPoint[m_pString[i].m_nP2].m_vLocation);
    }
  }

  // Draw wheels
  for(i = 0; i < m_nWheels; ++i) {
    // sides
    OpenGLHelpers::SetColorFull(0.2, 0.2, 0.2, 0);
    int fan;
    if(m_pWheel[i].m_bLeft) {
      vTmp[0] = m_pWheel[i].m_vLocation + m_pWheel[i].m_orientation.m_vRight * 0.1;
      for(fan = 0; fan < 21; ++fan) {
          vTmp[fan + 1] = m_pWheel[i].m_vLocation + 
                          m_pWheel[i].m_orientation.m_vRight * 0.1 +
                          (m_pWheel[i].m_orientation.m_vForward * m_pWheel[i].m_dRadius *
                           cos((double(fan) / 20.0) * 2.0 * g_cdPI)) + 
                          (m_pWheel[i].m_orientation.m_vUp * m_pWheel[i].m_dRadius *
                           sin((double(fan) / 20.0) * 2.0 * g_cdPI));
        }
    } else {
      vTmp[0] = m_pWheel[i].m_vLocation - m_pWheel[i].m_orientation.m_vRight * 0.1;
      for(fan = 0; fan < 21; ++fan) {
        vTmp[fan + 1] = m_pWheel[i].m_vLocation - 
                        m_pWheel[i].m_orientation.m_vRight * 0.1 +
                        (m_pWheel[i].m_orientation.m_vForward * m_pWheel[i].m_dRadius *
                         cos((double(fan) / 20.0) * 2.0 * g_cdPI)) + 
                        (m_pWheel[i].m_orientation.m_vUp * m_pWheel[i].m_dRadius *
                         sin((double(fan) / 20.0) * 2.0 * g_cdPI));
      }
    }
    OpenGLHelpers::TriangleFan(vTmp, 22);

    // Hub caps
    OpenGLHelpers::SetColorFull(0.5, 0.5, 0.7, 0);
    if(m_pWheel[i].m_bLeft) {
      vTmp[0] = m_pWheel[i].m_vLocation + m_pWheel[i].m_orientation.m_vRight * 0.11;
      for(fan = 0; fan < 21; ++fan) {
          vTmp[fan + 1] = m_pWheel[i].m_vLocation + 
                          m_pWheel[i].m_orientation.m_vRight * 0.11 +
                          (m_pWheel[i].m_orientation.m_vForward * m_pWheel[i].m_dRadius * 0.7 *
                           cos((double(fan) / 20.0) * 2.0 * g_cdPI)) + 
                          (m_pWheel[i].m_orientation.m_vUp * m_pWheel[i].m_dRadius * 0.7 *
                           sin((double(fan) / 20.0) * 2.0 * g_cdPI));
        }
    } else {
      vTmp[0] = m_pWheel[i].m_vLocation - m_pWheel[i].m_orientation.m_vRight * 0.11;
      for(fan = 0; fan < 21; ++fan) {
        vTmp[fan + 1] = m_pWheel[i].m_vLocation - 
                        m_pWheel[i].m_orientation.m_vRight * 0.11 +
                        (m_pWheel[i].m_orientation.m_vForward * m_pWheel[i].m_dRadius * 0.7 *
                         cos((double(fan) / 20.0) * 2.0 * g_cdPI)) + 
                        (m_pWheel[i].m_orientation.m_vUp * m_pWheel[i].m_dRadius * 0.7 *
                         sin((double(fan) / 20.0) * 2.0 * g_cdPI));
      }
    }
    OpenGLHelpers::TriangleFan(vTmp, 22);

    // Traction surface
    OpenGLHelpers::SetColorFull(0.2, 0.2, 0.2, 0);
    for(fan = 0; fan < 21; ++fan) {
      vTmp[fan * 2] = m_pWheel[i].m_vLocation + 
                          m_pWheel[i].m_orientation.m_vRight * 0.1 +
                          (m_pWheel[i].m_orientation.m_vForward * m_pWheel[i].m_dRadius *
                           cos((double(fan) / 20.0) * 2.0 * g_cdPI)) + 
                          (m_pWheel[i].m_orientation.m_vUp * m_pWheel[i].m_dRadius *
                           sin((double(fan) / 20.0) * 2.0 * g_cdPI));
      vTmp[fan * 2 + 1] = m_pWheel[i].m_vLocation - 
                          m_pWheel[i].m_orientation.m_vRight * 0.1 +
                          (m_pWheel[i].m_orientation.m_vForward * m_pWheel[i].m_dRadius *
                           cos((double(fan) / 20.0) * 2.0 * g_cdPI)) + 
                          (m_pWheel[i].m_orientation.m_vUp * m_pWheel[i].m_dRadius *
                           sin((double(fan) / 20.0) * 2.0 * g_cdPI));
    }
    OpenGLHelpers::TriangleStrip(vTmp, 42);

    // Grooves
    OpenGLHelpers::SetColorFull(0, 0, 0, 0);
    OpenGLHelpers::Lines(vTmp, 42);
  }

  // glDisable(GL_BLEND);
}

    /*
    OpenGLHelpers::SetColorFull(0.7, 0.2, 0.2, 0);
    vTmp[0]  = m_pPointVis[1];
    vTmp[1]  = m_pPointVis[0];
    vTmp[2]  = m_pPointVis[2];
    vTmp[3]  = m_pPointVis[8];
    vTmp[4]  = m_pPointVis[3];
    vTmp[5]  = m_pPointVis[7];
    vTmp[6]  = m_pPointVis[4];
    vTmp[7]  = m_pPointVis[6];
    vTmp[8]  = m_pPointVis[5];
    OpenGLHelpers::TriangleStrip(vTmp, 9);

    glNormal3d(orientation.m_vRight.m_dX, 
               orientation.m_vRight.m_dY,
               orientation.m_vRight.m_dZ);
    OpenGLHelpers::SetColorFull(0.7, 0.2, 0.2, 0);
    vTmp[0]  = m_pPointVis[1 + 9];
    vTmp[1]  = m_pPointVis[0 + 9];
    vTmp[2]  = m_pPointVis[2 + 9];
    vTmp[3]  = m_pPointVis[8 + 9];
    vTmp[4]  = m_pPointVis[3 + 9];
    vTmp[5]  = m_pPointVis[7 + 9];
    vTmp[6]  = m_pPointVis[4 + 9];
    vTmp[7]  = m_pPointVis[6 + 9];
    vTmp[8]  = m_pPointVis[5 + 9];
    OpenGLHelpers::TriangleStrip(vTmp, 9);

    OpenGLHelpers::SetColorFull(0.6, 0.1, 0.1, 0);
    vTmp[0]  = m_pPointVis[0];
    vTmp[1]  = m_pPointVis[9];
    vTmp[2]  = m_pPointVis[1];
    vTmp[3]  = m_pPointVis[10];
    vTmp[4]  = m_pPointVis[2];
    vTmp[5]  = m_pPointVis[11];
    vTmp[6]  = m_pPointVis[3];
    vTmp[7]  = m_pPointVis[12];
    vTmp[8]  = m_pPointVis[4];
    vTmp[9]  = m_pPointVis[13];
    vTmp[10] = m_pPointVis[5];
    vTmp[11] = m_pPointVis[14];
    vTmp[12] = m_pPointVis[6];
    vTmp[13] = m_pPointVis[15];
    vTmp[14] = m_pPointVis[7];
    vTmp[15] = m_pPointVis[16];
    OpenGLHelpers::TriangleStrip(vTmp, 16);

    OpenGLHelpers::SetColorFull(0.05, 0.05, 0.05, 0);
    vTmp[0] = m_pPointVis[7];
    vTmp[1] = m_pPointVis[16];
    vTmp[2] = m_pPointVis[8];
    vTmp[3] = m_pPointVis[17];
    vTmp[4] = m_pPointVis[0];
    vTmp[5] = m_pPointVis[9];
    OpenGLHelpers::TriangleStrip(vTmp, 6);
    */

    /*
    BOrientation orientation;
    orientation.m_vForward = (m_pPoint[0].m_vLocation - m_pPoint[7].m_vLocation);
    orientation.m_vRight   = (m_pPoint[16].m_vLocation - m_pPoint[7].m_vLocation);
    orientation.m_vUp = orientation.m_vForward.CrossProduct(orientation.m_vRight);
    orientation.Normalize();
    for(i = 0; i < m_nPoints; ++i) {
      m_pPointVis[i] = orientation.m_vRight   * m_pPointOrig[i].m_dX +
                       orientation.m_vForward * m_pPointOrig[i].m_dY -
                       orientation.m_vUp      * m_pPointOrig[i].m_dZ +
                       vCenter;
    }
    */

  // Draw wheels
  for(i = 0; i < m_nWheels; ++i) {
    if(!m_pWheel[i].m_bFixed) {
      continue;
    }
    // sides
    OpenGLHelpers::SetColorFull(0.2, 0.2, 0.2, 0);
    int fan;
    if(m_pWheel[i].m_bLeft) {
      vTmp[0] = m_pWheel[i].m_vLocation - m_pWheel[i].m_orientation.m_vRight * 0.1;
      for(fan = 0; fan < 21; ++fan) {
          vTmp[fan + 1] = m_pWheel[i].m_vLocation - 
                          m_pWheel[i].m_orientation.m_vRight * 0.1 +
                          (m_pWheel[i].m_orientation.m_vForward * m_pWheel[i].m_dRadius *
                           cos((double(fan) / 20.0) * 2.0 * g_cdPI)) + 
                          (m_pWheel[i].m_orientation.m_vUp * m_pWheel[i].m_dRadius *
                           sin((double(fan) / 20.0) * 2.0 * g_cdPI));
        }
    } else {
      vTmp[0] = m_pWheel[i].m_vLocation + m_pWheel[i].m_orientation.m_vRight * 0.1;
      for(fan = 0; fan < 21; ++fan) {
        vTmp[fan + 1] = m_pWheel[i].m_vLocation + 
                        m_pWheel[i].m_orientation.m_vRight * 0.1 +
                        (m_pWheel[i].m_orientation.m_vForward * m_pWheel[i].m_dRadius *
                         cos((double(fan) / 20.0) * 2.0 * g_cdPI)) + 
                        (m_pWheel[i].m_orientation.m_vUp * m_pWheel[i].m_dRadius *
                         sin((double(fan) / 20.0) * 2.0 * g_cdPI));
      }
    }
    OpenGLHelpers::TriangleFan(vTmp, 22);

    // Hub caps
    OpenGLHelpers::SetColorFull(0.5, 0.5, 0.7, 0);
    if(m_pWheel[i].m_bLeft) {
      vTmp[0] = m_pWheel[i].m_vLocation - m_pWheel[i].m_orientation.m_vRight * 0.11;
      for(fan = 0; fan < 21; ++fan) {
          vTmp[fan + 1] = m_pWheel[i].m_vLocation - 
                          m_pWheel[i].m_orientation.m_vRight * 0.11 +
                          (m_pWheel[i].m_orientation.m_vForward * m_pWheel[i].m_dRadius * 0.7 *
                           cos((double(fan) / 20.0) * 2.0 * g_cdPI)) + 
                          (m_pWheel[i].m_orientation.m_vUp * m_pWheel[i].m_dRadius * 0.7 *
                           sin((double(fan) / 20.0) * 2.0 * g_cdPI));
        }
    } else {
      vTmp[0] = m_pWheel[i].m_vLocation + m_pWheel[i].m_orientation.m_vRight * 0.11;
      for(fan = 0; fan < 21; ++fan) {
        vTmp[fan + 1] = m_pWheel[i].m_vLocation + 
                        m_pWheel[i].m_orientation.m_vRight * 0.11 +
                        (m_pWheel[i].m_orientation.m_vForward * m_pWheel[i].m_dRadius * 0.7 *
                         cos((double(fan) / 20.0) * 2.0 * g_cdPI)) + 
                        (m_pWheel[i].m_orientation.m_vUp * m_pWheel[i].m_dRadius * 0.7 *
                         sin((double(fan) / 20.0) * 2.0 * g_cdPI));
      }
    }
    OpenGLHelpers::TriangleFan(vTmp, 22);

    // Traction surface
    OpenGLHelpers::SetColorFull(0.2, 0.2, 0.2, 0);
    for(fan = 0; fan < 21; ++fan) {
      vTmp[fan * 2] = m_pWheel[i].m_vLocation + 
                          m_pWheel[i].m_orientation.m_vRight * 0.1 +
                          (m_pWheel[i].m_orientation.m_vForward * m_pWheel[i].m_dRadius *
                           cos((double(fan) / 20.0) * 2.0 * g_cdPI)) + 
                          (m_pWheel[i].m_orientation.m_vUp * m_pWheel[i].m_dRadius *
                           sin((double(fan) / 20.0) * 2.0 * g_cdPI));
      vTmp[fan * 2 + 1] = m_pWheel[i].m_vLocation - 
                          m_pWheel[i].m_orientation.m_vRight * 0.1 +
                          (m_pWheel[i].m_orientation.m_vForward * m_pWheel[i].m_dRadius *
                           cos((double(fan) / 20.0) * 2.0 * g_cdPI)) + 
                          (m_pWheel[i].m_orientation.m_vUp * m_pWheel[i].m_dRadius *
                           sin((double(fan) / 20.0) * 2.0 * g_cdPI));
    }
    OpenGLHelpers::TriangleStrip(vTmp, 42);

    // Grooves
    OpenGLHelpers::SetColorFull(0, 0, 0, 0);
    OpenGLHelpers::Lines(vTmp, 42);
  }


#endif

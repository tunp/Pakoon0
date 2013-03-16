//
// OpenGL helpers
//

//#include "stdafx.h"
#include "OpenGLHelpers.h"

extern bool g_cbBlackAndWhite;
extern bool g_cbMipMap;

void OpenGLHelpers::SetColorFull(double dRed, double dGreen, double dBlue, double dAlpha) {

  if(g_cbBlackAndWhite) {
    double bw;
    bw = (dRed*0.37+dGreen*0.42+dBlue*0.2);
    dRed = dGreen = dBlue = bw;
  }

  static GLfloat fCurColor[4];
  fCurColor[0] = (GLfloat) dRed;
  fCurColor[1] = (GLfloat) dGreen;
  fCurColor[2] = (GLfloat) dBlue;
  fCurColor[3] = (GLfloat) dAlpha;
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, fCurColor);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, fCurColor);
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 50.0f);
}

void OpenGLHelpers::SetColor(double dRed, double dGreen, double dBlue, double dAlpha) {
  glColor3d((GLdouble) dRed, (GLdouble) dGreen, (GLdouble) dBlue);
}



void OpenGLHelpers::Triangle(BVector& p1, BVector& p2, BVector& p3) {
  glBegin(GL_TRIANGLES);
  glVertex3d(p1.m_dX, p1.m_dY, p1.m_dZ);
  glVertex3d(p2.m_dX, p2.m_dY, p2.m_dZ);
  glVertex3d(p3.m_dX, p3.m_dY, p3.m_dZ);
  glEnd();
}

void OpenGLHelpers::TxtrTriangleBare(BVector& p1, double dTxtrX1, double dTxtrY1, 
                                     BVector& p2, double dTxtrX2, double dTxtrY2, 
                                     BVector& p3, double dTxtrX3, double dTxtrY3) {
  glTexCoord2d(dTxtrX1, dTxtrY1);
  glVertex3d(p1.m_dX, p1.m_dY, p1.m_dZ);
  glTexCoord2d(dTxtrX2, dTxtrY2);
  glVertex3d(p2.m_dX, p2.m_dY, p2.m_dZ);
  glTexCoord2d(dTxtrX3, dTxtrY3);
  glVertex3d(p3.m_dX, p3.m_dY, p3.m_dZ);
}


void OpenGLHelpers::TxtrTriangle(BVector& p1, double dTxtrX1, double dTxtrY1, 
                                 BVector& p2, double dTxtrX2, double dTxtrY2, 
                                 BVector& p3, double dTxtrX3, double dTxtrY3) {
  glBegin(GL_TRIANGLES);
  glTexCoord2d(dTxtrX1, dTxtrY1);
  glVertex3d(p1.m_dX, p1.m_dY, p1.m_dZ);
  glTexCoord2d(dTxtrX2, dTxtrY2);
  glVertex3d(p2.m_dX, p2.m_dY, p2.m_dZ);
  glTexCoord2d(dTxtrX3, dTxtrY3);
  glVertex3d(p3.m_dX, p3.m_dY, p3.m_dZ);
  glEnd();
}

void OpenGLHelpers::TriangleStrip(BVector *pvPoints, int nPoints) {
  glBegin(GL_TRIANGLE_STRIP);
  for(int i = 0; i < nPoints; ++i) {
    glVertex3d(pvPoints[i].m_dX, pvPoints[i].m_dY, pvPoints[i].m_dZ);
  }
  glEnd();
}

void OpenGLHelpers::TriangleStripWithNormals(BVector *pvPoints, BVector *pvNormals, int nPoints) {
  glBegin(GL_TRIANGLE_STRIP);
  for(int i = 0; i < nPoints; ++i) {
    if(!(i % 2)) {
      glNormal3d(pvNormals[i/2].m_dX, pvNormals[i/2].m_dY, pvNormals[i/2].m_dZ);
    }
    glVertex3d(pvPoints[i].m_dX, pvPoints[i].m_dY, pvPoints[i].m_dZ);
  }
  glEnd();
}

void OpenGLHelpers::TriangleStripTexturedX(BVector *pvPoints, 
                                           int      nPoints, 
                                           int      nOrig1, 
                                           int      nOrig2, 
                                           double   dTXTRMin, 
                                           double   dTXTRMax) {
  double dOffset1, dFactor, dOffset2;
  dOffset1 = pvPoints[nOrig1].m_dY;
  dOffset2 = pvPoints[nOrig1].m_dZ;
  dFactor  = fabs(pvPoints[nOrig1].m_dY - pvPoints[nOrig2].m_dY);
  glBegin(GL_TRIANGLE_STRIP);
  for(int i = 0; i < nPoints; ++i) {
    glTexCoord2d((pvPoints[i].m_dY - dOffset1) * dFactor, 
                 (pvPoints[i].m_dZ - dOffset2) * dFactor);
    glVertex3d(pvPoints[i].m_dX, pvPoints[i].m_dY, pvPoints[i].m_dZ);
  }
  glEnd();
}


void OpenGLHelpers::TriangleFan(BVector *pvPoints, int nPoints) {
  glBegin(GL_TRIANGLE_FAN);
  for(int i = 0; i < nPoints; ++i) {
    glVertex3d(pvPoints[i].m_dX, pvPoints[i].m_dY, pvPoints[i].m_dZ);
  }
  glEnd();
}

void OpenGLHelpers::TriangleFanWithNormals(BVector *pvPoints, BVector *pvNormals, int nPoints) {
  glBegin(GL_TRIANGLE_FAN);
  for(int i = 0; i < nPoints; ++i) {
    glNormal3d(pvNormals[i].m_dX, pvNormals[i].m_dY, pvNormals[i].m_dZ);
    glVertex3d(pvPoints[i].m_dX, pvPoints[i].m_dY, pvPoints[i].m_dZ);
  }
  glEnd();
}

void OpenGLHelpers::Line(BVector& p1, BVector& p2) {
  glBegin(GL_LINES);
  glVertex3d(p1.m_dX, p1.m_dY, p1.m_dZ);
  glVertex3d(p2.m_dX, p2.m_dY, p2.m_dZ);
  glEnd();
}

void OpenGLHelpers::Lines(BVector *pvPoints, int nPoints) {
  glBegin(GL_LINES);
  for(int i = 0; i < nPoints; ++i) {
    glVertex3d(pvPoints[i].m_dX, pvPoints[i].m_dY, pvPoints[i].m_dZ);
  }
  glEnd();
}

void OpenGLHelpers::CreateMipMaps(GLubyte *pStart, int nWidth, int nHeight, int nComponents) {
  long *fAve = new long[nComponents];
  int c, i;
  for(c = 0; c < nComponents; ++c) {
    fAve[c] = 0;
  }
  for(i = 0; i < nWidth * nHeight; ++i) {
    for(c = 0; c < nComponents; ++c) {
      fAve[c] += pStart[i * nComponents + c];
    }
  }
  for(c = 0; c < nComponents; ++c) {
    fAve[c] /= nWidth * nHeight;
  }
  for(i = 0; i < nWidth * nHeight; ++i) {
    for(c = 0; c < nComponents; ++c) {
      pStart[nWidth * nHeight * nComponents + i * nComponents + c] = GLubyte(fAve[c]);
    }
  }
  delete [] fAve;

  GLubyte *pPosOld = pStart;
  GLubyte *pPosNew = pPosOld + nWidth * nHeight * nComponents;
  int nComponent;
  int nLevel = 1;
  while(true) {
    nWidth /= 2;
    nHeight /= 2;
    if(nWidth == 0) {
      nWidth = 1;
    }
    if(nHeight == 0) {
      nHeight = 1;
    }
    for(int y = 0; y < nHeight; ++y) {
      for(int x = 0; x < nWidth; ++x) {
        for(nComponent = 0; nComponent < nComponents; ++nComponent) {
          pPosNew[y * nWidth * nComponents + x * nComponents + nComponent] = 
             (pPosOld[y     * 2 * (nWidth*2) * nComponents + x     * 2 * nComponents + nComponent] +
              pPosOld[y     * 2 * (nWidth*2) * nComponents + (x+1) * 2 * nComponents + nComponent] +
              pPosOld[(y+1) * 2 * (nWidth*2) * nComponents + x     * 2 * nComponents + nComponent] +
              pPosOld[(y+1) * 2 * (nWidth*2) * nComponents + (x+1) * 2 * nComponents + nComponent]) / 4;
        }
      }
    }
    pPosOld = pPosNew;
    pPosNew = pPosOld + nWidth * nHeight * nComponents;
    ++nLevel;
    if((nWidth == 1) && (nHeight == 1)) {
      break;
    }
  }
}



GLuint g_pNames[100];
bool   g_nBoundTextures[100] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

void OpenGLHelpers::CreateTexNames(int n) {
  glGenTextures(n, g_pNames);
}

void OpenGLHelpers::FreeTexNames(int n) {
  // Free the textures and mark them as not initialized
  glDeleteTextures(n, g_pNames);
  for(int i = 0; i < n; ++i) {
    g_nBoundTextures[i] = false;
  }
}

void OpenGLHelpers::DefineMipMapTextures(int nWidth, int nHeight, int nComponents, GLenum format, GLubyte *pStart, int nTexName) {
  if(g_nBoundTextures[nTexName]) {
    glBindTexture(GL_TEXTURE_2D, g_pNames[nTexName]);
    return;
  }

  g_nBoundTextures[nTexName] = true;
  glBindTexture(GL_TEXTURE_2D, g_pNames[nTexName]);

  int nLevel = -1;
  GLubyte *pPos = pStart;
  while(true) {
    ++nLevel;
    glTexImage2D(GL_TEXTURE_2D, nLevel, nComponents, nWidth, nHeight, 0, format, GL_UNSIGNED_BYTE, pPos);
    pPos += (nWidth * nHeight * nComponents);
    if((nWidth == 1) && (nHeight == 1)) {
      break;
    }
    nWidth /= 2;
    nHeight /= 2;
    if(nWidth == 0) {
      nWidth = 1;
    }
    if(nHeight == 0) {
      nHeight = 1;
    }

    if(!g_cbMipMap) {
      break; // if mipmapping is not used, don't define further levels
    }
  }
}

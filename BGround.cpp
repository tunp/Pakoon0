//
// Ground
//

#include "BGround.h"

#include "OpenGLHelpers.h"

#include <cstdio>
#include <cstdlib>

#include <iostream>

using namespace std;

const double dPI = 3.141592654;
extern bool g_cbBlackAndWhite;

BGround::BGround() {

  // Load ground height map
  LoadGroundFromFile("./World/ground.raw");

  // Load stationary objects
  LoadObjectsFromFile("./World/objects.dat");

  // Load ground textures
  // LoadTextures();

  m_dCoarseDistance = 16000.0;
}

BGround::~BGround() {
  if(m_pObjects) {
    delete [] m_pObjects;
  }
}



extern double Random(double dRange);


void BGround::LoadObjectsFromFile(std::string sFilename) {
  m_nObjects = 2;
  m_pObjects = new BObject[2];
  m_pObjects[0].m_dRed = 0.3;
  m_pObjects[0].m_dGreen = 0.1;
  m_pObjects[0].m_dBlue = 0.12;
  m_pObjects[0].m_nFaces = 5;
  m_pObjects[0].m_dFriction = 0.5;
  m_pObjects[0].m_vOrigin = BVector(40.0 * g_cdGroundScale, 4.0 * g_cdGroundScale, 0.0);

  m_pObjects[0].m_face[0].m_nPoints = 4;
  m_pObjects[0].m_face[0].m_vPoint[0].Set(0, 0, 0.4);
  m_pObjects[0].m_face[0].m_vPoint[1].Set(0, 0, 0.0);
  m_pObjects[0].m_face[0].m_vPoint[2].Set(1, 0, -1);
  m_pObjects[0].m_face[0].m_vPoint[3].Set(1, 0, -0.6);

  m_pObjects[0].m_face[1].m_nPoints = 4;
  m_pObjects[0].m_face[1].m_vPoint[0].Set(1, 1, -0.6);
  m_pObjects[0].m_face[1].m_vPoint[1].Set(1, 1, -1);
  m_pObjects[0].m_face[1].m_vPoint[2].Set(0, 1, 0.0);
  m_pObjects[0].m_face[1].m_vPoint[3].Set(0, 1, 0.4);

  m_pObjects[0].m_face[2].m_nPoints = 4;
  m_pObjects[0].m_face[2].m_vPoint[0].Set(1, 0, -0.6);
  m_pObjects[0].m_face[2].m_vPoint[1].Set(1, 0, -1);
  m_pObjects[0].m_face[2].m_vPoint[2].Set(1, 1, -1);
  m_pObjects[0].m_face[2].m_vPoint[3].Set(1, 1, -0.6);

  m_pObjects[0].m_face[3].m_nPoints = 4;
  m_pObjects[0].m_face[3].m_vPoint[0].Set(0, 0, 0);
  m_pObjects[0].m_face[3].m_vPoint[1].Set(0, 1, 0);
  m_pObjects[0].m_face[3].m_vPoint[2].Set(1, 1, -1);
  m_pObjects[0].m_face[3].m_vPoint[3].Set(1, 0, -1);

  m_pObjects[0].m_face[4].m_nPoints = 4;
  m_pObjects[0].m_face[4].m_vPoint[0].Set(1, 0, -0.6);
  m_pObjects[0].m_face[4].m_vPoint[1].Set(1, 1, -0.6);
  m_pObjects[0].m_face[4].m_vPoint[2].Set(0, 1, 0.4);
  m_pObjects[0].m_face[4].m_vPoint[3].Set(0, 0, 0.4);

  m_pObjects[0].m_vScale.Set(100.0, 30.0, 30.0);
  m_pObjects[0].Setup();

  m_pObjects[1].m_dRed = 0.3;
  m_pObjects[1].m_dGreen = 0.1;
  m_pObjects[1].m_dBlue = 0.12;
  m_pObjects[1].m_nFaces = 5;
  m_pObjects[1].m_dFriction = 0.5;
  m_pObjects[1].m_vOrigin = BVector(51.3 * g_cdGroundScale, 4.0 * g_cdGroundScale, 0.0);

  m_pObjects[1].m_face[0].m_nPoints = 4;
  m_pObjects[1].m_face[0].m_vPoint[0].Set(1, 0, -0.6);
  m_pObjects[1].m_face[0].m_vPoint[1].Set(1, 0, -1);
  m_pObjects[1].m_face[0].m_vPoint[2].Set(0, 0, 0.0);
  m_pObjects[1].m_face[0].m_vPoint[3].Set(0, 0, 0.4);

  m_pObjects[1].m_face[1].m_nPoints = 4;
  m_pObjects[1].m_face[1].m_vPoint[0].Set(0, 1, 0.4);
  m_pObjects[1].m_face[1].m_vPoint[1].Set(0, 1, 0.0);
  m_pObjects[1].m_face[1].m_vPoint[2].Set(1, 1, -1);
  m_pObjects[1].m_face[1].m_vPoint[3].Set(1, 1, -0.6);

  m_pObjects[1].m_face[2].m_nPoints = 4;
  m_pObjects[1].m_face[2].m_vPoint[0].Set(1, 1, -0.6);
  m_pObjects[1].m_face[2].m_vPoint[1].Set(1, 1, -1);
  m_pObjects[1].m_face[2].m_vPoint[2].Set(1, 0, -1);
  m_pObjects[1].m_face[2].m_vPoint[3].Set(1, 0, -0.6);

  m_pObjects[1].m_face[3].m_nPoints = 4;
  m_pObjects[1].m_face[3].m_vPoint[0].Set(1, 0, -1);
  m_pObjects[1].m_face[3].m_vPoint[1].Set(1, 1, -1);
  m_pObjects[1].m_face[3].m_vPoint[2].Set(0, 1, 0);
  m_pObjects[1].m_face[3].m_vPoint[3].Set(0, 0, 0);

  m_pObjects[1].m_face[4].m_nPoints = 4;
  m_pObjects[1].m_face[4].m_vPoint[0].Set(0, 0, 0.4);
  m_pObjects[1].m_face[4].m_vPoint[1].Set(0, 1, 0.4);
  m_pObjects[1].m_face[4].m_vPoint[2].Set(1, 1, -0.6);
  m_pObjects[1].m_face[4].m_vPoint[3].Set(1, 0, -0.6);

  m_pObjects[1].m_vScale.Set(-100.0, 30.0, 30.0);
  m_pObjects[1].Setup();
} // LoadObjectsFromFile


void BGround::RenderSky(int nDispList) {
  glDisable(GL_BLEND);
  GLfloat fLight1DiffuseG[ 4];
  GLfloat fLight1SpecularG[ 4];
  fLight1DiffuseG[0] = 0.0f;
  fLight1DiffuseG[1] = 0.0f;
  fLight1DiffuseG[2] = 0.0f;
  fLight1DiffuseG[3] = 0.0f;
  fLight1SpecularG[0] = 0.0f;
  fLight1SpecularG[1] = 0.0f;
  fLight1SpecularG[2] = 0.0f;
  fLight1SpecularG[3] = 0.0f;
  glLightfv(GL_LIGHT0, GL_DIFFUSE,  fLight1DiffuseG);
  glLightfv(GL_LIGHT0, GL_SPECULAR,  fLight1SpecularG);
  glShadeModel(GL_SMOOTH);
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_CULL_FACE);
  // sky
  OpenGLHelpers::SetColorFull(1, 1, 1, 1);
  if(nDispList == DISPLIST_SKYCOARSE) {
    OpenGLHelpers::DefineMipMapTextures(8, 8, 3, GL_RGB, m_pTxtrSky, 2);
  } else if(nDispList == DISPLIST_SKYDETAILED) {
    OpenGLHelpers::DefineMipMapTextures(512, 128, 3, GL_RGB, m_pTxtrSkyDetailed, 3);
  }
  for(int y = 0; y < 9; ++y) {
    glBegin(GL_TRIANGLE_STRIP);
    for(int x = 0; x < 21; ++x) {
      if(y == 0) {
        if(nDispList == DISPLIST_SKYCOARSE) {
          glTexCoord2d(double(x) / 20.0, 1.0 / 8.0);
        } else if(nDispList == DISPLIST_SKYDETAILED) {
          glTexCoord2d(double(x) / 20.0, 1.0 / 128.0);
        }
      } else {
        glTexCoord2d(double(x) / 20.0, double(y) / 10.0);
      }
      glVertex3d(cos(double(x) / 20.0 * 2.0 * dPI) * cos(double(y) / 10.0 * dPI / 2.0) * 10000.0, 
                 sin(double(x) / 20.0 * 2.0 * dPI) * cos(double(y) / 10.0 * dPI / 2.0) * 10000.0, 
                 sin(double(y) / 10.0 * dPI / 2.0) * -10000.0);
      glTexCoord2d(double(x) / 20.0, double(y+1) / 10.0);
      glVertex3d(cos(double(x) / 20.0 * 2.0 * dPI) * cos(double(y+1) / 10.0 * dPI / 2.0) * 10000.0, 
                 sin(double(x) / 20.0 * 2.0 * dPI) * cos(double(y+1) / 10.0 * dPI / 2.0) * 10000.0, 
                 sin(double(y+1) / 10.0 * dPI / 2.0) * -10000.0);
    }
    glEnd();
  }
  glCullFace(GL_FRONT);
  glBegin(GL_TRIANGLE_FAN);
  if(nDispList == DISPLIST_SKYCOARSE) {
    glTexCoord2d(0.5, 7.0/8.0);
  } else if(nDispList == DISPLIST_SKYDETAILED) {
    glTexCoord2d(0.5, 127.0/128.0);
  }
  glVertex3d(0.0, 0.0, -10000.0);
  int x;
  for(x = 0; x < 21; ++x) {
        glTexCoord2d(double(x) / 20.0, 9.0 / 10.0);
        glVertex3d(cos(double(x) / 20.0 * 2.0 * dPI) * cos(9.0 / 10.0 * dPI / 2.0) * 10000.0, 
                   sin(double(x) / 20.0 * 2.0 * dPI) * cos(9.0 / 10.0 * dPI / 2.0) * 10000.0, 
                   sin(9.0 / 10.0 * dPI / 2.0) * -10000.0);
  }
  glEnd();
  glCullFace(GL_BACK);
  glDisable(GL_CULL_FACE);  
  glDisable(GL_TEXTURE_2D);
  fLight1DiffuseG[0] = 0.8f;
  fLight1DiffuseG[1] = 0.8f;
  fLight1DiffuseG[2] = 0.8f;
  fLight1DiffuseG[3] = 0.8f;
  fLight1SpecularG[0] = 0.5f;
  fLight1SpecularG[1] = 0.5f;
  fLight1SpecularG[2] = 0.5f;
  fLight1SpecularG[3] = 0.5f;
  glLightfv(GL_LIGHT0, GL_DIFFUSE,  fLight1DiffuseG);
  glLightfv(GL_LIGHT0, GL_SPECULAR,  fLight1SpecularG);
}


//*****************************************************************************
void BGround::PreProcessVisualization() {
  // Generate ground as a opengl display list to speed up the visualization
  CreateGroundAreas();

  // Generate skies and ocean as a opengl display list to speed up the visualization

  OpenGLHelpers::DefineMipMapTextures(8, 8, 3, GL_RGB, m_pTxtrSky, 2);
  OpenGLHelpers::DefineMipMapTextures(512, 128, 3, GL_RGB, m_pTxtrSkyDetailed, 3);

  glNewList(DISPLIST_SKYDETAILED, GL_COMPILE);
  RenderSky(DISPLIST_SKYDETAILED);
  glEndList();
  glNewList(DISPLIST_SKYCOARSE, GL_COMPILE);
  RenderSky(DISPLIST_SKYCOARSE);
  glEndList();

  // Sea bed
  OpenGLHelpers::DefineMipMapTextures(8, 8, 3, GL_RGB, m_pTxtrOcean, 4);
  glNewList(DISPLIST_SEABED, GL_COMPILE);
  glEnable(GL_TEXTURE_2D);
  glShadeModel(GL_SMOOTH);
  OpenGLHelpers::SetColorFull(1, 1, 1, 1);
  OpenGLHelpers::DefineMipMapTextures(8, 8, 3, GL_RGB, m_pTxtrOcean, 4);
  glBegin(GL_TRIANGLE_FAN);
  glTexCoord2d(0.5, 1.0 / 8.0);
  glVertex3d(0.0, 0.0, 500.0);
  for(int x = 0; x < 21; ++x) {
        glTexCoord2d(double(x) / 20.0, 7.5 / 8.0);
        glVertex3d(cos(double(x) / 20.0 * 2.0 * dPI) * 10000.0, 
                   sin(double(x) / 20.0 * 2.0 * dPI) * 10000.0, 
                   0.0);
  }
  glEnd();
  glEndList();

  // Water surface
  glNewList(DISPLIST_WATERSURFACE, GL_COMPILE);
  glDisable(GL_CULL_FACE);
  glCallList(DISPLIST_GROUNDWATER);
  glEnable(GL_BLEND);
  glNormal3d(0, 0, -1);
  glBegin(GL_TRIANGLE_STRIP);
  glVertex3d(0, -10000.0, g_cdWaterLevel);
  glVertex3d(0,  10000.0, g_cdWaterLevel);
  glVertex3d(-10000.0, -10000.0, g_cdWaterLevel);
  glVertex3d(-10000.0,  10000.0, g_cdWaterLevel);
  glEnd();
  glBegin(GL_TRIANGLE_STRIP);
  glVertex3d(0, 0.0, g_cdWaterLevel);
  glVertex3d(10000.0,  0.0, g_cdWaterLevel);
  glVertex3d(0.0, -10000.0, g_cdWaterLevel);
  glVertex3d(10000.0, -10000.0, g_cdWaterLevel);
  glEnd();
  glBegin(GL_TRIANGLE_STRIP);
  glVertex3d(0, g_cnGroundYSize * g_cdGroundScale, g_cdWaterLevel);
  glVertex3d(10000.0,  g_cnGroundYSize * g_cdGroundScale, g_cdWaterLevel);
  glVertex3d(0.0, 10000.0, g_cdWaterLevel);
  glVertex3d(10000.0, 10000.0, g_cdWaterLevel);
  glEnd();
  glBegin(GL_TRIANGLE_STRIP);
  glVertex3d(g_cnGroundXSize * g_cdGroundScale, 0.0, g_cdWaterLevel);
  glVertex3d(g_cnGroundXSize * g_cdGroundScale,  10000.0, g_cdWaterLevel);
  glVertex3d(10000.0, 0.0, g_cdWaterLevel);
  glVertex3d(10000.0,  10000.0, g_cdWaterLevel);
  glEnd();
  glDisable(GL_BLEND);
  glEndList();

  // Objects
  glNewList(DISPLIST_OBJECTS, GL_COMPILE);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_FRONT);
  glCallList(DISPLIST_GROUNDSAND);
  for(int i = 0; i < m_nObjects; ++i) {
    OpenGLHelpers::SetColorFull(m_pObjects[i].m_dRed, 
                                m_pObjects[i].m_dGreen, 
                                m_pObjects[i].m_dBlue, 
                                1);
    for(int nFace = 0; nFace < m_pObjects[i].m_nFaces; ++nFace) {
      glNormal3d(m_pObjects[i].m_face[nFace].m_vNormal.m_dX,
                 m_pObjects[i].m_face[nFace].m_vNormal.m_dY,
                 m_pObjects[i].m_face[nFace].m_vNormal.m_dZ);
      glBegin(GL_POLYGON);
      for(int nPoint = 0; nPoint < m_pObjects[i].m_face[nFace].m_nPoints; ++nPoint) {
        glVertex3d(m_pObjects[i].m_face[nFace].m_vPoint[nPoint].m_dX,
                   m_pObjects[i].m_face[nFace].m_vPoint[nPoint].m_dY,
                   m_pObjects[i].m_face[nFace].m_vPoint[nPoint].m_dZ);
      }
      glEnd();
    }
  }
  glDisable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glEndList();
}


//*****************************************************************************
void BGround::Paint(int nSkyDetail, int nWaterDetail, BCamera &rCamera, TGroundType gt) {
  if(gt == GT_GROUND) {
    glDisable(GL_DEPTH_TEST);
    // Render seabed
    glCallList(DISPLIST_SEABED);

    // Render water 
    if(nWaterDetail == 1) {
      glCallList(DISPLIST_WATERSURFACE);
    }

    // Render sky
    if(nSkyDetail == 1) {
      glCallList(DISPLIST_SKYCOARSE);
    } else if(nSkyDetail == 2) {
      glCallList(DISPLIST_SKYDETAILED);
    }
    glEnable(GL_DEPTH_TEST);
  }
  
  // Render ground
  PaintGroundAreas(rCamera, gt);

  // Render Objects
  glCallList(DISPLIST_OBJECTS);
}




//*****************************************************************************
void BGround::CreateGroundAreas() {

  OpenGLHelpers::DefineMipMapTextures(64, 64, 3, GL_RGB, m_dTexture, 5);
  glNewList(DISPLIST_GROUNDSAND, GL_COMPILE);
  glEnable(GL_CULL_FACE);  
  glEnable(GL_TEXTURE_2D);
  glDisable(GL_BLEND);
  OpenGLHelpers::SetColorFull(0.78, 0.58, 0.38, 0);
  OpenGLHelpers::DefineMipMapTextures(64, 64, 3, GL_RGB, m_dTexture, 5);
  glShadeModel(GL_SMOOTH);
  GLfloat fLight[4];
  fLight[0] = 0.0f;
  fLight[1] = 0.0f;
  fLight[2] = 0.0f;
  fLight[3] = 0.0f;
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, fLight);
  glEndList();

  OpenGLHelpers::DefineMipMapTextures(128, 128, 3, GL_RGB, m_dMonVal, 6);
  glNewList(DISPLIST_GROUNDROCK, GL_COMPILE);
  glEnable(GL_CULL_FACE);  
  glEnable(GL_TEXTURE_2D);
  glDisable(GL_BLEND);
  OpenGLHelpers::SetColorFull(1.0, 0.9, 0.7, 0);
  OpenGLHelpers::DefineMipMapTextures(128, 128, 3, GL_RGB, m_dMonVal, 6);
  glShadeModel(GL_SMOOTH);
  glEndList();

  glNewList(DISPLIST_GROUNDWATER, GL_COMPILE);
  glDisable(GL_CULL_FACE);  
  glDisable(GL_TEXTURE_2D);
  glEnable(GL_BLEND);
  glBlendFunc(GL_ONE, GL_SRC_COLOR);
  OpenGLHelpers::SetColorFull(0.15, 0.25, 0.35, 0);
  glShadeModel(GL_SMOOTH);
  glEndList();

  int nList = 0, x, y;
  for(y = 0; y < g_cnGroundYSize / g_cnAreaSize; ++y) {
    for(x = 0; x < g_cnGroundXSize / g_cnAreaSize; ++x) {
      m_areas[nList].m_nDLFullRes1 = DISPLIST_GROUNDFULLRESBASE1 + nList;
      m_areas[nList].m_nDLFullRes2 = DISPLIST_GROUNDFULLRESBASE2 + nList;
      m_areas[nList].m_nDLFullRes3 = DISPLIST_GROUNDFULLRESBASE3 + nList;
      m_areas[nList].m_nDLCoarseRes1 = DISPLIST_GROUNDCOARSERESBASE1 + nList;
      m_areas[nList].m_nDLCoarseRes2 = DISPLIST_GROUNDCOARSERESBASE2 + nList;
      m_areas[nList].m_nDLCoarseRes3 = DISPLIST_GROUNDCOARSERESBASE3 + nList;
      m_areas[nList].m_vCenter = BVector(double(x * g_cnAreaSize) * g_cdGroundScale + (g_cnAreaSize * g_cdGroundScale) / 2.0, 
                                         double(y * g_cnAreaSize) * g_cdGroundScale + (g_cnAreaSize * g_cdGroundScale) / 2.0,
                                         0.0);

      glNewList(m_areas[nList].m_nDLFullRes1, GL_COMPILE);
      RenderAreaFull(x * g_cnAreaSize, y * g_cnAreaSize, g_cnAreaSize, GT_SAND);
      glEndList();
      glNewList(m_areas[nList].m_nDLFullRes2, GL_COMPILE);
      RenderAreaFull(x * g_cnAreaSize, y * g_cnAreaSize, g_cnAreaSize, GT_ROCK);
      glEndList();
      glNewList(m_areas[nList].m_nDLFullRes3, GL_COMPILE);
      RenderAreaFull(x * g_cnAreaSize, y * g_cnAreaSize, g_cnAreaSize, GT_WATER);
      glEndList();
      ++nList;
    }
  }

  nList = 0;
  for(y = 0; y < g_cnGroundYSize / g_cnAreaSize; ++y) {
    for(x = 0; x < g_cnGroundXSize / g_cnAreaSize; ++x) {
      glNewList(m_areas[nList].m_nDLCoarseRes1, GL_COMPILE);
      RenderAreaCoarse(x * g_cnAreaSize, y * g_cnAreaSize, g_cnAreaSize, GT_SAND);
      glEndList();
      glNewList(m_areas[nList].m_nDLCoarseRes2, GL_COMPILE);
      RenderAreaCoarse(x * g_cnAreaSize, y * g_cnAreaSize, g_cnAreaSize, GT_ROCK);
      glEndList();
      glNewList(m_areas[nList].m_nDLCoarseRes3, GL_COMPILE);
      RenderAreaCoarse(x * g_cnAreaSize, y * g_cnAreaSize, g_cnAreaSize, GT_WATER);
      glEndList();
      ++nList;
    }
  }


}


bool BGround::GroundTriangleIsOfType(int x, int y, int nSize, int nTriangle, TGroundType gt) {
  BIndexGuard safe(g_cnGroundXSize * g_cnGroundYSize - 1, 0);
  BVector vNormal;
  double d1, d2, d3;
  if(nTriangle == 1) {
    vNormal = m_sqrHeightMapNormals[safe(y * g_cnGroundXSize + x)].m_vNormal1;
    d1 = m_dHeightMap[safe(y * g_cnGroundXSize + x)];
    d2 = m_dHeightMap[safe(y * g_cnGroundXSize + x+nSize)];
    d3 = m_dHeightMap[safe((y+nSize) * g_cnGroundXSize + x+nSize)];
  } else {
    vNormal = m_sqrHeightMapNormals[safe(y * g_cnGroundXSize + x)].m_vNormal2;
    d1 = m_dHeightMap[safe(y * g_cnGroundXSize + x)];
    d2 = m_dHeightMap[safe((y+nSize) * g_cnGroundXSize + x)];
    d3 = m_dHeightMap[safe((y+nSize) * g_cnGroundXSize + x+nSize)];
  }

  switch(gt) {
    case GT_SAND:
      return (vNormal.m_dZ < -0.5) && !GroundTriangleIsOfType(x, y, nSize, nTriangle, GT_WATER);
      break;
    case GT_ROCK:
      return (vNormal.m_dZ >= -0.5) && !GroundTriangleIsOfType(x, y, nSize, nTriangle, GT_WATER);
      break;
    case GT_WATER:
      return (d1 > g_cdWaterLevel) || (d2 > g_cdWaterLevel) || (d3 > g_cdWaterLevel);
      break;
  }
  return false;
}



double BGround::SurfaceHeightAt(int x, int y, TGroundType gt) {
  BIndexGuard safe(g_cnGroundXSize * g_cnGroundYSize - 1, 0);
  if(gt == GT_WATER) {
    return g_cdWaterLevel;
  } else {
    return m_dHeightMap[safe(y * g_cnGroundXSize + x)];
  }
}


void BGround::RenderAreaFull(int nX, int nY, int nSize, TGroundType gt) {
  BIndexGuard safe(g_cnGroundXSize * g_cnGroundYSize - 1, 0);
  double dBase;
  double dScale;
  glBegin(GL_TRIANGLES);
  for(int x = nX; x < nX + nSize; ++x) {
    for(int y = nY; y < nY + nSize; ++y) {
      if(GroundTriangleIsOfType(x, y, 1, 1, gt)) {
        dBase  = Random(1.0);
        dScale = 0.5 + Random(1.0);
        if(gt == GT_ROCK) {
          glTexCoord2d(dBase + dScale * (x+y), dBase + fabs(m_dHeightMap[safe(y * g_cnGroundXSize + x)] / g_cdGroundScale * dScale));
        } else if(gt == GT_SAND) {
          glTexCoord2d(x * 0.3, y * 0.3);
        }
        glNormal3d(m_sqrHeightMapNormals[safe(y * g_cnGroundXSize + x)].m_vNormalVis.m_dX,
                   m_sqrHeightMapNormals[safe(y * g_cnGroundXSize + x)].m_vNormalVis.m_dY,
                   m_sqrHeightMapNormals[safe(y * g_cnGroundXSize + x)].m_vNormalVis.m_dZ);
        glVertex3d(x * g_cdGroundScale, 
                   y * g_cdGroundScale, 
                   SurfaceHeightAt(x, y, gt));

        if(gt == GT_ROCK) {
          glTexCoord2d(dBase + dScale * (x+y+1), dBase + fabs(m_dHeightMap[safe(y * g_cnGroundXSize + x+1)] / g_cdGroundScale * dScale));
        } else if(gt == GT_SAND) {
          glTexCoord2d((x+1) * 0.3, y * 0.3);
        }
        glNormal3d(m_sqrHeightMapNormals[safe(y * g_cnGroundXSize + x+1)].m_vNormalVis.m_dX,
                   m_sqrHeightMapNormals[safe(y * g_cnGroundXSize + x+1)].m_vNormalVis.m_dY,
                   m_sqrHeightMapNormals[safe(y * g_cnGroundXSize + x+1)].m_vNormalVis.m_dZ);
        glVertex3d((x+1) * g_cdGroundScale, 
                   y * g_cdGroundScale, 
                   SurfaceHeightAt(x+1, y, gt));

        if(gt == GT_ROCK) {
          glTexCoord2d(dBase + dScale * (x+y+2), dBase + fabs(m_dHeightMap[safe((y+1) * g_cnGroundXSize + x+1)] / g_cdGroundScale * dScale));
        } else if(gt == GT_SAND) {
          glTexCoord2d((x+1) * 0.3, (y+1) * 0.3);
        }
        glNormal3d(m_sqrHeightMapNormals[safe((y+1) * g_cnGroundXSize + x+1)].m_vNormalVis.m_dX,
                   m_sqrHeightMapNormals[safe((y+1) * g_cnGroundXSize + x+1)].m_vNormalVis.m_dY,
                   m_sqrHeightMapNormals[safe((y+1) * g_cnGroundXSize + x+1)].m_vNormalVis.m_dZ);
        glVertex3d((x+1) * g_cdGroundScale, 
                   (y+1) * g_cdGroundScale, 
                   SurfaceHeightAt(x+1, y+1, gt));

        switch(gt) {
          case GT_SAND:
            m_sqrHeightMapNormals[safe(y * g_cnGroundXSize + x)].m_dFriction1 = 0.5;
            m_sqrHeightMapNormals[safe(y * g_cnGroundXSize + x)].m_dSurfaceDepth1 = 0.03;
            break;
          case GT_ROCK:
            m_sqrHeightMapNormals[safe(y * g_cnGroundXSize + x)].m_dFriction1 = 0.9;
            m_sqrHeightMapNormals[safe(y * g_cnGroundXSize + x)].m_dSurfaceDepth1 = 0.0;
            break;
          case GT_WATER:
            m_sqrHeightMapNormals[safe(y * g_cnGroundXSize + x)].m_dFriction1 = 0.2;
            m_sqrHeightMapNormals[safe(y * g_cnGroundXSize + x)].m_dSurfaceDepth1 = 0.0;
            m_sqrHeightMapNormals[safe(y * g_cnGroundXSize + x)].m_bWater1 = true;
            break;
        }
      }
      if(GroundTriangleIsOfType(x, y, 1, 2, gt)) {
        dBase  = Random(1.0);
        dScale = 0.5 + Random(1.0);
        if(gt == GT_ROCK) {
          glTexCoord2d(dBase + dScale * (x+y), dBase + fabs(m_dHeightMap[safe(y * g_cnGroundXSize + x)] / g_cdGroundScale * dScale));
        } else if(gt == GT_SAND) {
          glTexCoord2d(x * 0.3, y * 0.3);
        }
        glNormal3d(m_sqrHeightMapNormals[safe(y * g_cnGroundXSize + x)].m_vNormalVis.m_dX,
                   m_sqrHeightMapNormals[safe(y * g_cnGroundXSize + x)].m_vNormalVis.m_dY,
                   m_sqrHeightMapNormals[safe(y * g_cnGroundXSize + x)].m_vNormalVis.m_dZ);
        glVertex3d(x * g_cdGroundScale, 
                   y * g_cdGroundScale,
                   SurfaceHeightAt(x, y, gt));

        if(gt == GT_ROCK) {
          glTexCoord2d(dBase + dScale * (x+y+2), dBase + fabs(m_dHeightMap[safe((y+1) * g_cnGroundXSize + x+1)] / g_cdGroundScale * dScale));
        } else if(gt == GT_SAND) {
          glTexCoord2d((x+1) * 0.3, (y+1) * 0.3);
        }
        glNormal3d(m_sqrHeightMapNormals[safe((y+1) * g_cnGroundXSize + x+1)].m_vNormalVis.m_dX,
                   m_sqrHeightMapNormals[safe((y+1) * g_cnGroundXSize + x+1)].m_vNormalVis.m_dY,
                   m_sqrHeightMapNormals[safe((y+1) * g_cnGroundXSize + x+1)].m_vNormalVis.m_dZ);
        glVertex3d((x+1) * g_cdGroundScale, 
                   (y+1) * g_cdGroundScale, 
                   SurfaceHeightAt(x+1, y+1, gt));

        if(gt == GT_ROCK) {
          glTexCoord2d(dBase + dScale * (x+y+1), dBase + fabs(m_dHeightMap[safe((y+1) * g_cnGroundXSize + x)] / g_cdGroundScale * dScale));
        } else if(gt == GT_SAND) {
          glTexCoord2d(x * 0.3, (y+1) * 0.3);
        }
        glNormal3d(m_sqrHeightMapNormals[safe((y+1) * g_cnGroundXSize + x)].m_vNormalVis.m_dX,
                   m_sqrHeightMapNormals[safe((y+1) * g_cnGroundXSize + x)].m_vNormalVis.m_dY,
                   m_sqrHeightMapNormals[safe((y+1) * g_cnGroundXSize + x)].m_vNormalVis.m_dZ);
        glVertex3d(x * g_cdGroundScale, 
                   (y+1) * g_cdGroundScale, 
                   SurfaceHeightAt(x, y+1, gt));

        switch(gt) {
          case GT_SAND:
            m_sqrHeightMapNormals[safe(y * g_cnGroundXSize + x)].m_dFriction2 = 0.5;
            m_sqrHeightMapNormals[safe(y * g_cnGroundXSize + x)].m_dSurfaceDepth2 = 0.03;
            break;
          case GT_ROCK:
            m_sqrHeightMapNormals[safe(y * g_cnGroundXSize + x)].m_dFriction2 = 0.9;
            m_sqrHeightMapNormals[safe(y * g_cnGroundXSize + x)].m_dSurfaceDepth2 = 0.0;
            break;
          case GT_WATER:
            m_sqrHeightMapNormals[safe(y * g_cnGroundXSize + x)].m_dFriction2 = 0.2;
            m_sqrHeightMapNormals[safe(y * g_cnGroundXSize + x)].m_dSurfaceDepth2 = 0.0;
            m_sqrHeightMapNormals[safe(y * g_cnGroundXSize + x)].m_bWater2 = true;
            break;
        }
      }
    }
  }
  glEnd();
}


void BGround::RenderAreaCoarse(int nX, int nY, int nSize, TGroundType gt){
  double dBase;
  double dScale;
  BIndexGuard safe(g_cnGroundXSize * g_cnGroundYSize - 1, 0);
  if(AreaFlatEnough(nX, nY, nSize)) {
    // Render are using corner points
    int x = nX;
    int y = nY;
    glBegin(GL_TRIANGLES);
    if(GroundTriangleIsOfType(x, y, nSize, 1, gt)) {
      dBase  = Random(1.0);
      dScale = 0.5 + Random(1.0) * double(nSize);
      if(gt == GT_ROCK) {
        glTexCoord2d(dBase + dScale * (x+y), dBase + fabs(m_dHeightMap[safe(y * g_cnGroundXSize + x)] / g_cdGroundScale * dScale));
      } else if(gt == GT_SAND) {
        glTexCoord2d(x * 0.3 * nSize, y * 0.3 * nSize);
      }
      glNormal3d(m_sqrHeightMapNormals[safe(y * g_cnGroundXSize + x)].m_vNormalVis.m_dX,
                 m_sqrHeightMapNormals[safe(y * g_cnGroundXSize + x)].m_vNormalVis.m_dY,
                 m_sqrHeightMapNormals[safe(y * g_cnGroundXSize + x)].m_vNormalVis.m_dZ);
      glVertex3d(x * g_cdGroundScale, y * g_cdGroundScale, m_dHeightMap[safe(y * g_cnGroundXSize + x)]);

      if(gt == GT_ROCK) {
        glTexCoord2d(dBase + dScale * (x+y+nSize), dBase + fabs(m_dHeightMap[safe(y * g_cnGroundXSize + (x+nSize))] / g_cdGroundScale * dScale));
      } else if(gt == GT_SAND) {
        glTexCoord2d((x+nSize) * 0.3 * nSize, y * 0.3 * nSize);
      }
      glNormal3d(m_sqrHeightMapNormals[safe(y * g_cnGroundXSize + x+nSize)].m_vNormalVis.m_dX,
                 m_sqrHeightMapNormals[safe(y * g_cnGroundXSize + x+nSize)].m_vNormalVis.m_dY,
                 m_sqrHeightMapNormals[safe(y * g_cnGroundXSize + x+nSize)].m_vNormalVis.m_dZ);
      glVertex3d((x+nSize) * g_cdGroundScale, y * g_cdGroundScale, m_dHeightMap[safe(y * g_cnGroundXSize + (x+nSize))]);

      if(gt == GT_ROCK) {
        glTexCoord2d(dBase + dScale * (x+y+2*nSize), dBase + fabs(m_dHeightMap[safe((y+nSize) * g_cnGroundXSize + (x+nSize))] / g_cdGroundScale * dScale));
      } else if(gt == GT_SAND) {
        glTexCoord2d((x+nSize) * 0.3 * nSize, (y+nSize) * 0.3 * nSize);
      }
      glNormal3d(m_sqrHeightMapNormals[safe((y+nSize) * g_cnGroundXSize + x+nSize)].m_vNormalVis.m_dX,
                 m_sqrHeightMapNormals[safe((y+nSize) * g_cnGroundXSize + x+nSize)].m_vNormalVis.m_dY,
                 m_sqrHeightMapNormals[safe((y+nSize) * g_cnGroundXSize + x+nSize)].m_vNormalVis.m_dZ);
      glVertex3d((x+nSize) * g_cdGroundScale, (y+nSize) * g_cdGroundScale, m_dHeightMap[safe((y+nSize) * g_cnGroundXSize + (x+nSize))]);
    } else {
    // if(GroundTriangleIsOfType(x, y, nSize, 2, gt)) {
      dBase  = Random(1.0);
      dScale = 0.5 + Random(1.0) * double(nSize);
      if(gt == GT_ROCK) {
        glTexCoord2d(dBase + dScale * (x+y), dBase + fabs(m_dHeightMap[safe(y * g_cnGroundXSize + x)] / g_cdGroundScale * dScale));
      } else if(gt == GT_SAND) {
        glTexCoord2d(x * 0.3 * nSize, y * 0.3 * nSize);
      }
      glNormal3d(m_sqrHeightMapNormals[safe(y * g_cnGroundXSize + x)].m_vNormalVis.m_dX,
                 m_sqrHeightMapNormals[safe(y * g_cnGroundXSize + x)].m_vNormalVis.m_dY,
                 m_sqrHeightMapNormals[safe(y * g_cnGroundXSize + x)].m_vNormalVis.m_dZ);
      glVertex3d(x * g_cdGroundScale, y * g_cdGroundScale, m_dHeightMap[safe(y * g_cnGroundXSize + x)]);

      if(gt == GT_ROCK) {
        glTexCoord2d(dBase + dScale * (x+y+2*nSize), dBase + fabs(m_dHeightMap[safe((y+nSize) * g_cnGroundXSize + (x+nSize))] / g_cdGroundScale * dScale));
      } else if(gt == GT_SAND) {
        glTexCoord2d((x+nSize) * 0.3 * nSize, (y+nSize) * 0.3 * nSize);
      }
      glNormal3d(m_sqrHeightMapNormals[safe((y+nSize) * g_cnGroundXSize + x+nSize)].m_vNormalVis.m_dX,
                 m_sqrHeightMapNormals[safe((y+nSize) * g_cnGroundXSize + x+nSize)].m_vNormalVis.m_dY,
                 m_sqrHeightMapNormals[safe((y+nSize) * g_cnGroundXSize + x+nSize)].m_vNormalVis.m_dZ);
      glVertex3d((x+nSize) * g_cdGroundScale, (y+nSize) * g_cdGroundScale, m_dHeightMap[safe((y+nSize) * g_cnGroundXSize + (x+nSize))]);

      if(gt == GT_ROCK) {
        glTexCoord2d(dBase + dScale * (x+y+nSize), dBase + fabs(m_dHeightMap[safe((y+nSize) * g_cnGroundXSize + x)] / g_cdGroundScale * dScale));
      } else if(gt == GT_SAND) {
        glTexCoord2d(x * 0.3 * nSize, (y+nSize) * 0.3 * nSize);
      }
      glNormal3d(m_sqrHeightMapNormals[safe((y+nSize) * g_cnGroundXSize + x)].m_vNormalVis.m_dX,
                 m_sqrHeightMapNormals[safe((y+nSize) * g_cnGroundXSize + x)].m_vNormalVis.m_dY,
                 m_sqrHeightMapNormals[safe((y+nSize) * g_cnGroundXSize + x)].m_vNormalVis.m_dZ);
      glVertex3d(x * g_cdGroundScale, (y+nSize) * g_cdGroundScale, m_dHeightMap[safe((y+nSize) * g_cnGroundXSize + x)]);
    }
    glEnd();
  } else {
    nSize /= 2;
    RenderAreaCoarse(nX, nY, nSize, gt);
    RenderAreaCoarse(nX + nSize, nY, nSize, gt);
    RenderAreaCoarse(nX, nY + nSize, nSize, gt);
    RenderAreaCoarse(nX + nSize, nY + nSize, nSize, gt);
  }
}

bool BGround::AreaFlatEnough(int nX, int nY, int nSize) {
  BIndexGuard safe(g_cnGroundXSize * g_cnGroundYSize - 1, 0);
  if(nSize <= 1) {
    return true;
  } else {
    double dH00 = m_dHeightMap[safe(nY * g_cnGroundXSize + nX)];
    double dH10 = m_dHeightMap[safe(nY * g_cnGroundXSize + nX + 1)];
    double dH01 = m_dHeightMap[safe((nY + 1) * g_cnGroundXSize + nX)];
    double dH11 = m_dHeightMap[safe((nY + 1) * g_cnGroundXSize + nX + 1)];
    for(int y = nY; y <= nY + nSize; ++y) {
      for(int x = nX; x <= nX + nSize; ++x) {
        if((x != nX) && (x != nX + nSize) && (y != nY) && (y != nY + nSize)) {
          double dFX = 1.0 - double(x - nX) / double(nSize);
          double dFY = 1.0 - double(y - nY) / double(nSize);
          double dIdealHeight = m_dHeightMap[safe(y * g_cnGroundXSize + x)];
          double dInterpolatedHeight = dFX         *  dFY        * dH00 + 
                                       (1.0 - dFX) * dFY         * dH10 + 
                                       dFX         * (1.0 - dFY) * dH01 + 
                                       (1.0 - dFX) * (1.0 - dFY) * dH11;
          if(fabs(dInterpolatedHeight - dIdealHeight) > double(nSize) * 3.0) {
            return false;
          }
        }
      }
    }
  }
  return true;
}

void BGround::PaintGroundAreas(BCamera &rCamera, TGroundType gt) {
  int a, nList;
  glEnable(GL_TEXTURE_2D);
  for(int nPass = 0; nPass < 3; ++nPass) {
    if(gt == GT_GROUND && nPass == 2) {
      break;
    }
    if(gt == GT_WATER && nPass != 2) {
      continue;
    }
    glCallList(DISPLIST_GROUNDSAND + nPass);
    for(a = 0; a < g_cnAreas; ++a) {
      BVector vToArea = m_areas[a].m_vCenter - rCamera.m_vLocation;
      double dLen = vToArea.Length();
      vToArea.ToUnitLength();
      if((dLen < 2 * g_cnAreaSize * g_cdGroundScale) || 
         (rCamera.m_orientation.m_vForward.ScalarProduct(vToArea) > 0.5)) {
        if(dLen > m_dCoarseDistance) {
          if(nPass == 0) {
            nList = m_areas[a].m_nDLCoarseRes1;
          } else if(nPass == 1) {
            nList = m_areas[a].m_nDLCoarseRes2;
          } else {
            nList = m_areas[a].m_nDLCoarseRes3;
          }
        } else {
          if(nPass == 0) {
            nList = m_areas[a].m_nDLFullRes1;
          } else if(nPass == 1) {
            nList = m_areas[a].m_nDLFullRes2;
          } else {
            nList = m_areas[a].m_nDLFullRes3;
          }
        }
        glCallList(nList);
      }
    }
  }
  glDisable(GL_CULL_FACE);
  glDisable(GL_TEXTURE_2D);
  glDisable(GL_BLEND);
}


//*****************************************************************************
void BGround::LoadGroundFromFile(std::string sFilename) {
  int x, y;
  FILE *fp = fopen(sFilename.c_str(), "r");
  if(fp) {    
    for(y = 0; y < g_cnGroundYSize; ++y) {
      for(x = 0; x < g_cnGroundXSize; ++x) {
        unsigned char cTmp;
        fscanf(fp, "%c", &cTmp);
        m_dHeightMap[y * g_cnGroundXSize + x] = -double(255 - cTmp) * g_cdGroundHeightScale;
      }
    }
    fclose(fp);

    // Precalculate ground normals
    // Initialize with level ground
    for(y = 0; y < g_cnGroundYSize; ++y) {
      for(x = 0; x < g_cnGroundXSize; ++x) {
        m_sqrHeightMapNormals[y * g_cnGroundXSize + x].m_vNormal1 = BVector(0, 0, -1);
        m_sqrHeightMapNormals[y * g_cnGroundXSize + x].m_vNormal2 = BVector(0, 0, -1);
      }
    }
    // Calculate triangle normals for the simulation
    for(y = 1; y < (g_cnGroundYSize - 1); ++y) {
      for(x = 1; x < (g_cnGroundXSize - 1); ++x) {
        BVector v1, v2, v3, v4;
        v1.Set(x * g_cdGroundScale, y * g_cdGroundScale, m_dHeightMap[y * g_cnGroundXSize + x]);
        v2.Set((x+1) * g_cdGroundScale, y * g_cdGroundScale, m_dHeightMap[y * g_cnGroundXSize + (x+1)]);
        v3.Set(x * g_cdGroundScale, (y+1) * g_cdGroundScale, m_dHeightMap[(y+1) * g_cnGroundXSize + x]);
        v4.Set((x+1) * g_cdGroundScale, (y+1) * g_cdGroundScale, m_dHeightMap[(y+1) * g_cnGroundXSize + (x+1)]);
        m_sqrHeightMapNormals[y * g_cnGroundXSize + x].m_vNormal1 = (v1 - v2).CrossProduct(v4 - v2);
        m_sqrHeightMapNormals[y * g_cnGroundXSize + x].m_vNormal1.ToUnitLength();
        m_sqrHeightMapNormals[y * g_cnGroundXSize + x].m_vNormal2 = (v4 - v3).CrossProduct(v1 - v3);
        m_sqrHeightMapNormals[y * g_cnGroundXSize + x].m_vNormal2.ToUnitLength();
      }
    }
    // calculate visualization normals
    for(y = 0; y < g_cnGroundYSize; ++y) {
      m_sqrHeightMapNormals[y * g_cnGroundXSize].m_vNormalVis = BVector(0, 0, -1);
    }
    for(x = 0; x < g_cnGroundXSize; ++x) {
      m_sqrHeightMapNormals[x].m_vNormalVis = BVector(0, 0, -1);
    }

    for(y = 1; y < g_cnGroundYSize; ++y) {
      for(x = 1; x < g_cnGroundXSize; ++x) {
        m_sqrHeightMapNormals[y * g_cnGroundXSize + x].m_vNormalVis = 
          m_sqrHeightMapNormals[(y - 1) * g_cnGroundXSize + x - 1].m_vNormal1 +
          m_sqrHeightMapNormals[(y - 1) * g_cnGroundXSize + x - 1].m_vNormal2 +
          m_sqrHeightMapNormals[(y - 1) * g_cnGroundXSize + x].m_vNormal1 +
          m_sqrHeightMapNormals[(y - 1) * g_cnGroundXSize + x].m_vNormal2 +
          m_sqrHeightMapNormals[y * g_cnGroundXSize + x - 1].m_vNormal1 +
          m_sqrHeightMapNormals[y * g_cnGroundXSize + x - 1].m_vNormal2 +
          m_sqrHeightMapNormals[y * g_cnGroundXSize + x].m_vNormal1 +
          m_sqrHeightMapNormals[y * g_cnGroundXSize + x].m_vNormal2;
        m_sqrHeightMapNormals[y * g_cnGroundXSize + x].m_vNormalVis.ToUnitLength();
      }
    }
  } else {
	  cout << "Cannot open ground file!" << endl;
    //AfxMessageBox("Cannot open ground file!");
  }
}

void BGround::LoadTextures() {
  // Create a phony ground texture
  int x;
  for(x = 0; x < 64 * 64; ++x) {
    m_dTexture[x*3]   = GLubyte((0.85 + double(rand()) / double(RAND_MAX) * 0.15) * 255.0);  
    m_dTexture[x*3+1] = m_dTexture[x*3];  
    m_dTexture[x*3+2] = m_dTexture[x*3];  
  }
  // Create mipmap levels
  OpenGLHelpers::CreateMipMaps(m_dTexture, 64, 64, 3);


  // Test Monument valley texture
  FILE *fp = fopen("./Textures/monval.raw", "rb");
  if(fp) {    
    for(int y = 0; y < 128; ++y) {
      for(int x = 0; x < 128; ++x) {
        unsigned char cTmp;
        fscanf(fp, "%c", &cTmp);
        m_dMonVal[y * 128 * 3 + x * 3] = cTmp;
        fscanf(fp, "%c", &cTmp);
        m_dMonVal[y * 128 * 3 + x * 3 + 1] = cTmp;
        fscanf(fp, "%c", &cTmp);
        m_dMonVal[y * 128 * 3 + x * 3 + 2] = cTmp;

        if(g_cbBlackAndWhite) {
          double r, g, b, bw;
          r = m_dMonVal[y * 128 * 3 + x * 3];
          g = m_dMonVal[y * 128 * 3 + x * 3 + 1];
          b = m_dMonVal[y * 128 * 3 + x * 3 + 2];
          bw = r*0.37f+g*0.42f+b*0.2f;
          m_dMonVal[y * 128 * 3 + x * 3] = GLubyte(bw);
          m_dMonVal[y * 128 * 3 + x * 3 + 1] = GLubyte(bw);
          m_dMonVal[y * 128 * 3 + x * 3 + 2] = GLubyte(bw);
        }
      }
    }
    fclose(fp);
  } else {
	  cout << "Cannot open monval.raw!" << endl;
    //AfxMessageBox("Cannot open monval.raw!");
  }

  OpenGLHelpers::CreateMipMaps(m_dMonVal, 128, 128, 3);

  // Load sky texture
  fp = fopen("./Textures/skysmall.raw", "rb");
  if(fp) {    
    for(int y = 0; y < 8; ++y) {
      for(int x = 0; x < 8; ++x) {
        unsigned char cTmp;
        fscanf(fp, "%c", &cTmp);
        m_pTxtrSky[y * 8 * 3 + x * 3] = cTmp;
        fscanf(fp, "%c", &cTmp);
        m_pTxtrSky[y * 8 * 3 + x * 3 + 1] = cTmp;
        fscanf(fp, "%c", &cTmp);
        m_pTxtrSky[y * 8 * 3 + x * 3 + 2] = cTmp;

        if(g_cbBlackAndWhite) {
          double r, g, b, bw;
          r = m_pTxtrSky[y * 8 * 3 + x * 3];
          g = m_pTxtrSky[y * 8 * 3 + x * 3 + 1];
          b = m_pTxtrSky[y * 8 * 3 + x * 3 + 2];
          bw = r*0.37f+g*0.42f+b*0.2f;
          m_pTxtrSky[y * 8 * 3 + x * 3] = GLubyte(bw);
          m_pTxtrSky[y * 8 * 3 + x * 3 + 1] = GLubyte(bw);
          m_pTxtrSky[y * 8 * 3 + x * 3 + 2] = GLubyte(bw);
        }
      }
    }
    fclose(fp);
  } else {
	  cout << "Cannot open skysmall.raw!" << endl;
    //AfxMessageBox("Cannot open skysmall.raw!");
  }

  // Create mipmap levels
  OpenGLHelpers::CreateMipMaps(m_pTxtrSky, 8, 8, 3);

  // Load detailed sky texture
  fp = fopen("./Textures/skydetailed.raw", "rb");
  if(fp) {    
    for(int y = 0; y < 128; ++y) {
      for(int x = 0; x < 512; ++x) {
        unsigned char cTmp;
        fscanf(fp, "%c", &cTmp);
        m_pTxtrSkyDetailed[y * 512 * 3 + x * 3] = cTmp;
        fscanf(fp, "%c", &cTmp);
        m_pTxtrSkyDetailed[y * 512 * 3 + x * 3 + 1] = cTmp;
        fscanf(fp, "%c", &cTmp);
        m_pTxtrSkyDetailed[y * 512 * 3 + x * 3 + 2] = cTmp;

        if(g_cbBlackAndWhite) {
          double r, g, b, bw;
          r = m_pTxtrSkyDetailed[y * 512 * 3 + x * 3];
          g = m_pTxtrSkyDetailed[y * 512 * 3 + x * 3 + 1];
          b = m_pTxtrSkyDetailed[y * 512 * 3 + x * 3 + 2];
          bw = r*0.37f+g*0.42f+b*0.2f;
          m_pTxtrSkyDetailed[y * 512 * 3 + x * 3] = GLubyte(bw);
          m_pTxtrSkyDetailed[y * 512 * 3 + x * 3 + 1] = GLubyte(bw);
          m_pTxtrSkyDetailed[y * 512 * 3 + x * 3 + 2] = GLubyte(bw);
        }
      }
    }
    fclose(fp);
  } else {
	  cout << "Cannot open skydetailed.raw!" << endl;
    //AfxMessageBox("Cannot open skydetailed.raw!");
  }

  // Create mipmap levels
  OpenGLHelpers::CreateMipMaps(m_pTxtrSkyDetailed, 512, 128, 3);

  // Load Ocean texture (sea bed)
  fp = fopen("./Textures/seabed.raw", "rb");
  if(fp) {    
    for(int y = 0; y < 8; ++y) {
      for(int x = 0; x < 8; ++x) {
        unsigned char cTmp;
        fscanf(fp, "%c", &cTmp);
        m_pTxtrOcean[y * 8 * 3 + x * 3] = cTmp;
        fscanf(fp, "%c", &cTmp);
        m_pTxtrOcean[y * 8 * 3 + x * 3 + 1] = cTmp;
        fscanf(fp, "%c", &cTmp);
        m_pTxtrOcean[y * 8 * 3 + x * 3 + 2] = cTmp;

        if(g_cbBlackAndWhite) {
          double r, g, b, bw;
          r = m_pTxtrOcean[y * 8 * 3 + x * 3];
          g = m_pTxtrOcean[y * 8 * 3 + x * 3 + 1];
          b = m_pTxtrOcean[y * 8 * 3 + x * 3 + 2];
          bw = r*0.37f+g*0.42f+b*0.2f;
          m_pTxtrOcean[y * 8 * 3 + x * 3] = GLubyte(bw);
          m_pTxtrOcean[y * 8 * 3 + x * 3 + 1] = GLubyte(bw);
          m_pTxtrOcean[y * 8 * 3 + x * 3 + 2] = GLubyte(bw);
        }
      }
    }
    fclose(fp);
  } else {
	  cout << "Cannot open seabed.raw!" << endl;
    //AfxMessageBox("Cannot open seabed.raw!");
  }
  // Create mipmap levels
  OpenGLHelpers::CreateMipMaps(m_pTxtrOcean, 8, 8, 3);
}




/*
  m_pObjects[0].m_face[0].m_nPoints = 4;
  m_pObjects[0].m_face[0].m_vPoint[0].Set(0, 0, 0);
  m_pObjects[0].m_face[0].m_vPoint[1].Set(0, 0, -1);
  m_pObjects[0].m_face[0].m_vPoint[2].Set(1, 0, -1);
  m_pObjects[0].m_face[0].m_vPoint[3].Set(1, 0, 0);

  m_pObjects[0].m_face[1].m_nPoints = 4;
  m_pObjects[0].m_face[1].m_vPoint[0].Set(1, 1, 0);
  m_pObjects[0].m_face[1].m_vPoint[1].Set(1, 1, -1);
  m_pObjects[0].m_face[1].m_vPoint[2].Set(0, 1, -1);
  m_pObjects[0].m_face[1].m_vPoint[3].Set(0, 1, 0);

  m_pObjects[0].m_face[2].m_nPoints = 4;
  m_pObjects[0].m_face[2].m_vPoint[0].Set(0, 1, 0);
  m_pObjects[0].m_face[2].m_vPoint[1].Set(0, 1, -1);
  m_pObjects[0].m_face[2].m_vPoint[2].Set(0, 0, -1);
  m_pObjects[0].m_face[2].m_vPoint[3].Set(0, 0, 0);

  m_pObjects[0].m_face[3].m_nPoints = 4;
  m_pObjects[0].m_face[3].m_vPoint[0].Set(1, 0, 0);
  m_pObjects[0].m_face[3].m_vPoint[1].Set(1, 0, -1);
  m_pObjects[0].m_face[3].m_vPoint[2].Set(1, 1, -1);
  m_pObjects[0].m_face[3].m_vPoint[3].Set(1, 1, 0);

  m_pObjects[0].m_face[4].m_nPoints = 4;
  m_pObjects[0].m_face[4].m_vPoint[0].Set(0, 0, -1);
  m_pObjects[0].m_face[4].m_vPoint[1].Set(0, 1, -1);
  m_pObjects[0].m_face[4].m_vPoint[2].Set(1, 1, -1);
  m_pObjects[0].m_face[4].m_vPoint[3].Set(1, 0, -1);

  m_pObjects[0].m_vScale.Set(30.0, 100, 15);
*/

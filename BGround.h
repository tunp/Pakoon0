//
// Ground
//

#pragma once

#include "BaseClasses.h"
#include "BCamera.h"
#include "BObject.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <string>

const double g_cdGroundScale = 20.0;
const double g_cdGroundHeightScale = 0.5;
const int    g_cnGroundXSize = 96; // must be multiple of g_cnAreaSize
const int    g_cnGroundYSize = 48; // must be multiple of g_cnAreaSize
//const int    g_cnGroundXSize = 160; // must be multiple of g_cnAreaSize
//const int    g_cnGroundYSize = 112; // must be multiple of g_cnAreaSize
const int    g_cnAreaSize = 16;    // must be power of two
const int    g_cnAreas = (g_cnGroundXSize / g_cnAreaSize) * (g_cnGroundYSize / g_cnAreaSize);
const double g_cdWaterLevel = -2.499;

class BGroundArea {
public:
  int     m_nDLFullRes1;
  int     m_nDLFullRes2;
  int     m_nDLFullRes3;
  int     m_nDLCoarseRes1;
  int     m_nDLCoarseRes2;
  int     m_nDLCoarseRes3;
  BVector m_vCenter;
  BGroundArea() {m_nDLFullRes1 = 
                 m_nDLFullRes2 = 
                 m_nDLFullRes3 = 
                 m_nDLCoarseRes1 = 
                 m_nDLCoarseRes2 =
                 m_nDLCoarseRes3 = 0;}
};

class BGroundSquare {
public:
  double  m_dFriction1;
  double  m_dFriction2;
  double  m_dSurfaceDepth1;
  double  m_dSurfaceDepth2;
  bool    m_bWater1;
  bool    m_bWater2;
  BVector m_vNormal1;
  BVector m_vNormal2;
  BVector m_vNormalVis;
  BGroundSquare() {m_bWater1 = m_bWater2 = false;};
};

class BGround {
  GLubyte m_dTexture[(64 * 64 * 3) * 2]; // reserve room also for mipmaps
  GLubyte m_pTxtrSky[(8 * 8 * 3) * 2];
  GLubyte m_pTxtrSkyDetailed[(512 * 128 * 3) * 2];
  GLubyte m_pTxtrOcean[(8 * 8 * 3) * 2];

  void RenderSky(int nDispList);

public:

  enum TGroundType {GT_GROUND, GT_SAND, GT_ROCK, GT_WATER};

  BGroundArea   m_areas[g_cnAreas];
  GLubyte       m_dMonVal[(128 * 128 * 3) * 2]; // reserve room also for mipmaps
  GLubyte       m_dSkyMap[256 * 256 * 3];
  double        m_dHeightMap[g_cnGroundYSize * g_cnGroundXSize];
  BGroundSquare m_sqrHeightMapNormals[g_cnGroundYSize * g_cnGroundXSize];
  double        m_dCoarseDistance;

  int           m_nObjects;
  BObject      *m_pObjects;

  BGround();
  virtual ~BGround();

  void PreProcessVisualization();
  void CreateGroundAreas();
  bool GroundTriangleIsOfType(int x, int y, int nSize, int nTriangle, TGroundType gt);
  double SurfaceHeightAt(int x, int y, TGroundType gt);
  void RenderAreaFull(int nX, int nY, int nSize, TGroundType gt);
  void RenderAreaCoarse(int nX, int nY, int nSize, TGroundType gt);
  bool AreaFlatEnough(int nX, int nY, int nSize);
  void PaintGroundAreas(BCamera &rCamera, TGroundType gt);
  void Paint(int nSkyDetail, int nWaterDetail, BCamera &rCamera, TGroundType gt);
  void LoadGroundFromFile(std::string sFilename);
  void LoadObjectsFromFile(std::string sFilename);
  void LoadTextures();
};

//
// OpenGL helpers
//

#include "BaseClasses.h"
#include "GL/gl.h"
#include "GL/glu.h"

#define DISPLIST_GROUNDFULLRESBASE1   500
#define DISPLIST_GROUNDFULLRESBASE2   DISPLIST_GROUNDFULLRESBASE1+18
#define DISPLIST_GROUNDFULLRESBASE3   DISPLIST_GROUNDFULLRESBASE2+18
#define DISPLIST_GROUNDCOARSERESBASE1 DISPLIST_GROUNDFULLRESBASE3+18
#define DISPLIST_GROUNDCOARSERESBASE2 DISPLIST_GROUNDCOARSERESBASE1+18
#define DISPLIST_GROUNDCOARSERESBASE3 DISPLIST_GROUNDCOARSERESBASE2+18
#define DISPLIST_GROUNDSAND     DISPLIST_GROUNDCOARSERESBASE3+18
#define DISPLIST_GROUNDROCK     DISPLIST_GROUNDCOARSERESBASE3+18+1
#define DISPLIST_GROUNDWATER    DISPLIST_GROUNDCOARSERESBASE3+18+2
#define DISPLIST_USER           DISPLIST_GROUNDWATER+1
#define DISPLIST_WHOLEGROUND    DISPLIST_USER+1
#define DISPLIST_SKYCOARSE      DISPLIST_USER+2
#define DISPLIST_SKYDETAILED    DISPLIST_USER+3
#define DISPLIST_SEABED         DISPLIST_USER+4
#define DISPLIST_WATERSURFACE   DISPLIST_USER+5
#define DISPLIST_CARBODY        DISPLIST_USER+6
#define DISPLIST_CARWHEEL       DISPLIST_USER+7
#define DISPLIST_CARWHEELBROKEN DISPLIST_USER+8
#define DISPLIST_SHADOWTXTR     DISPLIST_USER+9
#define DISPLIST_ROTORTXTR      DISPLIST_USER+10
#define DISPLIST_JETTXTR        DISPLIST_USER+11
#define DISPLIST_CARTXTR        DISPLIST_USER+12
#define DISPLIST_CARBODYNOHELI  DISPLIST_USER+13
#define DISPLIST_CARBODYNOJET   DISPLIST_USER+14
#define DISPLIST_CARBODYHELI    DISPLIST_USER+15
#define DISPLIST_CARBODYJET     DISPLIST_USER+16
#define DISPLIST_OBJECTS        DISPLIST_USER+17
#define DISPLIST_CARWHEELBLADE  DISPLIST_USER+18


namespace OpenGLHelpers {
  void SetColorFull(double dRed, double dGreen, double dBlue, double dAlpha);
  void SetColor(double dRed, double dGreen, double dBlue, double dAlpha);
  void Triangle(BVector& p1, BVector& p2, BVector& p3);
  void TxtrTriangle(BVector& p1, double dTxtrX1, double dTxtrY1, 
                    BVector& p2, double dTxtrX2, double dTxtrY2, 
                    BVector& p3, double dTxtrX3, double dTxtrY3);
  void TxtrTriangleBare(BVector& p1, double dTxtrX1, double dTxtrY1, 
                        BVector& p2, double dTxtrX2, double dTxtrY2, 
                        BVector& p3, double dTxtrX3, double dTxtrY3);
  void TriangleStrip(BVector *pvPoints, int nPoints);
  void TriangleStripWithNormals(BVector *pvPoints, BVector *pvNormals, int nPoints);
  void TriangleStripTexturedX(BVector *pvPoints, 
                              int      nPoints, 
                              int      nOrig1, 
                              int      nOrig2, 
                              double   dTXTRMin, 
                              double   dTXTRMax);
  void TriangleFan(BVector *pvPoints, int nPoints);
  void TriangleFanWithNormals(BVector *pvPoints, BVector *pvNormals, int nPoints);
  void Line(BVector& p1, BVector& p2);
  void Lines(BVector *pvPoints, int nPoints);

  void CreateTexNames(int n);
  void FreeTexNames(int n);

  void CreateMipMaps(GLubyte *pStart, int nWidth, int nHeight, int nComponents);
  void DefineMipMapTextures(int nWidth, int nHeight, int nComponents, GLenum format, GLubyte *pStart, int nTexName);
};

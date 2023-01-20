// GetawayView.cpp : implementation of the CGetawayView class
//

#define OEMRESOURCE
//#include "Getaway.h"

#include <SDL2/SDL.h>
//#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_ttf.h>

#include "SDLHelpers.h"

#include <iostream>
#include <string>
#include <vector>

using namespace std;

#include "NewDialogs/Item.h"
#include "NewDialogs/Dialog.h"
#include "NewDialogs/Button.h"
#include "NewDialogs/Frame.h"
#include "NewDialogs/ButtonGroup.h"
#include "NewDialogs/DropDownList.h"
#include "ControllerModule.h"
#include "GetawayDoc.h"
#include "GetawayView.h"
#include "NewDialogs/DlgMainMenu.h"
#include "NewDialogs/DlgVisuals.h"
#include "NewDialogs/DlgSounds.h"
#include "NewDialogs/DlgControls.h"
#include "NewDialogs/DlgOnScreenKbd.h"
//#include "Dialogs.h"
//#include "DlgControls.h"
//#include "DlgSounds.h"

#include "OpenGLHelpers.h"
//#include <GL/gl.h>
//#include <GL/glu.h>

#include "SoundModule.h"
#include "Settings.h"

/*#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif*/

extern bool g_cbBlackAndWhite;

double g_dPhysicsStepsInSecond = 300.0;

/////////////////////////////////////////////////////////////////////////////
// CGetawayView

/*IMPLEMENT_DYNCREATE(CGetawayView, CView)

BEGIN_MESSAGE_MAP(CGetawayView, CView)
//{{AFX_MSG_MAP(CGetawayView)
ON_WM_CREATE()
ON_WM_ERASEBKGND()
ON_WM_DESTROY()
ON_WM_KEYDOWN()
ON_WM_KEYUP()
ON_WM_MOUSEMOVE()
ON_WM_LBUTTONDOWN()
ON_WM_TIMER()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()*/

/////////////////////////////////////////////////////////////////////////////
// CGetawayView construction/destruction

CGetawayView::CGetawayView()
{
	exit = false;
// TODO: add construction code here
  //m_nMenuTime = 0;
  m_pDrawFunction = &CGetawayView::OnDrawIntro;
  //m_pDrawFunction = &CGetawayView::OnDrawStartMenu;
  m_bDrawOnlyMenu = false;
  m_bFullRedraw = true;
  //m_hCursor = 0;
  SoundModule::Initialize();
  //m_bInitClock = true;
	pDoc = new CGetawayDoc();
	escape = false;
  touch_seen = false;
}

CGetawayView::~CGetawayView() {
	delete pDoc;
}


void CGetawayView::Load43AATexture() {
  // Load detailed sky texture
  FILE *fp = fopen("./Textures/43aa.raw", "rb");
  if(fp) {    
    for(int y = 0; y < 64; ++y) {
      for(int x = 0; x < 512; ++x) {
        unsigned char cTmp;
        fscanf(fp, "%c", &cTmp);
        m_d43AATexture[y * 512 * 3 + x * 3] = cTmp;
        m_d43AATexture[y * 512 * 3 + x * 3 + 1] = cTmp;
        m_d43AATexture[y * 512 * 3 + x * 3 + 2] = cTmp;
      }
    }
    fclose(fp);
  } else {
	  cout << "Cannot open 43aa.raw!" << endl;
    //AfxMessageBox("Cannot open 43aa.raw!");
  }

  // Create mipmap levels
  OpenGLHelpers::CreateMipMaps(m_d43AATexture, 512, 64, 3);

  // Create display list for the 43AA texture
  OpenGLHelpers::DefineMipMapTextures(512, 64, 3, GL_RGB, m_d43AATexture, 10);
}


/*BOOL CGetawayView::PreCreateWindow(CREATESTRUCT& cs) {
  // TODO: Modify the Window class or styles here by modifying
  //  the CREATESTRUCT cs
  return CView::PreCreateWindow(cs);
}*/

//int CGetawayView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
void CGetawayView::OnCreate()
{
  /*BOOL retval = CView::OnCreate(lpCreateStruct);

  HWND hWnd = GetSafeHwnd();
  HDC hDC = ::GetDC(hWnd);
  PIXELFORMATDESCRIPTOR pfd = {
  sizeof(PIXELFORMATDESCRIPTOR), // size of this pfd
  1, // version number
  PFD_DRAW_TO_WINDOW |    // support window
    PFD_SUPPORT_OPENGL |  // support OpenGL
    // PFD_SUPPORT_GDI |     // support GDI
    PFD_DOUBLEBUFFER |    // double buffered
    PFD_NEED_SYSTEM_PALETTE, 
  PFD_TYPE_RGBA, // RGBA type
  32, // 32-bit color depth
  0, 0, 0, 0, 0, 0, // color bits ignored
  0, // no alpha buffer
  0, // shift bit ignored
  0, // no accumulation buffer
  0, 0, 0, 0, // accum bits ignored
  32, // 32-bit z-buffer 
  0, // no stencil buffer
  0, // no auxiliary buffer
  PFD_MAIN_PLANE, // main layer
  0, // reserved
  0, 0, 0 // layer masks ignored
  };
  int pixelformat = ChoosePixelFormat(hDC, &pfd);
  TRACE("Pixelformat %d\n", pixelformat);
  if(SetPixelFormat(hDC, pixelformat, &pfd) == FALSE) {
    ::MessageBox(NULL, "SetPixelFormat failed", "Error", MB_OK);
  }  
  m_hGLRC = wglCreateContext(hDC);
  wglMakeCurrent(hDC, m_hGLRC);

  // Read settings for graphics etc.
  CGetawayDoc* pDoc = GetDocument();
  ASSERT_VALID(pDoc);*/
  Settings::ReadSettings(pDoc->GetSimulation());

  // Initialize common OpenGL features
  InitializeOpenGL();

  // Prepare visualization
  //OpenGLHelpers::CreateTexNames(11);
  OpenGLHelpers::CreateTexNames(12);
  pDoc->GetSimulation()->GetGround()->LoadTextures();
  pDoc->GetSimulation()->GetCar()->LoadTextures();
  pDoc->GetSimulation()->GetGround()->PreProcessVisualization();
  pDoc->GetSimulation()->GetCar()->PreProcessVisualization();
  pDoc->GetSimulation()->PreProcessVisualization();

  Load43AATexture();
  loadStartMenu();

  // Change cursor to Pakoon! cursor
  /*m_hCursor = AfxGetApp()->LoadCursor(IDC_POINTER);
  if(m_hCursor) {
    ::SetClassLong(GetSafeHwnd(), GCL_HCURSOR, 0L);
    ::SetCursor(m_hCursor);
  }

  m_glfont.Init(hDC);

  ::ReleaseDC(GetSafeHwnd(), hDC);

  return retval;*/
}

/*BOOL CGetawayView::OnEraseBkgnd(CDC* pDC) {
  return TRUE;
}*/

/*void CGetawayView::OnDestroy() 
{
  HWND hWnd = GetSafeHwnd();
  HDC hDCWnd = ::GetDC(hWnd);
  wglMakeCurrent(hDCWnd, m_hGLRC);
  HDC   hDC = wglGetCurrentDC();
  wglMakeCurrent(NULL, NULL);
  if (m_hGLRC)
    wglDeleteContext(m_hGLRC);
  if (hDC)
    ::ReleaseDC(GetSafeHwnd(), hDC);
  CView::OnDestroy();
}*/


void CGetawayView::InitializeOpenGL() {
  glClearColor(0, 0, 0, 0); /* For RGB-mode */
  glClearDepth(1.0 );

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glShadeModel(GL_SMOOTH);
  // glEnable(GL_POLYGON_SMOOTH);

  /*
  GLfloat vFogColor[4];
  vFogColor[0] = 0.9;
  vFogColor[1] = 0.9;
  vFogColor[2] = 1.0;
  vFogColor[3] = 0.0;
  glEnable( GL_FOG);
  glFogi( GL_FOG_MODE, GL_LINEAR);
  glFogf( GL_FOG_START, 10);
  glFogf( GL_FOG_END, 500);
  glFogfv( GL_FOG_COLOR, vFogColor);
  */

  glEnable(GL_LIGHTING);

  GLfloat fLight1PositionG[ 4];
  GLfloat fLight1AmbientG[ 4];
  GLfloat fLight1DiffuseG[ 4];
  GLfloat fLight1SpecularG[ 4];

  fLight1PositionG[0] = (GLfloat) 0.15;  //GetDocument()->GetBird()->m_pntPosition.dX;
  fLight1PositionG[1] = (GLfloat) 0.25;  //GetDocument()->GetBird()->m_pntPosition.dY;
  fLight1PositionG[2] = (GLfloat) 1.0;   //GetDocument()->GetBird()->m_pntPosition.dZ;
  fLight1PositionG[3] = (GLfloat) 0.0;   // w=0 -> directional light (not positional)

  fLight1AmbientG[0] = 0.7f;
  fLight1AmbientG[1] = 0.7f;
  fLight1AmbientG[2] = 0.7f;
  fLight1AmbientG[3] = 0.7f;

  fLight1DiffuseG[0] = 0.8f;
  fLight1DiffuseG[1] = 0.8f;
  fLight1DiffuseG[2] = 0.8f;
  fLight1DiffuseG[3] = 0.8f;

  fLight1SpecularG[0] = 0.8f;
  fLight1SpecularG[1] = 0.8f;
  fLight1SpecularG[2] = 0.8f;
  fLight1SpecularG[3] = 0.8f;

  glLightfv( GL_LIGHT0, GL_AMBIENT,  fLight1AmbientG);
  glLightfv( GL_LIGHT0, GL_DIFFUSE,  fLight1DiffuseG);
  glLightfv( GL_LIGHT0, GL_SPECULAR, fLight1SpecularG);
  glLightfv( GL_LIGHT0, GL_POSITION, fLight1PositionG);                     
  glEnable( GL_LIGHT0);

  glEnable(GL_NORMALIZE);

  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

  //CGetawayDoc* pDoc = GetDocument();
  //ASSERT_VALID(pDoc);
  switch(pDoc->GetSimulation()->m_nTextureSmoothness) {
    case 0: // Boxy
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      break;
    case 1: // Gritty
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      break;
    case 2: // Silky
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      break;
  }

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // glEnable(GL_AUTO_NORMAL);
  // glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  // glClearColor(0.5f, 0.6f, 0.8f, 0.0f);
  // glClearColor(0.9f, 0.9f, 1.9f, 0.0f);
  glClearColor(0.6f, 0.7f, 0.9f, 0.0f);
  if(g_cbBlackAndWhite) {
    glClearColor(0.7f, 0.7f, 0.7f, 0.0f);
  }
  glClearColor(0, 0, 0, 0);
  return;
}




void CGetawayView::FadeInText(string s1, string s2, string s3, int ticks, TTF_Font *pfont1, TTF_Font *pfont2) {
  /*CBrush brushBlack(RGB(0, 0, 0));
  CRect rectWnd;
  GetClientRect(&rectWnd);
  pDC->SelectObject(pfont1);
  CSize sizeSmall1 = pDC->GetTextExtent(s1);
  CSize sizeSmall2 = pDC->GetTextExtent(s2);
  pDC->SelectObject(pfont2);
  CSize sizeBig = pDC->GetTextExtent(s3);*/

  // Create bitmaps for double buffer drawing
  /*CBitmap bmpSmall1;
  bmpSmall1.CreateCompatibleBitmap(pDC, sizeSmall1.cx, sizeSmall1.cy);
  CBitmap bmpSmall2;
  bmpSmall2.CreateCompatibleBitmap(pDC, sizeSmall2.cx, sizeSmall2.cy);
  CBitmap bmpBig;
  bmpBig.CreateCompatibleBitmap(pDC, sizeBig.cx, sizeBig.cy);

  CDC dcCopy;
  dcCopy.CreateCompatibleDC(pDC);
  dcCopy.SelectObject(pbmpArt);
  pDC->BitBlt(rectWnd.Width() * 2 / 3, rectWnd.Height() / 2 - 100, 200, 200, &dcCopy, 0, 0, SRCCOPY);

  dcCopy.SetBkMode(TRANSPARENT);
  dcCopy.SetTextAlign(TA_RIGHT);
  CRect rectTmp;*/
  
  int color;
  if (ticks < 0 || ticks > 2500) {
	  return;
  } else if (ticks < 250) {
	  color = ticks;
  } else if (ticks > 2250) {
	  color = 255 - (ticks - 2250);
  } else {
	  color = 255;
  }
//  for(color = 0; color < 255; color += 2) {
    /*dcCopy.SelectObject(&bmpSmall1);
    rectTmp.TopLeft() = CPoint(0, 0);
    rectTmp.BottomRight() = CPoint(sizeSmall1.cx, sizeSmall1.cy);
    dcCopy.FillRect(&rectTmp, &brushBlack);
    dcCopy.SelectObject(pfont1);
    dcCopy.SetTextColor(RGB(color, color, color));
    dcCopy.TextOut(sizeSmall1.cx, 0, s1);
    pDC->BitBlt(rectWnd.Width() * 2 / 3 - rectWnd.Width() / 20 - sizeSmall1.cx, 
                rectWnd.Height() / 2 - sizeSmall1.cy * 2, 
                sizeSmall1.cx, sizeSmall1.cy, &dcCopy, 0, 0, SRCCOPY);*/
                
	SDL_Surface *surface;
	double x, y;
	
	if (s1.length() > 0) {
		if(!(surface = TTF_RenderText_Blended(pfont1, s1.c_str(), getColor(color, color, color))))
			cout << TTF_GetError() << endl;
		x = window_width * 2 / 3 - window_width / 20 - surface->w;
		y = window_height / 2 - surface->h * 2;
		drawSurface(x, y, x + surface->w, y + surface->h, GL_BGRA, surface);
		SDL_FreeSurface(surface);
	}

    /*dcCopy.SelectObject(&bmpSmall2);
    rectTmp.TopLeft() = CPoint(0, 0);
    rectTmp.BottomRight() = CPoint(sizeSmall2.cx, sizeSmall2.cy);
    dcCopy.FillRect(&rectTmp, &brushBlack);
    dcCopy.TextOut(sizeSmall2.cx, 0, s2);
    pDC->BitBlt(rectWnd.Width() * 2 / 3 - rectWnd.Width() / 20 - sizeSmall2.cx, 
                rectWnd.Height() / 2 - sizeSmall2.cy, 
                sizeSmall2.cx, sizeSmall2.cy, &dcCopy, 0, 0, SRCCOPY);*/
         
    if (s2.length() > 0) {       
		if(!(surface = TTF_RenderText_Blended(pfont1, s2.c_str(), getColor(color, color, color))))
			cout << TTF_GetError() << endl;
		x = window_width * 2 / 3 - window_width / 20 - surface->w;
		y = window_height / 2 - surface->h;
		drawSurface(x, y, x + surface->w, y + surface->h, GL_BGRA, surface);
		SDL_FreeSurface(surface);
	}

    /*dcCopy.SelectObject(&bmpBig);
    rectTmp.TopLeft() = CPoint(0, 0);
    rectTmp.BottomRight() = CPoint(sizeBig.cx, sizeBig.cy);
    dcCopy.FillRect(&rectTmp, &brushBlack);
    dcCopy.SelectObject(pfont2);
    dcCopy.SetTextColor(RGB(color, color, color));
    dcCopy.TextOut(sizeBig.cx, 0, s3);
    pDC->BitBlt(rectWnd.Width() * 2 / 3 - rectWnd.Width() / 20 - sizeBig.cx, 
                rectWnd.Height() / 2, sizeBig.cx, sizeBig.cy, &dcCopy, 0, 0, SRCCOPY);*/

	if (s3.length() > 0) {
		if(!(surface = TTF_RenderText_Blended(pfont2, s3.c_str(), getColor(color, color, color))))
			cout << TTF_GetError() << endl;
		x = window_width * 2 / 3 - window_width / 20 - surface->w;
		y = window_height / 2;
		drawSurface(x, y, x + surface->w, y + surface->h, GL_BGRA, surface);
		SDL_FreeSurface(surface);
	}
	
/*    Sleep(nFadeDelay);
  }
  Sleep(nDelay);
  for(color = 255; color >= 0; color -= 2) {
    dcCopy.SelectObject(&bmpSmall1);
    rectTmp.TopLeft() = CPoint(0, 0);
    rectTmp.BottomRight() = CPoint(sizeSmall1.cx, sizeSmall1.cy);
    dcCopy.FillRect(&rectTmp, &brushBlack);
    dcCopy.SelectObject(pfont1);
    dcCopy.SetTextColor(RGB(color, color, color));
    dcCopy.TextOut(sizeSmall1.cx, 0, s1);
    pDC->BitBlt(rectWnd.Width() * 2 / 3 - rectWnd.Width() / 20 - sizeSmall1.cx, 
                rectWnd.Height() / 2 - sizeSmall1.cy * 2, 
                sizeSmall1.cx, sizeSmall1.cy, &dcCopy, 0, 0, SRCCOPY);

    dcCopy.SelectObject(&bmpSmall2);
    rectTmp.TopLeft() = CPoint(0, 0);
    rectTmp.BottomRight() = CPoint(sizeSmall2.cx, sizeSmall2.cy);
    dcCopy.FillRect(&rectTmp, &brushBlack);
    dcCopy.TextOut(sizeSmall2.cx, 0, s2);
    pDC->BitBlt(rectWnd.Width() * 2 / 3 - rectWnd.Width() / 20 - sizeSmall2.cx, 
                rectWnd.Height() / 2 - sizeSmall2.cy, 
                sizeSmall2.cx, sizeSmall2.cy, &dcCopy, 0, 0, SRCCOPY);

    dcCopy.SelectObject(&bmpBig);
    rectTmp.TopLeft() = CPoint(0, 0);
    rectTmp.BottomRight() = CPoint(sizeBig.cx, sizeBig.cy);
    dcCopy.FillRect(&rectTmp, &brushBlack);
    dcCopy.SelectObject(pfont2);
    dcCopy.SetTextColor(RGB(color, color, color));
    dcCopy.TextOut(sizeBig.cx, 0, s3);
    pDC->BitBlt(rectWnd.Width() * 2 / 3 - rectWnd.Width() / 20 - sizeBig.cx, 
                rectWnd.Height() / 2, sizeBig.cx, sizeBig.cy, &dcCopy, 0, 0, SRCCOPY);
    Sleep(nFadeDelay);
  }*/
}


static double g_cdPI = 3.1415926;

void CGetawayView::DrawPlanetGlow(double current, int nRad) {
	double max_angle = 160.0 + (300.0 - 160.0) * current;
  for(double dAngle = last_angle; dAngle < max_angle && dAngle < 300.0; dAngle += 0.03) {
    double dColor;
    if(dAngle < 200.0) {
      dColor = cos(fabs(dAngle - 200.0) / 40.0 * g_cdPI / 2.0);
    } else {
      dColor = cos(fabs(dAngle - 200.0) / 100.0 * g_cdPI / 2.0);
    }
    double dRedF = pow(dColor, 6);
    double dGreenF = pow(dColor, 10);
    double dBlueF = pow(dColor, 2);
    dColor = pow(dColor, 2.0) * 40.0;
    // dColor = 45.0;

    double dX = 201 + cos(dAngle / 180.0 * g_cdPI) * double(nRad);
    double dY = 201 + sin(dAngle / 180.0 * g_cdPI) * double(nRad);

    // Antialias over 4 pixels
    double d11, d12, d21, d22;
    double dX1, dX2;
    double dY1, dY2;
    dX1 = 2.0 * (0.5 - fabs(fabs(dX - int(dX)) - 0.5));
    dX2 = 1.0 - dX1;
    dY1 = 2.0 * (0.5 - fabs(fabs(dY - int(dY)) - 0.5));
    dY2 = 1.0 - dY1;

    d11 = dX1 * dY1;
    d21 = dX2 * dY1;
    d12 = dX1 * dY2;
    d22 = dX2 * dY2;

    int nX1, nX2;
    int nY1, nY2;
    nX1 = int(dX);
    if(dX - int(dX) > 0.5) {
      nX2 = nX1 + 1;
    } else {
      nX2 = nX1 - 1;
    }
    nY1 = int(dY);
    if(dY - int(dY) > 0.5) {
      nY2 = nY1 + 1;
    } else {
      nY2 = nY1 - 1;
    }

    /*COLORREF cTmp, cNew;
    cTmp = pDC->GetPixel(nX1, nY1);
    cNew = RGB(GetRValue(cTmp) + dColor * d11 * dRedF, GetGValue(cTmp) + dColor * d11 * dGreenF, GetBValue(cTmp) + dColor * d11 * dBlueF);
    pDC->SetPixel(nX1, nY1, cNew);
    cTmp = pDC->GetPixel(nX2, nY1);
    cNew = RGB(GetRValue(cTmp) + dColor * d21 * dRedF, GetGValue(cTmp) + dColor * d21 * dGreenF, GetBValue(cTmp) + dColor * d21 * dBlueF);
    pDC->SetPixel(nX2, nY1, cNew);
    cTmp = pDC->GetPixel(nX1, nY2);
    cNew = RGB(GetRValue(cTmp) + dColor * d12 * dRedF, GetGValue(cTmp) + dColor * d12 * dGreenF, GetBValue(cTmp) + dColor * d12 * dBlueF);
    pDC->SetPixel(nX1, nY2, cNew);
    cTmp = pDC->GetPixel(nX2, nY2);
    cNew = RGB(GetRValue(cTmp) + dColor * d22 * dRedF, GetGValue(cTmp) + dColor * d22 * dGreenF, GetBValue(cTmp) + dColor * d22 * dBlueF);
    pDC->SetPixel(nX2, nY2, cNew);*/
    
    SDL_Color cTmp, cNew;
    Uint32 *pixel;
    pixel = ((Uint32 *)credits_glow->pixels) + (nY1 * credits_glow->w + nX1);
	SDL_GetRGB(*pixel, credits_glow->format, &cTmp.r, &cTmp.g, &cTmp.b);
	cNew.r = cTmp.r + dColor * d11 * dRedF;
	cNew.g = cTmp.g + dColor * d11 * dGreenF;
	cNew.b = cTmp.b + dColor * d11 * dBlueF;
	*pixel = SDL_MapRGB(credits_glow->format, cNew.r, cNew.g, cNew.b);
	
    pixel = ((Uint32 *)credits_glow->pixels) + (nY2 * credits_glow->w + nX1);
	SDL_GetRGB(*pixel, credits_glow->format, &cTmp.r, &cTmp.g, &cTmp.b);
	cNew.r = cTmp.r + dColor * d21 * dRedF;
	cNew.g = cTmp.g + dColor * d21 * dGreenF;
	cNew.b = cTmp.b + dColor * d21 * dBlueF;
	*pixel = SDL_MapRGB(credits_glow->format, cNew.r, cNew.g, cNew.b);
	
    pixel = ((Uint32 *)credits_glow->pixels) + (nY1 * credits_glow->w + nX2);
	SDL_GetRGB(*pixel, credits_glow->format, &cTmp.r, &cTmp.g, &cTmp.b);
	cNew.r = cTmp.r + dColor * d12 * dRedF;
	cNew.g = cTmp.g + dColor * d12 * dGreenF;
	cNew.b = cTmp.b + dColor * d12 * dBlueF;
	*pixel = SDL_MapRGB(credits_glow->format, cNew.r, cNew.g, cNew.b);
	
    pixel = ((Uint32 *)credits_glow->pixels) + (nY2 * credits_glow->w + nX2);
	SDL_GetRGB(*pixel, credits_glow->format, &cTmp.r, &cTmp.g, &cTmp.b);
	cNew.r = cTmp.r + dColor * d22 * dRedF;
	cNew.g = cTmp.g + dColor * d22 * dGreenF;
	cNew.b = cTmp.b + dColor * d22 * dBlueF;
	*pixel = SDL_MapRGB(credits_glow->format, cNew.r, cNew.g, cNew.b);
  }
  
  last_angle = max_angle;
}


void CGetawayView::TestCredits() {
	int glow_ticks = 200;
	int text_ticks = 2500;
	int cur_ticks = SDL_GetTicks() - start_ticks;
	int max_ticks = 0;
  // create text fonts
  /*CFont fontSmall;
  CFont fontBig;
  fontSmall.CreatePointFont(140, "Lucida Sans Unicode", pDC);
  fontBig.CreatePointFont(220, "Arial Black", pDC);*/

  //CBitmap bmpArt;
  //bmpArt.LoadBitmap(IDB_BITMAP1);

  // Clear background and draw planet glow
  /*CBrush brushBlack(RGB(0, 0, 0));
  CRect rectWnd;
  GetClientRect(&rectWnd);
  pDC->FillRect(&rectWnd, &brushBlack);*/
  glClear(GL_COLOR_BUFFER_BIT);

  //DrawPlanetGlow(pDC, rectWnd.Width() * 1 / 3, rectWnd.Height() / 2 - 100, 250);
	DrawPlanetGlow((cur_ticks / glow_ticks), 200);
	double x = window_width * 1 / 3 - 201;
	double y = window_height / 2 - 100 - 201;
	drawSurface(x, y, x + 300, y + 300, GL_RGBA, credits_glow);

	x = window_width * 2 / 3;
	y = window_height / 2 - 100;
	drawSurface(x, y, x + credits_bitmap->w, y + credits_bitmap->h, GL_RGBA, credits_bitmap);

  // Draw texts fading in
  /*FadeInText(pDC, 
             "All Programming",
             "(3D, graphics, sounds, physics, UI, terrain, objects)",
             "Mikko Oksalahti",
             1, 2000, &fontSmall, &fontBig, &bmpArt);*/
  max_ticks += glow_ticks;
  FadeInText("All Programming", "(3D, graphics, sounds, physics, UI, terrain, objects)", "Mikko Oksalahti", cur_ticks - max_ticks, fontSmall, fontBig);
  /*if(::GetAsyncKeyState(VK_ESCAPE)) {
    return;
  }
  FadeInText(pDC, 
             "Textures and",
             "Sound Samples",
             "Mikko Oksalahti",
             1, 2000, &fontSmall, &fontBig, &bmpArt);*/
  max_ticks += text_ticks;
  FadeInText("Textures and", "Sound Samples", "Mikko Oksalahti", cur_ticks - max_ticks, fontSmall, fontBig);
  /*if(::GetAsyncKeyState(VK_ESCAPE)) {
    return;
  }
  FadeInText(pDC, 
             "",
             "Game Menu Music (MixDown3)",
             "Juha Kauppinen",
             1, 2000, &fontSmall, &fontBig, &bmpArt);*/
  max_ticks += text_ticks;
  FadeInText("", "Game Menu Music (MixDown3)", "Juha Kauppinen", cur_ticks - max_ticks, fontSmall, fontBig);
  /*if(::GetAsyncKeyState(VK_ESCAPE)) {
    return;
  }
  FadeInText(pDC, 
             "Special Thanks for",
             "Ideas, Technical Support and Game Knowledge",
             "Juha Pomppu",
             1, 2000, &fontSmall, &fontBig, &bmpArt);*/
  max_ticks += text_ticks;
  FadeInText("Special Thanks for", "Ideas, Technical Support and Game Knowledge", "Juha Pomppu", cur_ticks - max_ticks, fontSmall, fontBig);
  /*if(::GetAsyncKeyState(VK_ESCAPE)) {
    return;
  }
  FadeInText(pDC, 
             "Special Thanks for",
             "Initial Sparkle",
             "Jyrki Alakuijala",
             1, 2000, &fontSmall, &fontBig, &bmpArt);*/
  max_ticks += text_ticks;
  FadeInText("Special Thanks for", "Initial Sparkle", "Jyrki Alakuijala", cur_ticks - max_ticks, fontSmall, fontBig);
  /*if(::GetAsyncKeyState(VK_ESCAPE)) {
    return;
  }
  FadeInText(pDC, 
             "Special Thanks for",
             "Inspiration",
             "Guys Who Did the Homeworld",
             1, 2000, &fontSmall, &fontBig, &bmpArt);*/
  max_ticks += text_ticks;
  FadeInText("Special Thanks for", "Inspiration", "Guys Who Did the Homeworld", cur_ticks - max_ticks, fontSmall, fontBig);
  /*FadeInText(pDC, 
             "Special Thanks for",
             "The Pragmatic Sound Library (FMOD)",
             "Firelight Multimedia",
             1, 2000, &fontSmall, &fontBig, &bmpArt);*/
  max_ticks += text_ticks;
  FadeInText("Special Thanks for", "The Pragmatic Sound Library (FMOD)", "Firelight Multimedia", cur_ticks - max_ticks, fontSmall, fontBig);
  
  SDL_GL_SwapWindow(window);
  
  max_ticks += text_ticks;
  if (cur_ticks > max_ticks || escape) {
		SDL_FreeSurface(credits_bitmap);
		SDL_FreeSurface(credits_glow);
		TTF_CloseFont(fontSmall);
		TTF_CloseFont(fontBig);
		m_pDrawFunction = &CGetawayView::OnDrawStartMenu;
		SDL_ShowCursor(SDL_ENABLE);
	}
}


/*int CGetawayView::TypeInText(CDC *pDC, int nWaitms, int nChar, char *sText, bool &rbPauseAWhile) {

  rbPauseAWhile = false;

  CRect rectWnd;
  GetClientRect(&rectWnd);
  if(nChar == 0) {
    CBrush brushBlack(RGB(0, 0, 0));
    pDC->FillRect(&rectWnd, &brushBlack);
  }

  if(sText[nChar] == 0) {
    SoundModule::SetMessageSoundVolume(0);
    return 0;
  }
  if(nChar == 0) {
    SoundModule::SetMessageSoundVolume(128);
  }

  pDC->SetTextColor(RGB(128, 0, 0));
  int nCharsPerLine = 54;
  int nLines = strlen(sText) / nCharsPerLine + 1;
  CSize size = pDC->GetTextExtent("M");
  int nBaseY = rectWnd.Height() / 2 - (nLines * size.cy) / 2;
  int nBaseX = rectWnd.Width() / 2 - min(50, strlen(sText)) / 2 * size.cx;

  int nWritten = 0;
  while((nWritten < 4) && (sText[nChar] != 0)) {
    int nY = nBaseY + ((nChar / nCharsPerLine) + 1) * size.cy;
    int nX = nBaseX + ((nChar % nCharsPerLine) * size.cx);
    CString sChar;
    sChar.Format("%c", sText[nChar]);
    pDC->TextOut(nX, nY, sChar);
    // rbPauseAWhile = (sText[nChar] == '.');
    ++nChar;
    ++nWritten;
  }
  return nChar;
}*/

/*void CGetawayView::OnTimer(UINT nIDEvent) {
  if(nIDEvent == 1) {
    InvalidateRect(NULL);
    CView::OnTimer(nIDEvent);
  } else {
    CView::OnTimer(nIDEvent);
  }
}*/



//void CGetawayView::Draw43AAInPhase(CDC* pDC, HDC hDC, double dPhase) {
void CGetawayView::Draw43AAInPhase(double dPhase) {
  glDrawBuffer(GL_BACK);

  // Reset OpenGL
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glScaled(-1.0, 1.0, 1.0);
  double dAspect = double(window_width) / double(window_height);
  gluPerspective(70.0f, dAspect, 0.1f, 100.0f);
  
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // Set look at -point for camera 
  glViewport(0, 0, (GLint) window_width, (GLint) window_height);
  gluLookAt(0, -5, 0, 0, 0, 0, 0, 0, -1);

  glEnable(GL_BLEND);
  glDisable(GL_DEPTH_TEST);
  glBlendFunc(GL_ONE, GL_SRC_ALPHA);
  glEnable(GL_TEXTURE_2D);

  // Draw the polygons
  double dY1 = (dPhase - 0.5) * -30.0 - 5.0;
  double dY2 = (dPhase - 0.5) * -30.0 - 0.0;
  double dY3 = (dPhase - 0.5) * -30.0 + 5.0;
  double dColor;

  OpenGLHelpers::DefineMipMapTextures(512, 64, 3, GL_RGB, m_d43AATexture, 10);

  // 43%
  glBegin(GL_TRIANGLE_STRIP);

  if(dY1 > 0.0) {
    dColor = 1.0 - (fabs(dY1)/10.0);
  } else {
    dColor = 1.0 - (fabs(dY1)/5.0);
  }
  if(dColor < 0.0) {
    dColor = 0.0;
  }
  OpenGLHelpers::SetColorFull(dColor, 0, 0, 1);

  glTexCoord2d(0, 0);
  glVertex3d(-100.0 / 512.0 / 2.0 * 8.0, dY1, 0.5);
  glTexCoord2d(0, 1);
  glVertex3d(-100.0 / 512.0 / 2.0 * 8.0, dY1, -0.5);
  glTexCoord2d(100.0 / 512.0, 0);
  glVertex3d(100.0 / 512.0 / 2.0 * 8.0, dY1, 0.5);
  glTexCoord2d(100.0 / 512.0, 1);
  glVertex3d(100.0 / 512.0 / 2.0 * 8.0, dY1, -0.5);
  glEnd();

  // Artificial
  glBegin(GL_TRIANGLE_STRIP);

  dColor = 1.0 - (fabs(dY2)/5.0);
  if(dColor < 0.0) {
    dColor = 0.0;
  }
  OpenGLHelpers::SetColorFull(dColor, 0, 0, 1);

  glTexCoord2d(100.0 / 512.0, 0);
  glVertex3d(-186.0 / 512.0 / 2.0 * 8.0, dY2, 0.5);
  glTexCoord2d(100.0 / 512.0, 1);
  glVertex3d(-186.0 / 512.0 / 2.0 * 8.0, dY2, -0.5);
  glTexCoord2d(286.0 / 512.0, 0);
  glVertex3d(186.0 / 512.0 / 2.0 * 8.0, dY2, 0.5);
  glTexCoord2d(286.0 / 512.0, 1);
  glVertex3d(186.0 / 512.0 / 2.0 * 8.0, dY2, -0.5);
  glEnd();

  // Adrenaline
  glBegin(GL_TRIANGLE_STRIP);

  dColor = 1.0 - (fabs(dY3)/5.0);
  if(dColor < 0.0) {
    dColor = 0.0;
  }
  OpenGLHelpers::SetColorFull(dColor, 0, 0, 1);

  glTexCoord2d(286.0 / 512.0, 0);
  glVertex3d(-226.0 / 512.0 / 2.0 * 8.0, dY3, 0.5);
  glTexCoord2d(286.0 / 512.0, 1);
  glVertex3d(-226.0 / 512.0 / 2.0 * 8.0, dY3, -0.5);
  glTexCoord2d(512.0 / 512.0, 0);
  glVertex3d(226.0 / 512.0 / 2.0 * 8.0, dY3, 0.5);
  glTexCoord2d(512.0 / 512.0, 1);
  glVertex3d(226.0 / 512.0 / 2.0 * 8.0, dY3, -0.5);
  glEnd();

  glDisable(GL_BLEND);
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_TEXTURE_2D);

  glFinish();

  //SwapBuffers(hDC);
  SDL_GL_SwapWindow(window);
}


//void CGetawayView::OnDrawIntro(CDC* pDC) {
void CGetawayView::OnDrawIntro() {
  //HDC hDC = pDC->GetSafeHdc();
  //wglMakeCurrent(hDC, m_hGLRC); 
  static int nPhase = 0;
  static int nChar = 0;
  static int nCountDown = 30;
  static unsigned int clockStart;

  // Draw Metal Oxide logo sequence

  // Draw 43% Artificial Adrenaline sequence
  if(nPhase == 0) {
    //ShowCursor(FALSE);
    SDL_ShowCursor(SDL_DISABLE);
    //while(::GetAsyncKeyState(VK_ESCAPE)); // Flush escape keystrokes
    escape = false;

    // Start background music
    SoundModule::Start43AASound();
    nPhase = 1;

    // Record (music) start time
    clockStart = SDL_GetTicks();

    //InvalidateRect(NULL);
  } else {
    // Draw 43aa text polygons in correct position
    unsigned int clockNow = SDL_GetTicks();
    double dPhase = double(clockNow - clockStart) / 1000.0 / 12.0;

    Draw43AAInPhase(dPhase);

    // Check if it's time to split
    //if((dPhase > 1.0) || ::GetAsyncKeyState(VK_ESCAPE)) {
	if((dPhase > 1.0) || escape) {
      // End intro 

      // Switch to game menu
      m_pDrawFunction = &CGetawayView::OnDrawStartMenu;
      //m_pDrawFunction = &CGetawayView::OnDrawGame;
      m_bDrawOnlyMenu = false;
      //InvalidateRect(NULL);

      SoundModule::Stop43AASound();
      SoundModule::StartMenuMusic();

      // Try refreshing the screen to get the menu to be drawn correctly
      //DEVMODE devmode;
      //EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &devmode);
      //ChangeDisplaySettings(&devmode, CDS_FULLSCREEN);

      // Restore Cursor
      //ShowCursor(TRUE);
      SDL_ShowCursor(SDL_ENABLE);
      //wglMakeCurrent(hDC, NULL);
    } else {
      //InvalidateRect(NULL);
    }
  }

  //::ReleaseDC(GetSafeHwnd(), hDC);

  // Draw intro sequence
  /*
  static CFont fontBasic2;
  if(nPhase == 0) {
    // SoundModule::StartMessageSound();
    SoundModule::Start43AASound();
    ::SetCursor(NULL);
    while(::GetAsyncKeyState(VK_ESCAPE)); // Flush escape keystrokes
    CRect rectWnd;
    GetClientRect(&rectWnd);
    CBrush brushBlack(RGB(0, 0, 0));
    pDC->FillRect(&rectWnd, &brushBlack);

    CFont fontBasic1;
    fontBasic1.CreatePointFont(160, "Courier New", pDC);
    LOGFONT lf;
    fontBasic1.GetLogFont(&lf);
    lf.lfWeight = 700;
    fontBasic2.CreateFontIndirect(&lf);
    pDC->SelectObject(&fontBasic2);
    pDC->SetTextAlign(TA_LEFT);
    pDC->SetBkMode(TRANSPARENT);
    nPhase = 1;

    // Start paint timer
    SetTimer(1, 30, 0);
    return;
  }

  if(::GetAsyncKeyState(VK_ESCAPE)) {
    // Skip intro
    nPhase = 6;
    nCountDown = 0;
    return;
  }

  pDC->SelectObject(&fontBasic2);
  pDC->SetTextAlign(TA_LEFT);
  pDC->SetBkMode(TRANSPARENT);

  bool bPause = false;
  if(nPhase == 1) {    
    nChar = TypeInText(pDC, 2000, nChar, "V€H1CL€: ALL-T€RRA1N F0UR WH€€L DR1V€. AUT0MAT1C VAR1AT0R TRANSM1SS10N. 0N-D€MAND AUX1L1ARY J€T PR0PULS10N B00ST. 0N-D€MAND AUX1L1ARY H€L1R0T0R. V1SC0S1TY DAMP€D H1GH D€NS1TY CHASS1S. S0M€ SYST€MS G0.", bPause);
    if(nChar == 0) {
      nPhase = 2;
      nCountDown = 30;
    }
  }
  if(nPhase == 2) {    
    if(nCountDown) {      
      --nCountDown;
      return;
    }
    nChar = TypeInText(pDC, 2000, nChar, "L0CAT10N: C0V€R€D 1N L1GHT BR0WN SAND W1TH AN AV€RAG€ SUNK D€PTH F0R A STAT10NARY V€H1CL€ W€1GH1NG 900Kg W1TH A T0TAL 0F 50cm2 TRACT10N SURFAC€ 5cm. SL0P€ SURFAC€S €XC€€D1NG ANGL€ 0F 45° R€LAT1V€ T0 SURR0UND1NG WAT€R SURFAC€ D€NS€ P€TR1F1€D S€D1M€NT W1TH R€P€AT1NG PATT€RNS DAT1NG PR€-JURA P€R10D W1TH A 86% PR0BAB1L1TY. UNKN0WN STRUCTUR€ W1TH RUSTY 0UT€R10R. N0 S1GNS 0F €NTRANC€.", bPause);
    if(nChar == 0) {
      nPhase = 3;
      nCountDown = 30;
    }
  }

  if(nPhase == 3) {    
    if(nCountDown) {
      --nCountDown;
      return;
    }
    nChar = TypeInText(pDC, 2000, nChar, "S1GNS 0F L1F€: 0CCAS10NAL V1BRAT10NS 1N TH€ V€H1CL€. PR0BAB1L1TY THAT TH1S V1BRAT10N 1S CAUS€D BY W1ND AL0N€ 1S 98%. M1CR0B€S B0TH 1N WAT€R AND 0N TH€ 1SLAND SURFAC€.", bPause);
    if(nChar == 0) {
      nPhase = 4;
      nCountDown = 30;
    }
  }

  if(nPhase == 4) {    
    if(nCountDown) {
      --nCountDown;
      return;
    }
    nChar = TypeInText(pDC, 2000, nChar, "M1SS10N: UNKN0WN.", bPause);
    if(nChar == 0) {
      nPhase = 5;
      nCountDown = 30;
    }
  }

  if(nPhase == 5) {    
    if(nCountDown) {
      --nCountDown;
      return;
    }
    nChar = TypeInText(pDC, 2000, nChar, "1NF0JAB™ R€C€1V€D FR0M UN1T€D ST€€L AND PLAST€R, 1NC C€NTRAL L1NK DF3:213 - DATAP0UCH TRANSLAT10N BUFFER SUBN0RMAL", bPause);
    if(nChar == 0) {
      nPhase = 6;
      nCountDown = 30;
    }
  }

  if(nPhase == 6) {    
    if(nCountDown) {
      --nCountDown;
      return;
    }
    // Switch to game menu
    m_pDrawFunction = &CGetawayView::OnDrawStartMenu;
    m_bDrawOnlyMenu = false;
    InvalidateRect(NULL);

    // SoundModule::StopMessageSound();
    SoundModule::Stop43AASound();
    SoundModule::StartMenuMusic();

    // Kill paint timer
    KillTimer(1);

    // Restore Cursor
    if(m_hCursor) {
      ::SetCursor(m_hCursor);
    } else {
      ::SetCursor(LoadCursor(NULL, IDC_ARROW));
    }

  } else {
    if(bPause) {
      // Beep(100, 50);
      nCountDown = 10;
    }
    return;
  }
  */
}


//void CGetawayView::OnDrawCredits(CDC* pDC) {
void CGetawayView::OnDrawCredits() {
  //HDC hDC = pDC->GetSafeHdc();
  //wglMakeCurrent(hDC, NULL); 
  //ShowCursor(FALSE);
  //SDL_ShowCursor(SDL_DISABLE);
  //while(::GetAsyncKeyState(VK_ESCAPE)); // Flush escape keystrokes
  TestCredits();
  //ShowCursor(TRUE);
  //SDL_ShowCursor(SDL_ENABLE);
  //m_pDrawFunction = &CGetawayView::OnDrawStartMenu;
  //m_bDrawOnlyMenu = false;
  //m_bFullRedraw = true;
  //InvalidateRect(NULL);
}

void CGetawayView::OnDrawStartMenu() {
	/*glViewport(0, 0, (GLint) vi->current_w, (GLint) vi->current_h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, vi->current_w, vi->current_h, 0, 0, 1);
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClearColor(0.0, 0.0, 0.0, 1.0);
	
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	OpenGLHelpers::SetColorFull(1, 1, 1, 1);
	
	GLuint texture[1];
	
	// Draw graphics
	glGenTextures(1, &texture[0]);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, surface_start_menu_head->w, surface_start_menu_head->h, 0, GL_BGR, GL_UNSIGNED_BYTE, surface_start_menu_head->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	glBindTexture(GL_TEXTURE_2D, texture[0]);*/
	
	double x1 = window_width / 2 - 470/2;
	double y1 = window_height / 2 - 226;
	double x2 = x1 + 470;
	double y2 = y1 + 226;
	
	glClear(GL_COLOR_BUFFER_BIT);
	
	drawSurface(x1, y1, x2, y2, GL_BGR, surface_start_menu_head);
	
	/*glBegin(GL_QUADS);
	
	glTexCoord2f(0.0f, 0.0f);
	glVertex2f(x1, y1);
	glTexCoord2f(1.0f, 0.0f);
	glVertex2f(x2, y1);
	glTexCoord2f(1.0f, 1.0f);
	glVertex2f(x2, y2);
	glTexCoord2f(0.0f, 1.0f);
	glVertex2f(x1, y2);
	glEnd();
	
	glDeleteTextures(1, &texture[0]);*/
	
	
	
	// Draw copyright
	SDL_Surface *surface;
	SDL_Color color = {100, 100, 100};

	if(!(surface = TTF_RenderText_Blended(copyright_font, "© 2001 Mikko Oksalahti a.k.a. Metal Oxide. Visit www.nic.fi\\~moxide for latest version.", color)))
		cout << TTF_GetError() << endl;	
	
	/*glGenTextures(1, &texture[0]);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TextureImage[0]->w, TextureImage[0]->h, 0, GL_BGRA, GL_UNSIGNED_BYTE, TextureImage[0]->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);*/
	
	x1 = window_width / 2 - surface->w / 2;
	y1 = window_height - 2 * surface->h;
	x2 = x1 + surface->w;
	y2 = y1 + surface->h;
	
	drawSurface(x1, y1, x2, y2, GL_BGRA, surface);
	
	SDL_FreeSurface(surface);

	// Draw menu items
	DrawMenuItem(menu_font, 0, 0, "Begin", getColor(9*2/3, 115*2/3, 12*2/3));
	DrawMenuItem(menu_font, 1, 2, "Visuals", getColor(5*2/3, 85*2/3, 165*2/3));
	DrawMenuItem(menu_font, 2, 3, "Sounds", getColor(5*2/3, 85*2/3, 165*2/3));
	DrawMenuItem(menu_font, 3, 4, "Controls", getColor(5*2/3, 85*2/3, 165*2/3));
	DrawMenuItem(menu_font, 4, 5, "Credits", getColor(5*2/3, 85*2/3, 165*2/3));
	DrawMenuItem(menu_font, 5, 6, "Help", getColor(5*2/3, 85*2/3, 165*2/3));
	DrawMenuItem(menu_font, 6, 8, "Exit", getColor(135*2/3, 0, 0));
	
	drawDialogs();
	
	SDL_GL_SwapWindow(window);
	
	// Check whether a new update is needed to fade menu texts
	bool bUpdate = false;
	for(int m = 0; m < 7; ++m) {
		if((m_miMenu[m].m_nLight != 0) || (m_miMenu[m].m_nLight != 100)) {
			if(m_miMenu[m].m_bActive && (m_miMenu[m].m_nLight < 100)) {
				++(m_miMenu[m].m_nLight);
				bUpdate = true;  
			}
			if(!m_miMenu[m].m_bActive && (m_miMenu[m].m_nLight > 0)) {
				--(m_miMenu[m].m_nLight);
				bUpdate = true;  
			}
		}
	}
}

//void CGetawayView::OnDrawStartMenu(CDC* pDC) {
/*void CGetawayView::OnDrawStartMenu() {

  //if(m_hCursor) {
  //  ::SetCursor(m_hCursor);
  //}

  //HDC hDC = pDC->GetSafeHdc();  
  //wglMakeCurrent(hDC, m_hGLRC); // Setup as opengl dc to get the black and white stuff work

  SDL_Rect rectWnd;
  //GetClientRect(&rectWnd);
  //FIXME laita oikee reso
  rectWnd.w = 1024;
  rectWnd.h = 768;

  if(!m_bDrawOnlyMenu || m_bFullRedraw) {
    // Clear background
    CBrush brushBlack(RGB(0, 0, 0));
    pDC->FillRect(&rectWnd, &brushBlack);

    // Draw graphics
    static CBitmap bmpArt;
    static bool bLoaded = false;
    if(!bLoaded) {
      bmpArt.LoadBitmap(IDB_GAMEMENU);
      bLoaded = true;
    }

    CDC dcCopy;
    dcCopy.CreateCompatibleDC(pDC);
    dcCopy.SelectObject(&bmpArt);
    pDC->BitBlt(rectWnd.Width() / 2 - 470/2, rectWnd.Height() / 2 - 226, 470, 226, &dcCopy, 0, 0, SRCCOPY);

    // Draw copyright
    pDC->SetTextAlign(TA_CENTER);
    pDC->SetBkMode(TRANSPARENT);
    static CFont fontSmall;
    static bCreated2 = false;
    if(!bCreated2) {
      fontSmall.CreatePointFont(100, "Lucida Sans Unicode", pDC);
      bCreated2 = true;
    }

    pDC->SelectObject(&fontSmall);
    CSize size = pDC->GetTextExtent(_T("© 2001 Mikko"));
    pDC->SetTextColor(RGB(100, 100, 100));
    pDC->TextOut(rectWnd.Width() / 2, rectWnd.Height() - 2 * size.cy, _T("© 2001 Mikko Oksalahti a.k.a. Metal Oxide. Visit www.nic.fi\\~moxide for latest version."));

    m_bFullRedraw = false;
  }

  // Draw menu items
  static CFont font;
  static bCreated1 = false;
  if(!bCreated1) {
    if(rectWnd.Width() > 850) {
      // font.CreatePointFont(240, "Haettenschweiler", pDC);
	    font.CreatePointFont(200, "Impact", pDC);
    } else {
      // font.CreatePointFont(200, "Haettenschweiler", pDC);
	    font.CreatePointFont(160, "Impact", pDC);
    }
    bCreated1 = true;
  }
  pDC->SetTextAlign(TA_CENTER);
  pDC->SetBkMode(TRANSPARENT);
  pDC->SelectObject(&font);
  DrawMenuItem(pDC, &font, 0, 0, _T("Begin"), RGB(9*2/3, 115*2/3, 12*2/3), rectWnd);
  DrawMenuItem(pDC, &font, 1, 2, _T("Visuals"), RGB(5*2/3, 85*2/3, 165*2/3), rectWnd);
  DrawMenuItem(pDC, &font, 2, 3, _T("Sounds"), RGB(5*2/3, 85*2/3, 165*2/3), rectWnd);
  DrawMenuItem(pDC, &font, 3, 4, _T("Controls"), RGB(5*2/3, 85*2/3, 165*2/3), rectWnd);
  DrawMenuItem(pDC, &font, 4, 5, _T("Credits"), RGB(5*2/3, 85*2/3, 165*2/3), rectWnd);
  DrawMenuItem(pDC, &font, 5, 6, _T("Help"), RGB(5*2/3, 85*2/3, 165*2/3), rectWnd);
  DrawMenuItem(pDC, &font, 6, 8, _T("Exit"), RGB(135*2/3, 0, 0), rectWnd);

  m_bDrawOnlyMenu = false;

  // Check whether a new update is needed to fade menu texts
  bool bUpdate = false;
  for(int m = 0; m < 7; ++m) {
    if((m_miMenu[m].m_nLight != 0) || (m_miMenu[m].m_nLight != 100)) {
      if(m_miMenu[m].m_bActive && (m_miMenu[m].m_nLight < 100)) {
        ++(m_miMenu[m].m_nLight);
        bUpdate = true;  
      }
      if(!m_miMenu[m].m_bActive && (m_miMenu[m].m_nLight > 0)) {
        --(m_miMenu[m].m_nLight);
        bUpdate = true;  
      }
    }
  }
  if(bUpdate) {
    m_bDrawOnlyMenu = true;
    InvalidateRect(NULL);
  }

  ::ReleaseDC(GetSafeHwnd(), hDC);
}*/

void CGetawayView::DrawMenuItem(TTF_Font *font, int m, int nY, string text, SDL_Color color)
{
	GLuint texture[1];
	SDL_Surface *surface;
	
	if(m_miMenu[m].m_nLight > 0) {
		double dRed, dGreen, dBlue;
		dRed   = double(color.r);
		dGreen = double(color.g);
		dBlue  = double(color.b);

		double dLight = double(m_miMenu[m].m_nLight) / 100.0;

		dRed   = dRed   + dLight * 100.0;
		dGreen = dGreen + dLight * 100.0;
		dBlue  = dBlue  + dLight * 100.0;

		if(dRed > 255.0) {dRed = 255.0;}
		if(dGreen > 255.0) {dGreen = 255.0;}
		if(dBlue > 255.0) {dBlue = 255.0;}

		color.r = dRed;
		color.g = dGreen;
		color.b = dBlue;
	}
	
	if(!(surface = TTF_RenderText_Blended(font, text.c_str(), color)))
		cout << TTF_GetError() << endl;
	
	double x1 = window_width / 2 - surface->w / 2;
	double y1 = window_height / 2 + surface->h * nY;
	double x2 = x1 + surface->w;
	double y2 = y1 + surface->h;
	
	drawSurface(x1, y1, x2, y2, GL_BGRA, surface);
	
	SDL_FreeSurface(surface);
	
	m_miMenu[m].m_rect.SetRect(x1, y1, x2, y2);
}

/*void CGetawayView::DrawMenuItem(CDC* pDC, CFont *pFont, int m, int nY, CString sText, COLORREF color, CRect rectWnd) {
  if(m_miMenu[m].m_nLight > 0) {
    double dRed, dGreen, dBlue;
    dRed   = double(GetRValue(color));
    dGreen = double(GetGValue(color));
    dBlue  = double(GetBValue(color));

    double dLight = double(m_miMenu[m].m_nLight) / 100.0;

    dRed   = dRed   + dLight * 100.0;
    dGreen = dGreen + dLight * 100.0;
    dBlue  = dBlue  + dLight * 100.0;

    if(dRed > 255.0) {dRed = 255.0;}
    if(dGreen > 255.0) {dGreen = 255.0;}
    if(dBlue > 255.0) {dBlue = 255.0;}

    color = RGB(dRed, dGreen, dBlue);
  }
  CSize size = pDC->GetTextExtent(sText);
  CBitmap bmpSmall;
  bmpSmall.CreateCompatibleBitmap(pDC, size.cx, size.cy);
  CDC dcCopy;
  dcCopy.CreateCompatibleDC(pDC);
  dcCopy.SelectObject(&bmpSmall);
  CBrush brushBlack(RGB(0, 0, 0));
  dcCopy.FillRect(CRect(0, 0, size.cx, size.cy), &brushBlack);
  dcCopy.SetTextAlign(TA_LEFT);
  dcCopy.SetBkMode(TRANSPARENT);
  dcCopy.SelectObject(pFont);
  dcCopy.SetTextColor(color);
  dcCopy.TextOut(0, 0, sText);
  pDC->BitBlt(rectWnd.Width() / 2 - size.cx / 2, 
              rectWnd.Height() / 2 + size.cy * nY, 
              size.cx, size.cy, &dcCopy, 0, 0, SRCCOPY);


  m_miMenu[m].m_rect.SetRect(rectWnd.Width() / 2 - size.cx / 2,
                             rectWnd.Height() / 2 + size.cy * nY,
                             rectWnd.Width() / 2 + size.cx / 2,
                             rectWnd.Height() / 2 + size.cy * nY + size.cy);
}*/




double g_dRate = 60.0;

//void CGetawayView::OnDrawGame(CDC* pDC) {
void CGetawayView::OnDrawGame() {
  static int nFrameNo = 0;
  static unsigned int clockLastCheckPoint = SDL_GetTicks();
  static string sRate = "";

  if(m_bInitClock) {
    clockLastCheckPoint = SDL_GetTicks();
    m_bInitClock = false;
  }

  //CGetawayDoc* pDoc = GetDocument();
  //ASSERT_VALID(pDoc);

  //HDC hDC = pDC->GetSafeHdc();
  //wglMakeCurrent(hDC, m_hGLRC); 
  
  // Use OpenGL to draw the world

  glDrawBuffer(GL_BACK);

  // Reset OpenGL
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glScaled(-1.0, 1.0, 1.0);
  double dScreenFormat = 1.0;
  if(pDoc->GetSimulation()->m_nScreenFormat == 1) {
    dScreenFormat = 2.0 / 3.0;
  }
  double aspect = 1.0;
  aspect = (double) window_width / (double) (window_height * dScreenFormat);

  pDoc->GetSimulation()->PrePaint();

  BCamera *pCamera = pDoc->GetSimulation()->GetCamera();

  // double dScaler = 10.0; // To get better z-buffer performance on a Geforce MX2 card

  if(pCamera->m_locMode == BCamera::INCAR) {
    gluPerspective(pCamera->m_dAngleOfView, aspect, 0.06f, 15000.0f);
  } else if(pCamera->m_locMode == BCamera::OVERVIEW) {
    gluPerspective(pCamera->m_dAngleOfView, aspect, 2.0f, 15000.0f);
  } else {
    gluPerspective(pCamera->m_dAngleOfView, aspect, 0.5f, 15000.0f);
  }

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  // glScaled(dScaler, dScaler, dScaler);

  // Set look at -point for camera 
  glViewport(0, 
             (GLint) ((window_height - window_height * dScreenFormat) / 2), 
             (GLint) window_width, 
             (GLint) (double(window_height) * dScreenFormat));

  gluLookAt(pCamera->m_vLocation.m_dX, 
            pCamera->m_vLocation.m_dY, 
            pCamera->m_vLocation.m_dZ,
            pCamera->m_vLocation.m_dX + pCamera->m_orientation.m_vForward.m_dX, 
            pCamera->m_vLocation.m_dY + pCamera->m_orientation.m_vForward.m_dY, 
            pCamera->m_vLocation.m_dZ + pCamera->m_orientation.m_vForward.m_dZ, 
            pCamera->m_orientation.m_vUp.m_dX, 
            pCamera->m_orientation.m_vUp.m_dY, 
            pCamera->m_orientation.m_vUp.m_dZ);

  // Draw the world
  pDoc->GetSimulation()->Paint();

  // Calculate and draw framerate
  if(++nFrameNo == 10) {

    unsigned int clockNow = SDL_GetTicks();
    //g_dRate = 10.0 / (double(clockNow - (clockLastCheckPoint + m_nMenuTime)) / double(CLOCKS_PER_SEC));
    g_dRate = 10.0 / (double(clockNow - (clockLastCheckPoint)) / 1000.0);
    /*if(m_nMenuTime) {
      m_nMenuTime = 0;
    }*/

    pDoc->GetSimulation()->GetCar()->m_dSpeedKmh = pDoc->GetSimulation()->GetCar()->m_dSpeed * 
                                                   g_dRate * 
                                                   pDoc->GetSimulation()->m_nPhysicsStepsBetweenRender * 
                                                   3.6;
    /*sRate.Format("Framerate: %.1lf fps, Speed: %.1lf km/h, PhysicsSteps: %d, Altitude: %ld, Accel: %.2lf", 
                 g_dRate, 
                 pDoc->GetSimulation()->GetCar()->m_dSpeedKmh,
                 pDoc->GetSimulation()->m_nPhysicsStepsBetweenRender,
                 long(-pDoc->GetSimulation()->GetCar()->m_vLocation.m_dZ),
                 pDoc->GetSimulation()->GetCar()->m_dAccelerationFactor);*/

    // Calculate the absolute time sync ratio (ATSR)
    static int nSkipper = 0;
    if(++nSkipper >= 5) {
      nSkipper = 0;
      pDoc->GetSimulation()->m_nPhysicsStepsBetweenRender = int(g_dPhysicsStepsInSecond / g_dRate);
      pDoc->GetSimulation()->m_dPhysicsFraction           = g_dPhysicsStepsInSecond / g_dRate - double(int(g_dPhysicsStepsInSecond / g_dRate));
      pDoc->GetSimulation()->m_bCalibrateSimulationSpeed = false;
    }
    
    nFrameNo = 0;
    clockLastCheckPoint = clockNow;
  }

  /*if(pDoc->GetSimulation()->m_bDisplayInfo) {
	  m_glfont.Begin(rect.Width(), rect.Height());
	  glColor3d(0, 0, 0);
	  m_glfont.SetPosition(10, rect.Height() - 20);
	  m_glfont.Print(sRate);
    m_glfont.End();
  }*/

	drawDialogs();

  glFinish();

  //SwapBuffers(pDC->GetSafeHdc());
  SDL_GL_SwapWindow(window);


  //::ReleaseDC(GetSafeHwnd(), hDC);

  // Sleep(1);
  /*if(!pDoc->GetSimulation()->m_bPaused) {
    Invalidate();
  }*/
}




/////////////////////////////////////////////////////////////////////////////
// CGetawayView drawing

//void CGetawayView::OnDraw(CDC* pDC) {
void CGetawayView::OnDraw() {
  if(m_pDrawFunction) {
    // Call active draw function
    (this->*m_pDrawFunction)();
  }
}

/////////////////////////////////////////////////////////////////////////////
// CGetawayView diagnostics

/*#ifdef _DEBUG
void CGetawayView::AssertValid() const
{
CView::AssertValid();
}

void CGetawayView::Dump(CDumpContext& dc) const
{
CView::Dump(dc);
}

CGetawayDoc* CGetawayView::GetDocument() // non-debug version is inline
{
ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGetawayDoc)));
return (CGetawayDoc*)m_pDocument;
}
#endif //_DEBUG*/

/////////////////////////////////////////////////////////////////////////////
// CGetawayView message handlers

void CGetawayView::OnKeyDown(int nChar) {

  if(m_pDrawFunction != &CGetawayView::OnDrawGame) {
	  if (nChar == SDLK_ESCAPE)
		escape = true;
    return;
  }

  static bool bOpen = false;
  //CGetawayDoc* pDoc = GetDocument();
  // Check for user input
  //DlgMainMenu dialogMenu;
  unsigned int clockStart;

  if(pDoc->GetSimulation()->GetCamera()->m_bNeedsToBeInitialized) {
    pDoc->GetSimulation()->GetCamera()->m_locMode = BCamera::FOLLOW;
    pDoc->GetSimulation()->GetCamera()->m_dAngleOfView = 75.0;
    pDoc->GetSimulation()->GetCamera()->m_bNeedsToBeInitialized = false;
  }

  if(nChar == ControllerModule::m_keymap.m_unAccelerate) {
    pDoc->GetSimulation()->GetCar()->m_bAccelerating = true;
    pDoc->GetSimulation()->GetCar()->m_dAccelerationFactor = 1.0;
    if(pDoc->GetSimulation()->GetCar()->m_nHeliMode > 399) {
      pDoc->GetSimulation()->GetCar()->m_bHeliForwarding = true;
    }
  } else if(nChar == ControllerModule::m_keymap.m_unReverse) {
    pDoc->GetSimulation()->GetCar()->m_bReversing = true;
    if(pDoc->GetSimulation()->GetCar()->m_nHeliMode > 399) {
      pDoc->GetSimulation()->GetCar()->m_bHeliBacking = true;
    }
  } else if(nChar == ControllerModule::m_keymap.m_unBreak) {
    pDoc->GetSimulation()->GetCar()->m_bBreaking = true;
  } else if(nChar == ControllerModule::m_keymap.m_unLeft) {
    pDoc->GetSimulation()->GetCar()->m_bTurningLeft = true;
    if(pDoc->GetSimulation()->GetCar()->m_nHeliMode > 399) {
      pDoc->GetSimulation()->GetCar()->m_bHeliLefting = true;
    }
  } else if(nChar == ControllerModule::m_keymap.m_unRight) {
    pDoc->GetSimulation()->GetCar()->m_bTurningRight = true;
    if(pDoc->GetSimulation()->GetCar()->m_nHeliMode > 399) {
      pDoc->GetSimulation()->GetCar()->m_bHeliRighting = true;
    }
  } else if(nChar == ControllerModule::m_keymap.m_unCamera) {
    pDoc->GetSimulation()->SwitchCameraMode();
  } else if(nChar == ControllerModule::m_keymap.m_unLift) {
    if(pDoc->GetSimulation()->GetCar()->m_nHeliMode > 0) {
      pDoc->GetSimulation()->GetCar()->m_bHeliLifting = true;
    } else if(!pDoc->GetSimulation()->m_bLiftingUp) {
      pDoc->GetSimulation()->m_bLiftingUp = true;
      pDoc->GetSimulation()->m_dLiftZ     = pDoc->GetSimulation()->GetCar()->m_pPoint[0].m_vLocation.m_dZ;
    }
  } else if(nChar == ControllerModule::m_keymap.m_unHeli) {
    pDoc->GetSimulation()->ToggleHeli();
  } else if(nChar == ControllerModule::m_keymap.m_unJet) {
    pDoc->GetSimulation()->ToggleJet();
  } else {
    switch(nChar) {
      case SDLK_ESCAPE:
        if (!isDialogOpen<DlgMainMenu *>()) {
          dialogs.push_back(new DlgMainMenu(this));
          SDL_ShowCursor(SDL_ENABLE);
          pDoc->GetSimulation()->m_bPaused = true;
        }
        //dialog = new DlgMainMenu(this);
        /*pDoc->GetSimulation()->m_bPaused = true;
        dialogMenu.SetTitle("menu", 23);
        clockStart = clock();
        dialogMenu.DoModal(pDoc->GetSimulation());
        m_nMenuTime += clock() - clockStart;
        pDoc->GetSimulation()->m_bPaused = false;*/
        break;
      case SDLK_SPACE:
        pDoc->GetSimulation()->GetCar()->m_bHandBreaking = true;
        break;
      case SDLK_LSHIFT:
        pDoc->GetSimulation()->GetCar()->m_bABS = true;
        break;
      case SDLK_HOME:
        pDoc->GetSimulation()->GetCar()->LoadCarFromFile("./World/CarJetHeli.gaw");
        pDoc->GetSimulation()->GetCar()->Move(pDoc->GetSimulation()->GetCar()->GetHomeLocation());
        break;
      case SDLK_DELETE:
        pDoc->GetSimulation()->GetCar()->LoadCarFromFile("./World/CarJetHeli.gaw");
        pDoc->GetSimulation()->GetCar()->Move(BVector(0, 0, -1000.0));
        break;
      case SDLK_F1:
        pDoc->GetSimulation()->GetCamera()->m_locMode = BCamera::FOLLOW;
        pDoc->GetSimulation()->GetCamera()->m_dAngleOfView = 75.0;
        break;
      case SDLK_F2:
        pDoc->GetSimulation()->GetCamera()->m_locMode = BCamera::OVERVIEW;
        pDoc->GetSimulation()->GetCamera()->m_bInitLoc = true;
        pDoc->GetSimulation()->GetCamera()->m_dAngleOfView = 58.0;
        break;
      case SDLK_F3:
        pDoc->GetSimulation()->GetCamera()->m_locMode = BCamera::INCAR;
        pDoc->GetSimulation()->GetCamera()->m_dAngleOfView = 75.0;
        break;
      case SDLK_F4:
        pDoc->GetSimulation()->GetCamera()->m_locMode = BCamera::ONSIDE;
        pDoc->GetSimulation()->GetCamera()->m_dAngleOfView = 75.0;
        break;
      case SDLK_F5:
        pDoc->GetSimulation()->GetCamera()->m_locMode = BCamera::FIXED;
        pDoc->GetSimulation()->GetCamera()->m_vFixLocation = pDoc->GetSimulation()->GetCamera()->m_vLocation;
        pDoc->GetSimulation()->GetCamera()->m_dAngleOfView = 70.0;
        break;
      case SDLK_i:
        pDoc->GetSimulation()->m_bDisplayInfo = !pDoc->GetSimulation()->m_bDisplayInfo;
        break;
      case SDLK_d:
        pDoc->GetSimulation()->m_bRaining = !pDoc->GetSimulation()->m_bRaining;
        break;
      case SDLK_F9:
        pDoc->GetSimulation()->m_bSteeringAidOn = !pDoc->GetSimulation()->m_bSteeringAidOn;
        break;
      case SDLK_PLUS:
        pDoc->GetSimulation()->m_dAccelerationFactor *= 2.0;
        break;
      case SDLK_MINUS:
        pDoc->GetSimulation()->m_dAccelerationFactor /= 2.0;
        break;
    }
  }
  //CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CGetawayView::OnKeyUp(int nChar) {
  //CGetawayDoc* pDoc = GetDocument();

  if(nChar == ControllerModule::m_keymap.m_unAccelerate) {
    pDoc->GetSimulation()->GetCar()->m_bAccelerating = false;
    pDoc->GetSimulation()->GetCar()->m_dAccelerationFactor = 0.0;
    if(pDoc->GetSimulation()->GetCar()->m_nHeliMode > 399) {
      pDoc->GetSimulation()->GetCar()->m_bHeliForwarding = false;
    }
  } else if(nChar == ControllerModule::m_keymap.m_unReverse) {
    pDoc->GetSimulation()->GetCar()->m_bReversing = false;
    if(pDoc->GetSimulation()->GetCar()->m_nHeliMode > 399) {
      pDoc->GetSimulation()->GetCar()->m_bHeliBacking = false;
    }
  } else if(nChar == ControllerModule::m_keymap.m_unBreak) {
    pDoc->GetSimulation()->GetCar()->m_bBreaking = false;
  } else if(nChar == ControllerModule::m_keymap.m_unLeft) {
    pDoc->GetSimulation()->GetCar()->m_bTurningLeft = false;
    if(pDoc->GetSimulation()->GetCar()->m_nHeliMode > 399) {
      pDoc->GetSimulation()->GetCar()->m_bHeliLefting = false;
    }
  } else if(nChar == ControllerModule::m_keymap.m_unRight) {
    pDoc->GetSimulation()->GetCar()->m_bTurningRight = false;
    if(pDoc->GetSimulation()->GetCar()->m_nHeliMode > 399) {
      pDoc->GetSimulation()->GetCar()->m_bHeliRighting = false;
    }
  } else if(nChar == ControllerModule::m_keymap.m_unLift) {
    if(pDoc->GetSimulation()->GetCar()->m_nHeliMode > 399) {
      pDoc->GetSimulation()->GetCar()->m_bHeliLifting = false;
    } else {
      pDoc->GetSimulation()->m_bLiftingUp = false;
    }
  } else {
    switch(nChar) {
      case SDLK_SPACE:
        pDoc->GetSimulation()->GetCar()->m_bHandBreaking = false;
        break;
      case SDLK_LSHIFT:
        pDoc->GetSimulation()->GetCar()->m_bABS = false;
        break;
        break;
    }
  }
  //CView::OnKeyUp(nChar, nRepCnt, nFlags);
}

//void CGetawayView::OnMouseMove(UINT nFlags, CPoint point) {
void CGetawayView::OnMouseMove(int x, int y) {
	if (dialogs.size() > 0) {
		vector<Dialog *>::iterator i = --dialogs.end();
    bool allow_continue = (*i)->onMouseMove(x - (*i)->getPos()->x, y - (*i)->getPos()->y);
    if (!allow_continue) {
      return;
    }
	}
  if(m_pDrawFunction != &CGetawayView::OnDrawStartMenu) {
    return;
  }
  bool bChanged = false;
  for(int m = 0; m < 7; ++m) {
    if(m_miMenu[m].m_rect.PtInRect(x, y)) {
      // Highlight menu item
      if(!m_miMenu[m].m_bActive) {
        m_miMenu[m].m_bActive = true;
        bChanged = true;
      }
    } else {
      if(m_miMenu[m].m_bActive) {
        m_miMenu[m].m_bActive = false;
        bChanged = true;
      }
    }
  }

  /*if(bChanged) {
    m_bDrawOnlyMenu = true;
    InvalidateRect(NULL);
  }*/
}


//void CGetawayView::OnLButtonDown(UINT nFlags, CPoint point) {
void CGetawayView::OnLButtonDown(int x, int y) {
	if (dialogs.size() > 0) {
		vector<Dialog *>::iterator i = --dialogs.end();
    bool allow_continue = (*i)->onMousePress(x - (*i)->getPos()->x, y - (*i)->getPos()->y);
    if (!allow_continue) {
      return;
    }
	}
  if(m_pDrawFunction != &CGetawayView::OnDrawStartMenu) {
    return;
  }
  //CGetawayDoc* pDoc = GetDocument();
  for(int m = 0; m < 7; ++m) {
    if(m_miMenu[m].m_rect.PtInRect(x, y)) {
      // Run menu command
	    //DlgVisuals dialog1;
	    //DlgSounds dialog2;
	    //DlgControls dialog3;
      BVector vCurrent;
      switch(m) {
        case 0:
          // Begin
          SoundModule::StopMenuMusic();
          // Setup car in the starting position
          pDoc->GetSimulation()->GetCamera()->SetInitialLocation();
          pDoc->GetSimulation()->GetCar()->LoadCarFromFile("./World/CarJetHeli.gaw");
          pDoc->GetSimulation()->SetUpCamera();
          vCurrent = pDoc->GetSimulation()->GetCar()->m_vLocation * -1.0;
          pDoc->GetSimulation()->GetCar()->Move(vCurrent);
          pDoc->GetSimulation()->GetCar()->Move(pDoc->GetSimulation()->GetCar()->m_vHomeLocation);
          m_bInitClock = true;

          // Switch to game resolution
          /*DEVMODE devmode;
          EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &devmode);
          if((devmode.dmPelsWidth != (DWORD)pDoc->GetSimulation()->m_nDispWidth) || 
             (devmode.dmPelsHeight != (DWORD)pDoc->GetSimulation()->m_nDispHeight) || 
             (devmode.dmBitsPerPel != (DWORD)pDoc->GetSimulation()->m_nDispBits) ||
             (devmode.dmDisplayFrequency != (DWORD)pDoc->GetSimulation()->m_nDispHz)) {
            devmode.dmPelsWidth = (DWORD)pDoc->GetSimulation()->m_nDispWidth;
            devmode.dmPelsHeight = (DWORD)pDoc->GetSimulation()->m_nDispHeight;
            devmode.dmBitsPerPel = (DWORD)pDoc->GetSimulation()->m_nDispBits;
            devmode.dmDisplayFrequency = (DWORD)pDoc->GetSimulation()->m_nDispHz;
            ChangeDisplaySettings(&devmode, CDS_FULLSCREEN);
          }
          AfxGetMainWnd()->SetWindowPos(NULL, -2, -2, pDoc->GetSimulation()->m_nDispWidth + 4, pDoc->GetSimulation()->m_nDispHeight + 4, 0);*/

          // Take controller into use, if not done already
          //if(!ControllerModule::m_bCurrentInitialized) {
          //  ControllerModule::SwitchToController(ControllerModule::m_nCurrent);
          //}

          // Start game
          SoundModule::StartSkidSound();
          SoundModule::StartEngineSound();
          pDoc->GetSimulation()->GetCamera()->m_bNeedsToBeInitialized = true;
          m_pDrawFunction = &CGetawayView::OnDrawGame;
          //ShowCursor(FALSE);
          SDL_ShowCursor(SDL_DISABLE);
          if (touch_seen && !isDialogOpen<DlgOnScreenKbd *>()) {
            dialogs.push_back(new DlgOnScreenKbd(this));
          }
          //Invalidate();
          break;
        case 1:
          // Visuals
          /*dialog1.SetTitle("visuals", 42, 0.43, TA_LEFT);
	        dialog1.DoModal(pDoc->GetSimulation());	
          m_bDrawOnlyMenu = false;
          m_bFullRedraw = true;
          InvalidateRect(NULL);*/
          dialogs.push_back(new DlgVisuals(this));
          break;
        case 2:
          // Sounds
          /*dialog2.SetTitle("sounds", 42, 0.43, TA_LEFT);
	        dialog2.DoModal(pDoc->GetSimulation(), true);	
          m_bDrawOnlyMenu = false;
          m_bFullRedraw = true;
          InvalidateRect(NULL);*/
          dialogs.push_back(new DlgSounds(this));
          break;
        case 3:
          // Controls
          /*dialog3.SetTitle("controls", 38, 0.31, TA_LEFT);
	        dialog3.DoModal(pDoc->GetSimulation());	
          m_bDrawOnlyMenu = false;
          m_bFullRedraw = true;
          InvalidateRect(NULL);*/
          dialogs.push_back(new DlgControls(this));
          break;
        case 4:
          // Credits
          loadCredits();
          SDL_ShowCursor(SDL_DISABLE);
          escape = false;
          m_pDrawFunction = &CGetawayView::OnDrawCredits;
          //InvalidateRect(NULL);
          break;
        case 5:
          // Help (invocation technique courtesy of Mr. Jacob Cody)
          /*{
            char sCurDir[1024];
            GetCurrentDirectory(1024, sCurDir);
            CString sHelpPath = sCurDir;
            sHelpPath += "\\Help\\help.html";
            HINSTANCE hi;
            hi = ShellExecute(::GetDesktopWindow(), 
                              NULL, 
                              sHelpPath, 
                              NULL, 
                              NULL, 
                              SW_MAXIMIZE);
          }*/
          break;
        case 6:
          // Exit
          SoundModule::StopMenuMusic();
          //AfxGetMainWnd()->PostMessage(WM_COMMAND, ID_FILE_CLOSE, 0);
          exit = true;
          break;
      }
    }
  }
}

void CGetawayView::OnLButtonUp(int x, int y) {
	if (dialogs.size() > 0) {
		vector<Dialog *>::iterator i = --dialogs.end();
    bool allow_continue = (*i)->onMouseRelease(x - (*i)->getPos()->x, y - (*i)->getPos()->y);
    if (!allow_continue) {
      return;
    }
	}
}

void CGetawayView::OnFingerDown(float x, float y, int finger_id) {
  if (dialogs.size() > 0) {
    vector<Dialog *>::iterator i = --dialogs.end();
    (*i)->onFingerDown(x * window_width - (*i)->getPos()->x, y * window_height - (*i)->getPos()->y, finger_id);
  } else if (!isDialogOpen<DlgOnScreenKbd *>() && m_pDrawFunction == &CGetawayView::OnDrawGame) {
    dialogs.push_back(new DlgOnScreenKbd(this));
  }
  touch_seen = true;
}

void CGetawayView::OnFingerUp(float x, float y, int finger_id) {
  if (dialogs.size() > 0) {
    vector<Dialog *>::iterator i = --dialogs.end();
    (*i)->onFingerUp(x * window_width - (*i)->getPos()->x, y * window_height - (*i)->getPos()->y, finger_id);
  }
}

void CGetawayView::drawDialogs() {
	for (int x = 0; x < dialogs.size(); x++) {
	if (dialogs[x]->isExit()) {
		  delete dialogs[x];
		  dialogs[x] = NULL;
		  dialogs.erase(dialogs.begin()+x);
		  x--;
		  if (!isDialogOpen<DlgMainMenu *>() && m_pDrawFunction == &CGetawayView::OnDrawGame) {
			  SDL_ShowCursor(SDL_DISABLE);
		  }
		} else {
			SDL_Surface *surface = dialogs[x]->getSurface();
			SDL_Rect *r = dialogs[x]->getPos();
			if (dialogs[x]->isDefaultPos()) {
				r->x = window_width / 2 - surface->w / 2;
				r->y = window_height / 2 - surface->h / 2;
			}
			double x1 = r->x;
			double y1 = r->y;
			double x2 = r->x + r->w;
			double y2 = r->y + r->h;
			
			drawSurface(x1, y1, x2, y2, GL_RGBA, surface);
		}
	}
}

template<typename D> vector<Dialog *>::iterator CGetawayView::getFirstDialogOfType() {
  vector<Dialog *>::iterator i;
  for (i = dialogs.begin(); i != dialogs.end(); i++) {
    if (dynamic_cast<D>(*i)) {
      break;
    }
  }
  return i;
}

template<typename D> bool CGetawayView::isDialogOpen() {
  return getFirstDialogOfType<D>() != dialogs.end();
}

void CGetawayView::drawSurface(double x1, double y1, double x2, double y2, GLenum format, SDL_Surface *surface) {
	glViewport(0, 0, (GLint) window_width, (GLint) window_height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, window_width, window_height, 0, 0, 1);
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClearColor(0.0, 0.0, 0.0, 1.0);

	//glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	OpenGLHelpers::SetColorFull(1, 1, 1, 1);

  unsigned short row_data_width = surface->pitch / surface->format->BytesPerPixel;
	GLuint texture[1];
	glGenTextures(1, &texture[0]);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, row_data_width, surface->h, 0, format, GL_UNSIGNED_BYTE, surface->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texture[0]);

	glBegin(GL_QUADS);
	//glColor3f(0.0, 0.0, 0.0);
	glTexCoord2f(0.0f, 0.0f);
	glVertex2f(x1, y1);
	glTexCoord2f((float) surface->w / row_data_width, 0.0f);
	glVertex2f(x2, y1);
	glTexCoord2f((float) surface->w / row_data_width, 1.0f);
	glVertex2f(x2, y2);
	glTexCoord2f(0.0f, 1.0f);
	glVertex2f(x1, y2);
	glEnd();

	glDeleteTextures(1, &texture[0]);
}

void CGetawayView::loadStartMenu() {
	surface_start_menu_head = SDL_LoadBMP("res/MenuFinal.bmp");
	if (surface_start_menu_head == NULL)
		cout << "Cannot load file res/MenuFinal.bmp!" << endl;
	
	vector<string> copyright_fonts;
	copyright_fonts.push_back("FreeSans.ttf");
	copyright_fonts.push_back("DejaVuSans.ttf");
	copyright_fonts.push_back("l_10646.ttf");
	copyright_font = openFont(copyright_fonts, 16);
	if (!copyright_font)
		cout << "TTF_OpenFont is broken!" << endl;
	
	vector<string> menu_fonts;
	menu_fonts.push_back("impact.ttf");
	menu_fonts.push_back("FreeSans.ttf");
	menu_fonts.push_back("DejaVuSans.ttf");
	menu_font = openFont(menu_fonts, 26); // size == 200
	if (!menu_font)
		cout << "TTF_OpenFont is broken!" << endl;
	//TTF_CloseFont(menu_font);
	/*tex_start_menu_head = new GLubyte[(tex_s->w * tex_s->h * 3) * 2];
	memmove((void *)tex_start_menu_head, tex_s->pixels, tex_s->w * tex_s->h * 3);
	OpenGLHelpers::CreateMipMaps(tex_start_menu_head, tex_s->w, tex_s->h, 3);
	OpenGLHelpers::DefineMipMapTextures(tex_s->w, tex_s->h, 3, GL_BGR, tex_start_menu_head, 11);*/
	//SDL_FreeSurface(tex_s);
}

void CGetawayView::loadCredits() {
	last_angle = 160.0;
	int width = 300;
	int height = 300;
	credits_glow = SDL_CreateRGBSurface(0, width, height, 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
	
	SDL_Surface *val = SDL_LoadBMP("res/bitmap1.bmp");
	if (val == NULL)
		cout << "Cannot load file res/bitmap1.bmp!" << endl;
	credits_bitmap = SDL_CreateRGBSurface(0, val->w, val->h, 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
	SDL_BlitSurface(val, NULL, credits_bitmap, NULL);
	
	vector<string> fontSmalls;
	fontSmalls.push_back("FreeSans.ttf");
	fontSmalls.push_back("DejaVuSans.ttf");
	fontSmalls.push_back("l_10646.ttf");
	fontSmall = openFont(fontSmalls, 18);
	
	vector<string> fontBigs;
	fontBigs.push_back("ariblk.ttf");
	fontBigs.push_back("FreeSans.ttf");
	fontBigs.push_back("DejaVuSans.ttf");
	fontBig = openFont(fontBigs, 29);
	if (!fontSmall || !fontBig)
		cout << "TTF_OpenFont is broken!" << endl;
	start_ticks = SDL_GetTicks();
}

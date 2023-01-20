// GetawayView.h : interface of the CGetawayView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_GETAWAYVIEW_H__73E6FC07_BC82_11D4_B532_0060B0F1F5DD__INCLUDED_)
#define AFX_GETAWAYVIEW_H__73E6FC07_BC82_11D4_B532_0060B0F1F5DD__INCLUDED_

//#if _MSC_VER > 1000
//#pragma once
//#endif // _MSC_VER > 1000

//#include "STRMIF.H"
//#include "CONTROL.H"
//#include "UUIDS.H"
#include "BaseClasses.h"

class CRect 
{
private:
	int x1, y1, x2, y2;
public:
	void SetRectEmpty() {
		x1 = 0;
		y1 = 0;
		x2 = 0;
		y2 = 0;
	}
	
	void SetRect(int x1, int y1, int x2, int y2) {
		this->x1 = x1;
		this->y1 = y1;
		this->x2 = x2;
		this->y2 = y2;
	}
	
	bool PtInRect(int x, int y) {
		return (x > x1 && x < x2 && y > y1 && y < y2);
	}
};

class BMenuItem {
public:
  CRect   m_rect;
  bool    m_bActive;
  int     m_nLight;
  BMenuItem() {m_rect.SetRectEmpty(); m_bActive = false; m_nLight = 0;}
};


//class CGetawayView : public CView {
class CGetawayView {
public:
	bool exit;
	CGetawayDoc* pDoc;

  GLubyte m_d43AATexture[(512 * 64 * 3) * 2]; // reserve room also for mipmaps

  bool    m_bInitClock;

  //HCURSOR m_hCursor;

  BMenuItem m_miMenu[7];

  int window_width, window_height;

  /*GLFont  m_glfont;
  HGLRC   m_hGLRC;
  clock_t m_nMenuTime; // Time spent viewing menu

  int     m_chaMenuMusic; // Channel for the menu background music

  void DrawMenuItem(CDC* pDC, CFont *pFont, int m, int nY, CString sText, COLORREF color, CRect rectWnd);*/
  void DrawMenuItem(TTF_Font *font, int m, int nY, string text, SDL_Color textColor);
  //void DrawPlanetGlow(CDC *pDC, int nX, int nY, int nRad);
  void DrawPlanetGlow(double current, int nRad);
  //void FadeInText(CDC *pDC, CString s1, CString s2, CString s3,
  //                int nFadeDelay, int nDelay, 
  //                CFont *pfont1, CFont *pfont2,
  //                CBitmap *pbmpArt);
	void FadeInText(string s1, string s2, string s3, int ticks, TTF_Font *pfont1, TTF_Font *pfont2);
  //int TypeInText(CDC *pDC, int nWaitms, int nChar, char *sText, bool &rbPauseAWhile);

  void TestCredits();

  void Load43AATexture();
  void Draw43AAInPhase(double dPhase);

public:
  bool m_bDrawOnlyMenu;
  bool m_bFullRedraw;

  //void OnDrawIntro(CDC* pDC);
  void OnDrawIntro();
  //void OnDrawCredits(CDC* pDC);
  void OnDrawCredits();
  //void OnDrawStartMenu(CDC* pDC);
  void OnDrawStartMenu();
  //void OnDrawGame(CDC* pDC);
  void OnDrawGame();
private:

  //IMediaControl *m_pMediaControl;

//protected: // create from serialization only
public:
  CGetawayView();
  //DECLARE_DYNCREATE(CGetawayView)

// Attributes
public:
  //void (CGetawayView::*m_pDrawFunction)(CDC*);
  void (CGetawayView::*m_pDrawFunction)();

  //CGetawayDoc* GetDocument();

// Operations
public:

// Overrides
// ClassWizard generated virtual function overrides
//{{AFX_VIRTUAL(CGetawayView)
public:
//virtual void OnDraw(CDC* pDC);  // overridden to draw this view
virtual void OnDraw();  // overridden to draw this view
//virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
//afx_msg void OnTimer(UINT nIDEvent);
protected:
//}}AFX_VIRTUAL

void InitializeOpenGL();

// Implementation
public:
  virtual ~CGetawayView();
  /*#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
  #endif*/

protected:

// Generated message map functions
public:
  //{{AFX_MSG(CGetawayView)
  //afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
  void  OnCreate();
  //afx_msg BOOL OnEraseBkgnd(CDC* pDC);
  //afx_msg void OnDestroy();
  //afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
  void OnKeyDown(int nChar);
  //afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
  void OnKeyUp(int nChar);
  //afx_msg void OnMouseMove(UINT nFlags, CPoint point);
  void OnMouseMove(int x, int y);
  //afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
  void OnLButtonDown(int x, int y);
  //}}AFX_MSG
  //DECLARE_MESSAGE_MAP()
  void OnLButtonUp(int x, int y);
  void OnFingerDown(float x, float y, int finger_id);
  void OnFingerUp(float x, float y, int finger_id);
  bool touch_seen;
  
  vector<Dialog *> dialogs;
  
  void drawDialogs();

  template<typename D> vector<Dialog *>::iterator getFirstDialogOfType();
  template<typename D> bool isDialogOpen();
  
  void drawSurface(double x1, double y1, double x2, double y2, GLenum format, SDL_Surface *surface);
  
  bool escape;
  
  SDL_Surface *surface_start_menu_head;
  TTF_Font *copyright_font;
  TTF_Font *menu_font;
  void loadStartMenu();
  
  double last_angle;
  SDL_Surface *credits_glow;
  SDL_Surface *credits_bitmap;
  TTF_Font *fontSmall;
  TTF_Font *fontBig;
  int start_ticks;
  void loadCredits();
  SDL_Window *window;

  void setWindow(SDL_Window *window) {
    this->window = window;
  }
};

/*#ifndef _DEBUG  // debug version in GetawayView.cpp
inline CGetawayDoc* CGetawayView::GetDocument()
   { return (CGetawayDoc*)m_pDocument; }
#endif*/

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GETAWAYVIEW_H__73E6FC07_BC82_11D4_B532_0060B0F1F5DD__INCLUDED_)

// GetawayDoc.h : interface of the CGetawayDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_GETAWAYDOC_H__73E6FC05_BC82_11D4_B532_0060B0F1F5DD__INCLUDED_)
#define AFX_GETAWAYDOC_H__73E6FC05_BC82_11D4_B532_0060B0F1F5DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BSimulation.h"

//class CGetawayDoc : public CDocument {
class CGetawayDoc {

  BSimulation m_simulation;

//protected: // create from serialization only
public:
	//CGetawayDoc();
	//DECLARE_DYNCREATE(CGetawayDoc)

// Attributes
//public:

// Operations
public:

  BSimulation *GetSimulation() {return &m_simulation;};

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGetawayDoc)
	public:
	//virtual bool OnNewDocument();
	//virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	//virtual ~CGetawayDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CGetawayDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	//DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GETAWAYDOC_H__73E6FC05_BC82_11D4_B532_0060B0F1F5DD__INCLUDED_)

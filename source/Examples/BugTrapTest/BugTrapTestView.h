// BugTrapTestView.h : interface of the CBugTrapTestView class
//

#pragma once

// this is required to bypass VS.NET parser issues
#ifdef __NEVER_DEFINED__
 #define BTWindow<CView> CView
#endif

class CBugTrapTestView : public BTWindow<CView>
{
protected: // create from serialization only
	CBugTrapTestView();
	DECLARE_DYNCREATE(CBugTrapTestView)

// Attributes
public:
	CBugTrapTestDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CBugTrapTestView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in BugTrapTestView.cpp
inline CBugTrapTestDoc* CBugTrapTestView::GetDocument() const
   { return reinterpret_cast<CBugTrapTestDoc*>(m_pDocument); }
#endif

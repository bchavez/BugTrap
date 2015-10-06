// BugTrapTestView.cpp : implementation of the CBugTrapTestView class
//

#include "stdafx.h"
#include "BugTrapTest.h"

#include "BugTrapTestDoc.h"
#include "BugTrapTestView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CBugTrapTestView

IMPLEMENT_DYNCREATE(CBugTrapTestView, CView)

BEGIN_MESSAGE_MAP(CBugTrapTestView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CBugTrapTestView construction/destruction

CBugTrapTestView::CBugTrapTestView()
{
	// TODO: add construction code here
}

CBugTrapTestView::~CBugTrapTestView()
{
}

BOOL CBugTrapTestView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CBugTrapTestView drawing

void CBugTrapTestView::OnDraw(CDC* /*pDC*/)
{
	CBugTrapTestDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc;

	// TODO: add draw code for native data here
}

// CBugTrapTestView printing

BOOL CBugTrapTestView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CBugTrapTestView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CBugTrapTestView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

// CBugTrapTestView diagnostics

#ifdef _DEBUG
void CBugTrapTestView::AssertValid() const
{
	CView::AssertValid();
}

void CBugTrapTestView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CBugTrapTestDoc* CBugTrapTestView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBugTrapTestDoc)));
	return (CBugTrapTestDoc*)m_pDocument;
}
#endif //_DEBUG

// CBugTrapTestView message handlers

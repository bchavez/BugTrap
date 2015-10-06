// BugTrapTestDoc.cpp : implementation of the CBugTrapTestDoc class
//

#include "stdafx.h"
#include "BugTrapTest.h"

#include "BugTrapTestDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBugTrapTestDoc

IMPLEMENT_DYNCREATE(CBugTrapTestDoc, CDocument)

BEGIN_MESSAGE_MAP(CBugTrapTestDoc, CDocument)
END_MESSAGE_MAP()


// CBugTrapTestDoc construction/destruction

CBugTrapTestDoc::CBugTrapTestDoc()
{
	// TODO: add one-time construction code here

}

CBugTrapTestDoc::~CBugTrapTestDoc()
{
}

BOOL CBugTrapTestDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CBugTrapTestDoc serialization

void CBugTrapTestDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CBugTrapTestDoc diagnostics

#ifdef _DEBUG
void CBugTrapTestDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CBugTrapTestDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CBugTrapTestDoc commands

// BugTrapTestDoc.h : interface of the CBugTrapTestDoc class
//


#pragma once

class CBugTrapTestDoc : public CDocument
{
protected: // create from serialization only
	CBugTrapTestDoc();
	DECLARE_DYNCREATE(CBugTrapTestDoc)

// Attributes
public:

// Operations
public:

// Overrides
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CBugTrapTestDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};



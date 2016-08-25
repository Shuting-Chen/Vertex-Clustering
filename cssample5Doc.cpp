// cssample5Doc.cpp : implementation of the CCssample5Doc class
//

#include "stdafx.h"
#include "cssample5.h"

#include "cssample5Doc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CCssample5Doc

IMPLEMENT_DYNCREATE(CCssample5Doc, CDocument)

BEGIN_MESSAGE_MAP(CCssample5Doc, CDocument)
	//{{AFX_MSG_MAP(CCssample5Doc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCssample5Doc construction/destruction

CCssample5Doc::CCssample5Doc()
{
	// TODO: add one-time construction code here

}

CCssample5Doc::~CCssample5Doc()
{
}

BOOL CCssample5Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CCssample5Doc serialization

void CCssample5Doc::Serialize(CArchive& ar)
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

/////////////////////////////////////////////////////////////////////////////
// CCssample5Doc diagnostics

#ifdef _DEBUG
void CCssample5Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CCssample5Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCssample5Doc commands

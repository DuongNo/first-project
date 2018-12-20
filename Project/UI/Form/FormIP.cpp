// FormIP.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "resource.h"
#include "FormIP.h"


// CFormIP

IMPLEMENT_DYNCREATE(CFormIP, CFormView)

CFormIP::CFormIP()
	: CFormView(CFormIP::IDD)
{

}

CFormIP::~CFormIP()
{
}

void CFormIP::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFormIP, CFormView)
END_MESSAGE_MAP()


// CFormIP 진단입니다.

#ifdef _DEBUG
void CFormIP::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CFormIP::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CFormIP 메시지 처리기입니다.

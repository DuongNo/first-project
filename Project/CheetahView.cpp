
// CheetahView.cpp : CCheetahView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "CheetahApp.h"
#endif

#include "CheetahDoc.h"
#include "CheetahView.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


// CCheetahView

IMPLEMENT_DYNCREATE(CCheetahView, CFormView)

BEGIN_MESSAGE_MAP(CCheetahView, CFormView)
END_MESSAGE_MAP()

// CCheetahView ����/�Ҹ�

CCheetahView::CCheetahView()
	: CFormView(CCheetahView::IDD)
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

}

CCheetahView::~CCheetahView()
{
}

void CCheetahView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BOOL CCheetahView::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style = WS_OVERLAPPED | WS_CHILD;	// ���� �׵θ��� ���ش�.

	return CFormView::PreCreateWindow(cs);
}

void CCheetahView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

}


// CCheetahView ����

#ifdef _DEBUG
void CCheetahView::AssertValid() const
{
	CFormView::AssertValid();
}

void CCheetahView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CCheetahDoc* CCheetahView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCheetahDoc)));
	return (CCheetahDoc*)m_pDocument;
}
#endif //_DEBUG


// CCheetahView �޽��� ó����

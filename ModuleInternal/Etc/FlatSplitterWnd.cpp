// FlatSplitterWnd.cpp : implementation file
//

#include "stdafx.h"
#include "FlatSplitterWnd.h"
#include ".\flatsplitterwnd.h"

/////////////////////////////////////////////////////////////////////////////
// CFlatSplitterWnd

#define CONST_SPLITTER_THICK			2
#define CONST_SPLITTER_GAP				CONST_SPLITTER_THICK
#define CONST_SPLITTER_BORDER			0
#define CONST_SPLITTER_BORDER_SHARE		0

IMPLEMENT_DYNCREATE(CFlatSplitterWnd, CSplitterWnd)

CFlatSplitterWnd::CFlatSplitterWnd()
{
	m_cxSplitter	= m_cySplitter		= CONST_SPLITTER_THICK;
	m_cxSplitterGap = m_cySplitterGap	= CONST_SPLITTER_GAP;
	m_cxBorder		= m_cyBorder		= CONST_SPLITTER_BORDER;
	m_cxBorderShare = m_cyBorderShare	= CONST_SPLITTER_BORDER_SHARE;
}

CFlatSplitterWnd::~CFlatSplitterWnd()
{
}

BEGIN_MESSAGE_MAP(CFlatSplitterWnd, CSplitterWnd)
	//{{AFX_MSG_MAP(CFlatSplitterWnd)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFlatSplitterWnd message handlers

void CFlatSplitterWnd::SetSplitterThick(int nHorz, int nVert)
{
	if(nHorz < 2 || nVert < 2)		return;

	m_cxSplitter	= m_cxSplitterGap	= nHorz;
	m_cySplitter	= m_cySplitterGap	= nVert;

	RecalcLayout();

	Invalidate(FALSE);
}

void CFlatSplitterWnd::OnDrawSplitter(CDC* pDC, ESplitType nType, const CRect& rect)
{
	if((nType != splitBorder) || (pDC == NULL))
	{
		CSplitterWnd::OnDrawSplitter(pDC, nType, rect);
		return;
	}

	pDC->Draw3dRect(rect, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DHILIGHT));
}

void CFlatSplitterWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	CWnd::OnMouseMove(nFlags, point);
}

void CFlatSplitterWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	CWnd::OnLButtonDown(nFlags, point);
}

void CFlatSplitterWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	CWnd::OnLButtonUp(nFlags, point);
}

void CFlatSplitterWnd::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CWnd::OnLButtonDblClk(nFlags, point);
}

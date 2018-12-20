#if !defined(AFX_FLATSPLITTERWND_H__099C0BD1_B833_4483_89CA_D00FB2517754__INCLUDED_)
#define AFX_FLATSPLITTERWND_H__099C0BD1_B833_4483_89CA_D00FB2517754__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FlatSplitterWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFlatSplitterWnd frame with splitter


class CFlatSplitterWnd : public CSplitterWnd
{
	DECLARE_DYNCREATE(CFlatSplitterWnd)

public:
	CFlatSplitterWnd();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:
	void SetSplitterThick(int nHorz, int nVert);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFlatSplitterWnd)
	public:
	virtual void OnDrawSplitter(CDC* pDC, ESplitType nType, const CRect& rect);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CFlatSplitterWnd();

	// Generated message map functions
	//{{AFX_MSG(CFlatSplitterWnd)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FLATSPLITTERWND_H__099C0BD1_B833_4483_89CA_D00FB2517754__INCLUDED_)

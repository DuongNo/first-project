// dblRect.h: interface for the CdblRect class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DBLRECT_H__5E0B3D4A_7A9B_46EB_B05E_CB637584A47C__INCLUDED_)
#define AFX_DBLRECT_H__5E0B3D4A_7A9B_46EB_B05E_CB637584A47C__INCLUDED_

#include "dblSize.h"	// Added by ClassView
#include "dblPoint.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


struct tagdblRECT {
	double left;
	double top;
	double right;
	double bottom;
};

typedef tagdblRECT dblRECT, *PdblRECT, NEAR *NPdblRECT, FAR *LPdblRECT;

//class CdblRect : public dblRECT
class CdblRect : public tagdblRECT
{
public :
	CdblRect();
	CdblRect(double l, double t, double r, double b);
	CdblRect(const CdblPoint& leftTop, const CdblPoint& bottomRight);
	CdblRect(const CdblPoint& point, const CdblSize& size);
	CdblRect(const CRect& rect);
	virtual ~CdblRect();

#ifdef _DEBUG
    virtual void Dump( CDumpContext& dc ) const;
#endif

// Implememtations
public :
	CdblSize Size() const;
	CdblPoint TopLeft() const;
	CdblPoint BottomRight() const;
	CRect GetCRect() const {
		return CRect((long)left, (long)top, (long)right, (long)bottom);
	}
	
	void SetRectEmpty();
	BOOL IsRectEmpty() const;
	BOOL IsRectNull() const;

	BOOL IsCrossed2(CdblRect *lpRect1, CdblRect *lpRect2);
	void NormalizeRect();
	void OffsetRect(const CdblSize& size);
	void OffsetRect(double x, double y);

	void CenterPoint(double& dblCenterX, double& dblCenterY) const;
	CdblPoint CenterPoint() const;
	
	void UnionRect(const CdblRect& rect);
	BOOL IntersectRect(const CdblRect& rect1, const CdblRect& rect2);
	void InflateRect(double dx, double dy);
	void InflateRect(int l, int t, int r, int b);
	BOOL PtInRect(const CdblPoint& point) const;
	//! deplicated
	BOOL IsIncluded(const CdblRect& rect) const;
	BOOL RectInRect(const CdblRect& rect) const;
	BOOL EqualRect(const CdblRect& rect);
	void EnsureInRect(CdblRect& rect);

	double Height() const;
	double Width() const;
	BOOL IsCrossed(CdblRect *lpRect1, CdblRect *lpRect2);
	void operator=(const CdblRect& srcRect);
	void operator=(const CRect& srcRect);
	void operator+=(const CdblSize& size);
	void operator-=(const CdblSize& size);
	void operator+=(const CdblPoint& point);
	void operator-=(const CdblPoint& point);
	void operator|=(const CdblRect& rect);
	BOOL operator!=(const CdblRect& rect);
	BOOL operator==(const CdblRect& rect);
	CdblRect operator+(const CdblSize& size);

	CdblRect operator|(const CdblRect& rect);
	CdblRect operator&(const CdblRect& rect);
};

#endif // !defined(AFX_DBLRECT_H__5E0B3D4A_7A9B_46EB_B05E_CB637584A47C__INCLUDED_)

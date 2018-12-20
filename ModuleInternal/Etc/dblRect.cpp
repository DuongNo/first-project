// dblRect.cpp: implementation of the CdblRect class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "dblRect.h"
#include <afxtempl.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CdblRect::CdblRect()
{
	left	= 0.0;
	top		= 0.0;
	right	= 0.0;
	bottom	= 0.0;
}

CdblRect::CdblRect(double l, double t, double r, double b)
{
	left	= l;
	top		= t;
	right	= r;
	bottom	= b;
}

CdblRect::CdblRect(const CdblPoint& topLeft, const CdblPoint& bottomRight)
{
	left = topLeft.x;
	top = topLeft.y;
	right = bottomRight.x;
	bottom = bottomRight.y;
}

CdblRect::CdblRect(const CdblPoint& point, const CdblSize& size)
{
	left = point.x;
	top = point.y;
	right = left + size.cx;
	bottom = top + size.cy;
}

CdblRect::CdblRect(const CRect& rect)
{
	left = rect.left;
	top = rect.top;
	right = rect.right;
	bottom = rect.bottom;
}

CdblRect::~CdblRect()
{

}

void CdblRect::SetRectEmpty()
{
	left = right = top = bottom = 0.0;
}

double CdblRect::Width() const
{
	return (right-left);
}

double CdblRect::Height() const
{
	return (bottom-top);
}

CdblSize CdblRect::Size() const
{
	return CdblSize(Width(),Height());
}

CdblPoint CdblRect::TopLeft() const
{
	return CdblPoint(left, top);
}

CdblPoint CdblRect::BottomRight() const
{
	return CdblPoint(right, bottom);
}

BOOL CdblRect::IsRectNull() const
{
	if (left == 0.0 && right == 0.0 && top == 0.0 && bottom == 0.0)
		return TRUE;
	else 
		return FALSE;
}

BOOL CdblRect::IsRectEmpty() const
{
	double width = (double)(left - right);
	double height = (double)(top - bottom);

	if ((width+height) == 0.0) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}

void CdblRect::operator=(const CdblRect &srcRect)
{
	left	= srcRect.left;
	top		= srcRect.top;
	right	= srcRect.right;
	bottom	= srcRect.bottom;
}

void CdblRect::operator=(const CRect& srcRect)
{
	left	= srcRect.left;
	top		= srcRect.top;
	right	= srcRect.right;
	bottom	= srcRect.bottom;
}

void CdblRect::CenterPoint(double& dblCenterX, double& dblCenterY) const
{
	dblCenterX = (right+left)/2.0;
	dblCenterY = (bottom+top)/2.0;
}

CdblPoint CdblRect::CenterPoint() const
{
	CdblPoint centerPoint;
	CenterPoint(centerPoint.x, centerPoint.y);

	return centerPoint;
}

void CdblRect::OffsetRect(double x, double y)
{
	left	+= x;
	top		+= y;
	right	+= x;
	bottom	+= y;
}

void CdblRect::OffsetRect(const CdblSize& size)
{
	left	+= size.cx;
	top		+= size.cy;
	right	+= size.cx;
	bottom	+= size.cy;
}

void CdblRect::InflateRect(double dx, double dy)
{
	left -= dx;
	right += dx;
	top -= dy;
	bottom += dy;
}

void CdblRect::InflateRect(int l, int t, int r, int b)
{
	left -= l;
	right += r;
	top -= t;
	bottom += b;
}

BOOL CdblRect::IntersectRect(const CdblRect& rect1, const CdblRect& rect2)
{
	CdblRect _rect1 = rect1;
	CdblRect _rect2 = rect2;

	_rect1.NormalizeRect();
	_rect2.NormalizeRect();

	CdblPoint pt1(_rect1.left, _rect1.top);
	CdblPoint pt2(_rect1.right, _rect1.bottom);
	CdblPoint pt3(_rect1.left , _rect1.bottom); 
	CdblPoint pt4(_rect1.right, _rect1.top); 

	BOOL fTest1 = _rect2.PtInRect(pt1) || _rect2.PtInRect(pt2) ||
					_rect2.PtInRect(pt3) || _rect2.PtInRect(pt4);

	pt1 = CdblPoint(_rect2.left, _rect2.top);
	pt2 = CdblPoint(_rect2.right, _rect2.bottom);
	pt3 = CdblPoint(_rect2.left , _rect2.bottom); 
	pt4 = CdblPoint(_rect2.right, _rect2.top); 

	BOOL fTest2 = _rect1.PtInRect(pt1) || _rect1.PtInRect(pt2) ||
					_rect1.PtInRect(pt3) || _rect1.PtInRect(pt4);

	if (fTest1 || fTest2) {

		left = max(_rect1.left, _rect2.left);
		right = min(_rect1.right, _rect2.right);
		top = max(_rect1.top, _rect2.top);
		bottom = min(_rect1.bottom, _rect2.bottom);

		return TRUE;
	}

	SetRectEmpty();

	return FALSE;
}

void CdblRect::UnionRect(const CdblRect& rect)
{
	if (IsRectEmpty()) {
		left = rect.left;
		top = rect.top;
		right = rect.right;
		bottom = rect.bottom;
	} else {
		left	= (left <= rect.left) ? left : rect.left;
		top		= (top <= rect.top) ? top : rect.top;
		right	= (right >= rect.right) ? right : rect.right;
		bottom	= (bottom >= rect.bottom) ? bottom : rect.bottom;
	}
}

BOOL CdblRect::RectInRect(const CdblRect& rect) const
{
	return IsIncluded(rect);
}

BOOL CdblRect::IsIncluded(const CdblRect& rect) const
{
	CdblPoint pt1, pt2;
	pt1 = CdblPoint(rect.left, rect.top);
	pt2 = CdblPoint(rect.right, rect.bottom);

	if (PtInRect(pt1) == TRUE && PtInRect(pt2) == TRUE) {
		return TRUE;
	} else {
		return FALSE;
	}
}

BOOL CdblRect::PtInRect(const CdblPoint& point) const
{
	if (point.x>=left && point.x<=right && point.y>=top && point.y<=bottom) {
		return TRUE;
	} else {
		return FALSE;
	}
}

void CdblRect::NormalizeRect()
{
	double temp;
	if (left >= right) {
		temp	= left;
		left	= right;
		right	= temp;
	}
	if (top >= bottom) {
		temp	= top;
		top		= bottom;
		bottom	= temp;
	}
}

// park 6 28  두 Rect의 중복성 검사 
BOOL CdblRect::IsCrossed(CdblRect *lpRect1, CdblRect *lpRect2)
{
	CdblPoint pt1, pt2,pt3,pt4;

	pt1 = CdblPoint(lpRect1->left, lpRect1->top);
	pt2 = CdblPoint(lpRect1->right-0.001, lpRect1->bottom-0.001);
	pt3 = CdblPoint(lpRect1->left , lpRect1->bottom-0.001); 
	pt4 = CdblPoint(lpRect1->right-0.001, lpRect1->top); 

	if ((lpRect2->PtInRect(pt1) ==TRUE) || (lpRect2->PtInRect(pt2) == TRUE) ||
					(lpRect2->PtInRect(pt3) == TRUE) || (lpRect2->PtInRect(pt2)==TRUE)  ) {
		return TRUE;
	}

	pt1 = CdblPoint(lpRect2->left, lpRect2->top);
	pt2 = CdblPoint(lpRect2->right-0.001, lpRect2->bottom-0.001);
	pt3 = CdblPoint(lpRect2->left , lpRect2->bottom-0.001); 
	pt4 = CdblPoint(lpRect2->right-0.001, lpRect2->top); 
	
	if ((lpRect1->PtInRect(pt1) ==TRUE) || (lpRect1->PtInRect(pt2) == TRUE) ||
					(lpRect1->PtInRect(pt3) == TRUE) || (lpRect1->PtInRect(pt2)==TRUE)  ) {
		return TRUE;
	}
	return FALSE;
}

BOOL CdblRect::IsCrossed2(CdblRect *lpRect1, CdblRect *lpRect2)
{
	CdblPoint pt1_f, pt2_f,pt3_f,pt4_f;
	CdblPoint pt1_s, pt2_s,pt3_s,pt4_s;
	CdblPoint Cen1 , Cen2 ;
	CRect rtFirst,rtSecond,rtTemp;
	rtFirst = CRect((long) lpRect1->left,(long) lpRect1->top ,(long) lpRect1->right,(long) lpRect1->bottom);
	rtSecond = CRect((long) lpRect2->left,(long) lpRect2->top ,(long) lpRect2->right,(long) lpRect2->bottom);
	rtFirst.NormalizeRect();
	rtSecond.NormalizeRect();
	rtTemp = rtFirst.operator &(rtSecond);
	if(rtTemp.IsRectEmpty() == FALSE){
		return TRUE;
	}
	return FALSE; 
}

void CdblRect::operator-=(const CdblSize& size)
{
	OffsetRect(-size.cx, -size.cy);
}

void CdblRect::operator+=(const CdblSize& size)
{
	OffsetRect(size);
}

void CdblRect::operator-=(const CdblPoint& point)
{
	OffsetRect(-point.x, -point.y);
}

void CdblRect::operator+=(const CdblPoint& point)
{
	OffsetRect(point.x, point.y);
}

void CdblRect::operator|=(const CdblRect& rect)
{
	UnionRect(rect);
}

CdblRect CdblRect::operator|(const CdblRect& rect)
{
	CdblRect destRect = *this;
	destRect.UnionRect(rect);

	return destRect;
}

CdblRect CdblRect::operator&(const CdblRect& rect)
{
	CdblRect destRect;
	destRect.IntersectRect(*this, rect);

	return destRect;
}

CdblRect CdblRect::operator+(const CdblSize& size)
{
	CdblRect destRect = *this;
	destRect.OffsetRect(size);

	return destRect;
}

BOOL CdblRect::operator!=(const CdblRect& rect)
{
	return (rect.left != left || rect.top != top || rect.right != right || rect.bottom != bottom );
}

BOOL CdblRect::operator==(const CdblRect& rect)
{
	return EqualRect(rect);
}

BOOL CdblRect::EqualRect(const CdblRect& rect)
{
	return (rect.left == left && rect.top == top && rect.right == right && rect.bottom == bottom );
}

void CdblRect::EnsureInRect(CdblRect& rect)
{
	if (!IsRectEmpty()) {
		NormalizeRect();
		rect.NormalizeRect();
		long rectWidth = (long)rect.Width();
		long rectHeight = (long)rect.Height();
		if (rectWidth < Width() && rectHeight < Height()) {
			if (!rect.IsRectEmpty()) {

				CdblRect intersectRect;
				intersectRect.IntersectRect(*this, rect);
				if (!intersectRect.EqualRect(rect)) {
					if (rect.right > right) {
						rect.right = right;
						rect.left = rect.right - rectWidth;
					} else if (rect.left < left) {
						rect.left = left;
						rect.right = rect.left + rectWidth;
					}

					if (rect.top < top) {
						rect.top = top;
						rect.bottom = rect.top + rectHeight;
					} else if (rect.bottom > bottom) {
						rect.bottom = bottom;
						rect.top = rect.bottom - rectHeight;
					}
				}
			} else {
				if (left > rect.left) {
					rect.left = rect.right = left;
				} else if (right < rect.right) {
					rect.left = rect.right = right;
				}

				if (top > rect.top) {
					rect.top = rect.bottom = top;
				} else if (bottom < rect.bottom) {
					rect.top = rect.bottom = bottom;
				}
			}
		}
	}
}

#ifdef _DEBUG
void CdblRect::Dump( CDumpContext& dc ) const
{
    // now do the stuff for our specific class
    dc << "(" << left << ", " << top << ", " << right << ", " << bottom << ")\n"; 
}
#endif


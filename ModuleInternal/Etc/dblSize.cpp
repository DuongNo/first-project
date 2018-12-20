// dblSize.cpp: implementation of the CdblSize class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "dblSize.h"

#include "dblPoint.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CdblSize::CdblSize()
{
	cx = 0.0;
	cy = 0.0;
}

CdblSize::CdblSize(const CdblPoint& point)
{
	cx = point.x;
	cy = point.y;
}

CdblSize::CdblSize(const CSize& size)
{
	cx = size.cx;
	cy = size.cy;
}

CdblSize::CdblSize(double initCX, double initCY)
{
	cx = initCX;
	cy = initCY;
}

CdblSize::~CdblSize()
{

}

void CdblSize::operator=(const CdblSize &srcSize)
{
	cx = srcSize.cx;
	cy = srcSize.cy;
}

void CdblSize::operator=(const CdblPoint &srcPoint)
{
	cx = srcPoint.x;
	cy = srcPoint.y;
}

BOOL CdblSize::operator!=(const CdblSize& size)
{
	return (cx != size.cx || cy != size.cy);
}

BOOL CdblSize::operator==(const CdblSize& size)
{
	return (cx == size.cx && cy == size.cy);
}

CdblSize CdblSize::operator+(const CdblPoint& pt) const
{
	return CdblSize(cx+pt.x, cy+pt.y);
}

void CdblSize::operator+=(const CdblPoint& pt)
{
	cx += pt.x;
	cy += pt.y;
}

CdblSize CdblSize::operator+(const CdblSize& size) const
{
	return CdblSize(cx+size.cx, cy+size.cy);
}

void CdblSize::operator+=(const CdblSize& size)
{
	cx += size.cx;
	cy += size.cy;
}

CdblSize CdblSize::operator-(const CdblPoint& pt) const
{
	return CdblSize(cx-pt.x, cy-pt.y);
}

void CdblSize::operator-=(const CdblPoint& pt)
{
	cx -= pt.x;
	cy -= pt.y;
}

CdblSize CdblSize::operator-() const
{
	return CdblSize(-cx, -cy);
}

CdblSize CdblSize::operator-(const CdblSize& size) const
{
	return CdblSize(cx-size.cx, cy-size.cy);
}

void CdblSize::operator-=(const CdblSize& size)
{
	cx -= size.cx;
	cy -= size.cy;
}

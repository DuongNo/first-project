// dblPoint.cpp: implementation of the CdblPoint class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "dblPoint.h"

#include "dblSize.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CdblPoint::CdblPoint()
{
	x = 0.0;
	y = 0.0;
}


CdblPoint::CdblPoint(double initX, double initY)
{
	x = initX;
	y = initY;
}

CdblPoint::CdblPoint(const CPoint& point)
{
	x = point.x;
	y = point.y;
}

CdblPoint::CdblPoint(const CdblPoint& point)
{
	x = point.x;
	y = point.y;
}

CdblPoint::~CdblPoint()
{

}

void CdblPoint::Offset(double xOffset, double yOffset)
{
	x = x + xOffset;
	y = y + yOffset;
}

CdblPoint CdblPoint::operator-()
{
	return CdblPoint(-x, -y);
}

CdblPoint CdblPoint::operator-() const
{
	return CdblPoint(-x, -y);
}

CdblPoint CdblPoint::operator-(const CdblPoint& pt) const
{
	return CdblPoint(x-pt.x, y-pt.y);
}

CdblPoint CdblPoint::operator-(const CdblSize& size) const
{
	return CdblPoint(x-size.cx, y-size.cy);
}

CdblPoint CdblPoint::operator+(const CdblPoint& pt) const
{
	return CdblPoint(x+pt.x, y+pt.y);
}

CdblPoint CdblPoint::operator+(const CdblSize& size) const
{
	return CdblPoint(x+size.cx, y+size.cy);
}

void CdblPoint::operator =(const CdblPoint &srcPoint)
{
	x = srcPoint.x;
	y = srcPoint.y;
}

void CdblPoint::operator =(const CPoint &srcPoint)
{
	x = srcPoint.x;
	y = srcPoint.y;
}

BOOL CdblPoint::operator!=(const CdblPoint& point)
{
	return (x != point.x || y != point.y);
}

BOOL CdblPoint::operator!=(const CPoint& point)
{
	return (x != point.x || y != point.y);
}

BOOL CdblPoint::operator == (const CPoint &point)
{
	return (x == point.x && y == point.y);
}

BOOL CdblPoint::operator == (const CdblPoint &point)
{
	return (x == point.x && y == point.y);
}

void CdblPoint::operator-=(const CdblPoint& pt)
{
	x -= pt.x;
	y -= pt.y;
}

void CdblPoint::operator-=(const CdblSize& size)
{
	x -= size.cx;
	y -= size.cy;
}

void CdblPoint::operator+=(const CdblPoint& pt)
{
	x += pt.x;
	y += pt.y;
}

void CdblPoint::operator+=(const CdblSize& size)
{
	x += size.cx;
	y += size.cy;
}

void CdblPoint::operator*=(const double& dValue)
{
	x *= dValue;
	y *= dValue;
}

void CdblPoint::operator*=(const CdblPoint& dValue)
{
	x *= dValue.x;
	y *= dValue.y;
}

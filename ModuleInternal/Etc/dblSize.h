// dblSize.h: interface for the CdblSize class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

class CdblPoint;

typedef struct tagdblSIZE {
	double cx;
	double cy;
} dblSIZE;

class CdblSize : public dblSIZE
{
public:
	CdblSize();
	CdblSize(double initCX, double initCY);
	CdblSize(const CdblPoint& point);
	CdblSize(const CSize& size);
	virtual ~CdblSize();

	void operator =(const CdblSize &srcSize);
	void operator =(const CdblPoint &srcPoint);
	BOOL operator!=(const CdblSize& size);
	BOOL operator==(const CdblSize& size);

	void operator-=(const CdblSize& size);
	void operator+=(const CdblSize& size);
	void operator-=(const CdblPoint& point);
	void operator+=(const CdblPoint& point);

	CdblSize operator-(const CdblSize& size) const;
	CdblSize operator-() const;
	CdblSize operator+(const CdblSize& size) const;
	CdblSize operator-(const CdblPoint& point) const;
	CdblSize operator+(const CdblPoint& point) const;

	CSize GetCSize() const {
		return CSize((long)cx, (long)cy);
	}
};
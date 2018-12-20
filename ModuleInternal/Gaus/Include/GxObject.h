//==================================================================================================
// CGxObject : Gaus Library의 가강 기본적인 최상위 조상 class
//
// #2015-04-25, jhLee (semsemi)
//
// 파생 Class : CGxTask, CGxMaster, ......
//
//==================================================================================================
#pragma once

#include "Gaus.h"


class _GAUS_LIB CGxObject
{
public:
	CString		m_sName;				// 이름
	int			m_nObjType;			// 어떠한 종류의 객체인지 구분자

public:
	CGxObject(void);
	virtual ~CGxObject(void);

};


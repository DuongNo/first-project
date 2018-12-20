//
// CGxRobotMgr : CGxRobot 객체들을 생성하고 초기/조회 관리해주는 관리 class
// 
// 2014-12-23 ~, jhLee
//

#pragma once

#include "Gaus.h"
#include "GxSingleton.h"
#include "File\GxIniFile.h"

#include "Motion\GxMotion.h"
#include "Motion\GxRobot.h"


class _GAUS_LIB CGxRobotMgr : public CGxSingleton<CGxRobotMgr>				// Singleton으로 만든다.
{
public:
	CGxRobotMgr(void);
	~CGxRobotMgr(void);

	CString		m_sBasePath;								// 기본 경로
	CGxIniFile	m_iniData;									// 정보를 담고 있는 ini 파일

	CGxRobotPtrArray		m_arrRobotMap;					// CGxRobot class를 가지는 동적 array

public:

	void Test() { AfxMessageBox(_T("CGxRobotMgr TEST OK !") ); };


	BOOL Init(CString sBase);						// 설정값을 불러와서 초기화 시킨다.
	BOOL CreateRobot();								// 지정한 조건으로 Robot을 생성시켜준다.
	void ReleaseRobot();								// 사용을 마친 Robot들의 instance들을 삭제한다.
	int	GetCount();									// 드록된 Robot 수량을 되돌려준다.


	CGxRobot* GetRobot(int nIdx);					// Index로  해당 Robot의 포인터를 조회한다.
	CGxRobot* GetRobot(CString sName);			// 이름으로 해당 Robot의 포인터를 조회한다.

};

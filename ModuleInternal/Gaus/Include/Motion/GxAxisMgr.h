//
// CGxAxisMgr : CGxAxis의 인스턴스를 생성하고 초기화 및 MotionCmd와 일치시켜주는 축 관리 class
// 
//
// 2014-12-23 ~, jhLee
//

#pragma once

#include "Gaus.h"
#include "GxSingleton.h"
#include "Motion\GxAxis.h"
#include "File\GxIniFile.h"

class _GAUS_LIB CGxAxisMgr : public CGxSingleton<CGxAxisMgr>				// Singleton으로 만든다.
{
public:
	CGxAxisMgr(void);
	~CGxAxisMgr(void);

	CString				m_sBasePath;						// 기본 경로
	CGxIniFile			m_iniData;							// 정보를 담고 있는 ini 파일

	CGxAxisPtrArray		m_arrAxisMap;						// Axis class를 가지는 동적 array

public:

	void TestCall() { AfxMessageBox(_T("CGxAxisMgr TEST OK !") ); };

	int GetAxisCount();										// 등록된 축의 수량 조회
	BOOL Init(CString sBase);								// 설정값을 불러와서 초기화 시킨다.
	BOOL CreateAxis();										// 지정한 조건으로 축들을 생성시켜준다.
	BOOL StartUp();											// 지정된 축들을 활성화 시켜준다.	
	void ReleaseAxis();										// 사용을 마친 축들의 instance들을 삭제한다.

	
	CGxAxis* GetAxis(int nIdx);								// Index로  해당 축의 포인터를 조회한다.
	CGxAxis* GetAxis(CString sName);						// 축 이름으로 해당 축의 포인터를 조회한다.
	int GetAxisIndex(CString sName);								// 축 이름으로 해당 축의 순번을 조회한다.
	CGxAxis* GetAxisAtCmd(int nCmdIdx, int nIdx);			// 지정 모션보드의 지정 순번 축의 포인터를 조회한다.


	// 등록된 모든 축에 대하여 일괄 정지 명령을 지령한다.
	void MoveStopAll();										// 일반 감속 정지
	void MoveEmgStopAll();									// 긴급 정지

	void MoveStopRequest(BOOL bEmg);						// 모든 축에 정지 요청을 보낸다. 실제로 정지하고, 플랙 변수도 설정한다.
	void SetInterruptFlag(BOOL bFlag);						// 요청에 의한 정지인지 Flag 변수만 지정한다.
	void ClearStopRequest();								// 정지 요청을 초기화 시켜준다.


};


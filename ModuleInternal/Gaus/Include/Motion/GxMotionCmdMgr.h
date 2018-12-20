//
// CGxMotionCmdMgr : 각 Motion Controller의 API를 제어하는 MotionCmd를 관리해주는 관리 Class
//  
// 각 모션제어기를 초기화 해주는 역할과 원하는 MotionCmd를 찾는 기능을 수행한다.
//
// 2014-12-22 ~, jhLee
//

#pragma once

#include "Gaus.h"
#include "GxSingleton.h"
#include "Motion\GxMotionCmd.h"
#include "Motion\GxMotion.h"

#include "File\GxIniFile.h"


class _GAUS_LIB CGxMotionCmdMgr : public CGxSingleton<CGxMotionCmdMgr>				// Singleton으로 만든다.
{
//private:
public:
	CGxMotionCmdMgr(void);
	~CGxMotionCmdMgr(void);
	// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -

	BOOL					m_bSimulate;				// H/W 연결없이 가상모드로 동작할 것인가 ?
	CString					m_sBasePath;				// 기본 경로
	CGxIniFile				m_iniData;					// 정보를 담고 있는 ini 파일, MotionCmd.ini


	CGxMotionCmdPtrArray	m_arrMotionMap;				// 여러 Motion 제어 class를 가지는 동적 array
	//? CGxMapStringToMotionCmd	m_mapMotionMap;				// 이름으로 찾기위한 map, 실체는 array에 저장 된다.

public:

	void TestCall() { AfxMessageBox(_T("CGxMotionCmdMgr is Called !") ); };

	int GetMotionCmdCount();								// 동록된 Motion Cmd 수량 조회

	// 오류검사
	int GetCheckErrorCmd(int& nSubIdx);						// 문제가 있는 보드의 Index를 조회한다. -1 : 모두 정상, 0 ~ 문제가 있는 보드의 Index
	int GetIndexMotionType(int nIdx);						// 지정 MotionCmd Index의 Motion Type을 조회한다.
	int GetCheckErrorCmdType(int& nSubIdx);					// 문제가 있는 보드의 Type과 해당 보드의 순번을 조회한다. return값이 -1 이면 정상상태임
	CString GetCheckErrorCmdTypeName(int& nSubIdx);			// 문제가 있는 보드의 Type 이름과 해당 보드의 순번을 조회한다. return값이 _T("") 이면 정상상태임
	// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

	BOOL Init(CString sBase);								// 설정값을 불러와서 초기화 시킨다.
	BOOL CreateDevice();									// 설정된 모션제어기들을 생성시켜준다.
	BOOL OpenDevice();										// 설정값을 토대로 장치를 열어준다.
	void CloseDevice();										// 장치를 닫아준다.
	void ReleaseCmd();										// 사용을 마친 모션제어기들의 instance들을 삭제한다.

	// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	// 이곳에서 조회가 가능한 것은 각 모션 카드 종류별로 Master가되는 GxMotionCmd를 조회할 수 있다.
	// 초기 시작시 포인터 대입등의 용도로 사용된다.
	CGxMotionCmd* GetMotionCmdByType(int nType);		// 모션보드 Type으로 해당 Motion Master cmd를 조회한다.
	CGxMotionCmd* GetMotionCmdByIndex(int nIdx);		// Index로 해당 Motion Master cmd를 조회한다.
	CGxMotionCmd* GetMotionCmdByName(CString sName);	// 모션보드 이름으로 Motion Master Cmd를 조회한다.


};


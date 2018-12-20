#include "StdAfx.h"
#include "UnLoadRuleInfo.h"

#include <fstream>
#include <wtypes.h>

CUnLoadRuleInfo::CUnLoadRuleInfo(void)
{
	//2017-05-17,skcho,초기화 추가 unloader robot이 JOBCHANGE 변수 초기화가 안되서 TRAY에 못 담는 현상 발생
	m_nJobChangeType = 0;
}


CUnLoadRuleInfo::~CUnLoadRuleInfo(void)
{
}
// 상위에서 보내온 Job file download관련 정보를 읽어온다.
void CUnLoadRuleInfo::ReadUnloadRuleFileInfo()
{
	CString sSection;
	CString sKey;
	CString strPath = _T("D:\\Cheetah\\System\\UnLoadRuleInfo.ini");			// 가온누리에서 S8F1을 수신받을 때 기록한다.
	CIni iniFile(strPath);	


	sSection = "UNLOAD RULE INFO";

	//2017-02-26, jhLee, Job file download 수행을 위한 데이터를 파일에서 읽어온다.
	// Job file download 처리 과정
	m_sCurrentProduct	= iniFile.GetString(sSection,	_T("CURRENT PRODUCT"),		_T(""));		
	m_sCurrentPPID		= iniFile.GetString(sSection,	_T("CURRENT PPID"),		_T(""));		
	m_sTrayQtyInLot		= iniFile.GetString(sSection,	_T("TRAY QTY IN LOT"),		_T("30"));
	m_sCellQtyInTray	= iniFile.GetString(sSection,	_T("CELL QTY IN TRAY"),		_T("8"));
	m_sNextStepID		= iniFile.GetString(sSection,	_T("NEXT STEPID"),		_T("AMT"));

}

// 갱신된 Job file download관련 정보를 저장한다.
void CUnLoadRuleInfo::SaveUnloadRuleFileInfo()
{
	CString sSection;
	CString sKey;
	CString strPath = _T("D:\\Cheetah\\System\\UnLoadRuleInfo.ini");			// 가온누리에서 S8F1을 수신받을 때 기록한다.
	CIni iniFile(strPath);	

	sSection = "UNLOAD RULE INFO";

	// Job file download 처리 과정
	iniFile.WriteString(sSection,	_T("CURRENT PRODUCT"),		m_sCurrentProduct);			// 현재 진행과정 단계
	iniFile.WriteString(sSection,	_T("CURRENT PPID"),			m_sCurrentPPID);			// 현재 신호기로 부터 수신받은 과정
	iniFile.WriteString(sSection,	_T("TRAY QTY IN LOT"),		m_sTrayQtyInLot);
	iniFile.WriteString(sSection,	_T("CELL QTY IN TRAY"),		m_sCellQtyInTray);
	iniFile.WriteString(sSection,	_T("NEXT STEPID"),			m_sNextStepID);
	

}
void CUnLoadRuleInfo::SetJobChangeCase2Step(int nStep)
{
	m_nJobChangeCase2Step = nStep;
}
int CUnLoadRuleInfo::GetJobChangeCase2Step()
{
	return m_nJobChangeCase2Step;
}
CString CUnLoadRuleInfo::GetUnloadingRuleInfoCurrentPPID()
{
	return m_sCurrentPPID;
}
void CUnLoadRuleInfo::SetJobChangeType(int nType)
{
	m_nJobChangeType = nType;
}
int CUnLoadRuleInfo::GetJobChangeType()
{
	return m_nJobChangeType;
}

int CUnLoadRuleInfo::GetTrayLotQtyInt()
{
	int  nResult =0;  
	nResult = _tstoi(m_sTrayQtyInLot.GetString());
	return nResult;
}
int CUnLoadRuleInfo::GetCellInTrayQtyInt()
{
	int  nResult =0;  
	nResult = theRecipeBank.m_Tray.m_nCountX * theRecipeBank.m_Tray.m_nCountY;
	// 해당 카운트는 트레이 Lot의 셀 총 수량이다.
	//nResult = _tstoi(m_sCellQtyInTray.GetString());
	return nResult;
}
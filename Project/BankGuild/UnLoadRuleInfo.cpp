#include "StdAfx.h"
#include "UnLoadRuleInfo.h"

#include <fstream>
#include <wtypes.h>

CUnLoadRuleInfo::CUnLoadRuleInfo(void)
{
	//2017-05-17,skcho,�ʱ�ȭ �߰� unloader robot�� JOBCHANGE ���� �ʱ�ȭ�� �ȵǼ� TRAY�� �� ��� ���� �߻�
	m_nJobChangeType = 0;
}


CUnLoadRuleInfo::~CUnLoadRuleInfo(void)
{
}
// �������� ������ Job file download���� ������ �о�´�.
void CUnLoadRuleInfo::ReadUnloadRuleFileInfo()
{
	CString sSection;
	CString sKey;
	CString strPath = _T("D:\\Cheetah\\System\\UnLoadRuleInfo.ini");			// ���´������� S8F1�� ���Ź��� �� ����Ѵ�.
	CIni iniFile(strPath);	


	sSection = "UNLOAD RULE INFO";

	//2017-02-26, jhLee, Job file download ������ ���� �����͸� ���Ͽ��� �о�´�.
	// Job file download ó�� ����
	m_sCurrentProduct	= iniFile.GetString(sSection,	_T("CURRENT PRODUCT"),		_T(""));		
	m_sCurrentPPID		= iniFile.GetString(sSection,	_T("CURRENT PPID"),		_T(""));		
	m_sTrayQtyInLot		= iniFile.GetString(sSection,	_T("TRAY QTY IN LOT"),		_T("30"));
	m_sCellQtyInTray	= iniFile.GetString(sSection,	_T("CELL QTY IN TRAY"),		_T("8"));
	m_sNextStepID		= iniFile.GetString(sSection,	_T("NEXT STEPID"),		_T("AMT"));

}

// ���ŵ� Job file download���� ������ �����Ѵ�.
void CUnLoadRuleInfo::SaveUnloadRuleFileInfo()
{
	CString sSection;
	CString sKey;
	CString strPath = _T("D:\\Cheetah\\System\\UnLoadRuleInfo.ini");			// ���´������� S8F1�� ���Ź��� �� ����Ѵ�.
	CIni iniFile(strPath);	

	sSection = "UNLOAD RULE INFO";

	// Job file download ó�� ����
	iniFile.WriteString(sSection,	_T("CURRENT PRODUCT"),		m_sCurrentProduct);			// ���� ������� �ܰ�
	iniFile.WriteString(sSection,	_T("CURRENT PPID"),			m_sCurrentPPID);			// ���� ��ȣ��� ���� ���Ź��� ����
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
	// �ش� ī��Ʈ�� Ʈ���� Lot�� �� �� �����̴�.
	//nResult = _tstoi(m_sCellQtyInTray.GetString());
	return nResult;
}
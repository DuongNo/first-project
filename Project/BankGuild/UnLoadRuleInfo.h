#pragma once
class CUnLoadRuleInfo
{
public:
	CUnLoadRuleInfo(void);
	~CUnLoadRuleInfo(void);


	enum { 
		// int m_nJobProcessSeq 의 값
		eJOBChange_None					= 0,						// 아무런 진행을 하지않는다.
		eJOBChange_WaitUserConfirm,								// 정보를 수신받고 작업자의 적용 결정을 기다린다.			
		
		
		
		eJOBChange_Finish,											// 처리를 모두 마쳤다.

											
	};


CString m_sCurrentProduct;		
CString m_sCurrentPPID;
CString m_sTrayQtyInLot;
CString m_sCellQtyInTray;	
CString m_sNextStepID;

//2017-05-17,skcho, 301/302 보고시 CELL ID 보고 추가 CIM QUAL 요청
CString m_sStartCellID;
CString m_sEndCellID;

void ReadUnloadRuleFileInfo();
void SaveUnloadRuleFileInfo();

void SetJobChangeCase2Step(int nStep);
int  GetJobChangeCase2Step();
CString GetUnloadingRuleInfoCurrentPPID();
void SetJobChangeType(int nType);
int GetJobChangeType();
int GetTrayLotQtyInt();
int GetCellInTrayQtyInt();

protected:
	int m_nJobChangeCase2Step;
	int m_nJobChangeType;

};


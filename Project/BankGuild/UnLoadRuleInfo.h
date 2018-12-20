#pragma once
class CUnLoadRuleInfo
{
public:
	CUnLoadRuleInfo(void);
	~CUnLoadRuleInfo(void);


	enum { 
		// int m_nJobProcessSeq �� ��
		eJOBChange_None					= 0,						// �ƹ��� ������ �����ʴ´�.
		eJOBChange_WaitUserConfirm,								// ������ ���Źް� �۾����� ���� ������ ��ٸ���.			
		
		
		
		eJOBChange_Finish,											// ó���� ��� ���ƴ�.

											
	};


CString m_sCurrentProduct;		
CString m_sCurrentPPID;
CString m_sTrayQtyInLot;
CString m_sCellQtyInTray;	
CString m_sNextStepID;

//2017-05-17,skcho, 301/302 ����� CELL ID ���� �߰� CIM QUAL ��û
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


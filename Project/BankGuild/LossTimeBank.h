#pragma once

#include "File\GxIni.h"
#include "File\GxLogData.h"			// Cell Log�� ���� class

#include "BankGuild\MCDefine.h"


class CLossTimeBank
{
public:
	CLossTimeBank(void);
	~CLossTimeBank(void);


		// �񰡵� �׸� ���� ������ -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	TLosstimeList		m_rLosstimeList;						// �񰡵� �׸���� �����͸� �����ϴ� ����ü
	TLosstimeRecord		m_rLosstimeRecord;							// �񰡵� Log�� ����� ���� ������ ���

	CString             m_sTPSystemCode;                         //2017-03-25,skcho �߰�
	CString             m_sTPSystemName;
	
	CGxIni				m_iniLosstime;			// �񰡵� ������ �ӽ� ����Ѵ�.	
	CGxLogData			m_logLosstime;						// �񰡵� �α�

	BOOL LoadLosstimeItemList();
	
	// �񰡵� Log
//	CGxLogData	m_logLosstimeData;						// �񰡵� Log�� ����Ѵ�. (���ο� ���)
	void SetLosstimeLogPath(void* pValue);			// �񰡵� Log �����̸� ���� callback �Լ�
	void WriteLossTimeRecord(TLosstimeRecord& rRec, BOOL bPrevDay);		//���ο� ������� ������ �񰡵� ���ڵ� �����͸� ���Ϸ� ����Ѵ�.
	void WriteAutoLossTimeRecord(TLosstimeRecord& rRec);

	void InitLosstimeInfo();					// �񰡵� ������ �ӽ� ����ϱ����� �ʱ�ȭ
	void LoadLosstimeInfo();					// ���Ͽ� ����� �񰡵� ������ �о�´�. (Program �����)
	void SaveLosstimeInfo();					// ���� ������ �񰡵� ������ ���Ϸ� ����Ѵ�.
	void ClearLosstimeInfo();					// �񰡵� ���� ������ ���ٸ� ������ û�����ش�.

	void WriteLossTimeByAlarm();				// Alarm�� ���� ���������� �񰡵� ���°� ����Ǿ��ٰ� �����Ѵ�.
	void WriteOPLossTime(int nJigNo);		// �۾��� �ð��ʰ� ��� (�񰡵� �α׿� �����)
	void WriteEQLossTime(int nJigNo);		// ���� �ð��ʰ� ��� (�񰡵� �α׿� �����)

	// �񰡵� �α��� �Ϻ�, ���� �������� �������ش�.
	void LosstimeDataCloseProcess(BOOL bPrevDay);

	//�񰡵��α� ���� ����
	void InitialDataFile();
};


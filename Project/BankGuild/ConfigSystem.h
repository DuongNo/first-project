#pragma once

//////////////////////////////////////////////////////////////////////////
// ConfigSystem
// 1. ��� Parameter �߿��� �ֿ켱���� �д´�.
// 2. �߿�õǴ� ������ �������� �����ϴ� ��
// 2. �Ժη� �ٲ��� �ʴ� �κ��̹Ƿ� ������ ���� UI�� �������� �ʴ´�.
//////////////////////////////////////////////////////////////////////////
// System ������ �׻� �������� ����...
#define PATH_SYSTEM	_T("D:\\Cheetah\\System")

//////////////////////////////////////////////////////////////////////////

class CConfigSystem
{
public:
	CConfigSystem(void);
	~CConfigSystem(void);

	//////////////////////////////////////////////////////////////////////////
	// Path ����
	CString m_strDebugLogPath;
	CString m_strRecipePath;
	CString m_strRecipeBackupPath;
	CString m_strConfigPath;
	CString m_strConfigBackupPath;
	CString m_strCellLogPath;
	CString m_strLineNo;
	CString m_strImagePath;
	CString m_strLossTimeLogPath;

	CString m_strActive1Path;
	CString m_strActive2Path;
	CString m_strVTLocalPath;

	//20161227 byskcho
	CString m_strFDCPath;

	//20170118 byskcho
	/*CString m_strEQ_ID;
	CString m_strModel_Name;
	CString m_strPP_ID;
	*/

	//20170128 byskcho
	CString m_strDataPc_IP;

	CString m_strPassword;
	//2017-11-03, YMG ���� ��й�ȣ 
	CString m_strLampPassword;

	CString m_strCarrierReleaseReportPath;//2017-02-27, byskcho,  ���� ���� ���� Ʈ���� ���� write ��� ����
	CString m_strCarrierCloseReportPath;
	
	CString m_strCarrierAssignReportPath;

	CString m_strCarrierReqCellIDReportPath;//2018wjpCIM
	CString m_strCarrierLoadCellIDPath;     //2018wjpCIM

	//////////////////////////////////////////////////////////////////////////


	//2017-02-26, jhLee, Job file download ������ ���� ������
	enum 
	{ 
		eMAX_PGCount		= 8,								// ���� ��ȣ��� 4 JIG * 2 CH = 8���̴�.
		eMAX_JobFileCount = 30,								// �ϴ� �ִ� Download ó���� ������ ���� 30�� ������ ��Ƴ��´�.
	};

	enum { 
		// int m_nJobProcessSeq �� ��
		eJOB_None					= 0,						// �ƹ��� ������ �����ʴ´�.
		eJOB_WaitUserConfirm,								// ������ ���Źް� �۾����� ���� ������ ��ٸ���.			
		eJOB_Route,												// TxHost���� Route ������ �����Ͽ���.
		eJOB_TMDSend,											// TxHost���� Download�� ���� ������ �����Ͽ���.
		eJOB_CopyStart,										// TxHost���� TMD_COPY_START�� �����Ͽ���.
		eJOB_ChecksumStart,									// TxHost���� TMD_CHECKSUM_START�� �����Ͽ���.
		eJOB_ClientChecksum,									// TxHost���� CLIENT_CHECKSUM_START�� �����Ͽ���.
		eJOB_DownloadStart,									// TxHost���� TMD_DOWNLOAD_START�� �����Ͽ���.
		eJOB_Finish,											// ó���� ��� ���ƴ�.

		// int m_nJobProcessState �� ��
		eSTATE_JOB_None			= 0,						// ���� �ƹ��� ���۵� ���� �ʾҴ�.
		eSTATE_JOB_Send,										// ����� �����ϰ� ����� ��ٸ��� �ִ�.
		eSTATE_JOB_GOOD,										// ���������� ó���� ���ƴ�.
		eSTATE_JOB_NG,											// ������ ����� ���������� ó������ ���Ͽ���.
	};

	// Job file download ó�� ����
	int		m_nJobProcessSeq;							// ���� ������� �ܰ�
	int		m_nJobProcessRcv;							// ���� ��ȣ��� ���� ���Ź��� ����
	int		m_nJobProcessState;							// ���� ��������� ���
	CString	m_strActionFlag;							// ���� ����
	
	int     m_nJobProcessStep;                           //������ �� �����μ��� ���� ����
	int     m_bJobStart;
	// MES���� ���� Job download�� ���� ������
	int		m_nJobFileCount;								// MES�� ���� ��û���� Job file�� count
	CString	m_strServerPath;								// MES file server�� ���
	CString	m_strJobFilename1[eMAX_JobFileCount];	// MES�κ��� ���Ź��� ���� �̸�
	CString	m_strJobFilename2[eMAX_JobFileCount];	// ��ȣ��κ��� ���Ź��� ���� �̸�
	CString	m_strChecksum1[eMAX_JobFileCount];		// �������� ���� Checksum
	CString	m_strChecksum2[eMAX_JobFileCount];		// ��ȣ�⿡�� ���� Checksum

	// ���� ��ȣ���� ����, ��� Check�� �� ������ ���� ���� ������
	CString	m_strCurrentTMD[eMAX_PGCount];			// ���� �� ��ȣ���� �������� ���� TMD file �̸�
	CString	m_strCurrentChecksum[eMAX_PGCount];		// �� ��ȣ�� TMD ������ Checksum ����
	CString	m_strCurrentMD5[eMAX_PGCount];			// �� ��ȣ�� MD5 ó�� ���

	CString GetTMDFileList();								// ��ȣ��� ���� download file list�� ���ڿ��� ������ش�.
	void SetTMDServerPath(CString sPath);				// MES File server�� ��� ����
	void SetTMDFileCount(int nCount);					// ���Ź��� File List�� ������ �����Ѵ�.
	void SetTMDFileInfo(int nIdx, CString sFile, CString sCheck);		// ���������� �����̸��� Checksum�� �����Ѵ�.
	void SetTMDRcvFileInfo(int nIdx, CString sFile, CString sCheck);	// ���������� ��ȣ��κ��� �о���� �����̸��� Checksum�� �����Ѵ�.
	void SaveSystemFile(CString strPath);				// System ������ �����Ѵ�.


	void ReadTMDJobFileInfo();							// �������� ������ Job file download���� ������ �о�´�.
	void SaveTMDJobFileInfo();							// ���ŵ� Job file download���� ������ �����Ѵ�.

	void ReadSystemFile(CString strPath);
	
};


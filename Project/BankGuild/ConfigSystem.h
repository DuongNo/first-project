#pragma once

//////////////////////////////////////////////////////////////////////////
// ConfigSystem
// 1. 모든 Parameter 중에서 최우선으로 읽는다.
// 2. 중요시되는 폴더나 설정등을 저장하는 곳
// 2. 함부로 바뀌지 않는 부분이므로 가급적 편집 UI는 구현하지 않는다.
//////////////////////////////////////////////////////////////////////////
// System 폴더는 항상 고정으로 가자...
#define PATH_SYSTEM	_T("D:\\Cheetah\\System")

//////////////////////////////////////////////////////////////////////////

class CConfigSystem
{
public:
	CConfigSystem(void);
	~CConfigSystem(void);

	//////////////////////////////////////////////////////////////////////////
	// Path 정보
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
	//2017-11-03, YMG 램프 비밀번호 
	CString m_strLampPassword;

	CString m_strCarrierReleaseReportPath;//2017-02-27, byskcho,  상위 보고를 위한 트레이 정보 write 경로 지정
	CString m_strCarrierCloseReportPath;
	
	CString m_strCarrierAssignReportPath;

	CString m_strCarrierReqCellIDReportPath;//2018wjpCIM
	CString m_strCarrierLoadCellIDPath;     //2018wjpCIM

	//////////////////////////////////////////////////////////////////////////


	//2017-02-26, jhLee, Job file download 수행을 위한 데이터
	enum 
	{ 
		eMAX_PGCount		= 8,								// 현재 신호기는 4 JIG * 2 CH = 8개이다.
		eMAX_JobFileCount = 30,								// 일단 최대 Download 처리할 파일의 수는 30개 정도로 잡아놓는다.
	};

	enum { 
		// int m_nJobProcessSeq 의 값
		eJOB_None					= 0,						// 아무런 진행을 하지않는다.
		eJOB_WaitUserConfirm,								// 정보를 수신받고 작업자의 적용 결정을 기다린다.			
		eJOB_Route,												// TxHost에게 Route 정보를 전송하였다.
		eJOB_TMDSend,											// TxHost에게 Download할 파일 정보를 전송하였다.
		eJOB_CopyStart,										// TxHost에게 TMD_COPY_START를 전송하였다.
		eJOB_ChecksumStart,									// TxHost에게 TMD_CHECKSUM_START를 전송하였다.
		eJOB_ClientChecksum,									// TxHost에게 CLIENT_CHECKSUM_START를 전송하였다.
		eJOB_DownloadStart,									// TxHost에게 TMD_DOWNLOAD_START를 전송하였다.
		eJOB_Finish,											// 처리를 모두 마쳤다.

		// int m_nJobProcessState 의 값
		eSTATE_JOB_None			= 0,						// 아직 아무런 동작도 하지 않았다.
		eSTATE_JOB_Send,										// 명령을 전송하고 결과를 기다리고 있다.
		eSTATE_JOB_GOOD,										// 정상적으로 처리를 마쳤다.
		eSTATE_JOB_NG,											// 전송한 명령을 정상적으로 처리하지 못하였다.
	};

	// Job file download 처리 과정
	int		m_nJobProcessSeq;							// 현재 진행과정 단계
	int		m_nJobProcessRcv;							// 현재 신호기로 부터 수신받은 과정
	int		m_nJobProcessState;							// 현재 진행과정의 결과
	CString	m_strActionFlag;							// 동작 과정
	
	int     m_nJobProcessStep;                           //스레드 내 잡프로세스 스텝 변수
	int     m_bJobStart;
	// MES에서 받은 Job download를 위한 데이터
	int		m_nJobFileCount;								// MES로 부터 요청받은 Job file의 count
	CString	m_strServerPath;								// MES file server의 경로
	CString	m_strJobFilename1[eMAX_JobFileCount];	// MES로부터 수신받은 파일 이름
	CString	m_strJobFilename2[eMAX_JobFileCount];	// 신호기로부터 수신받은 파일 이름
	CString	m_strChecksum1[eMAX_JobFileCount];		// 상위에서 받은 Checksum
	CString	m_strChecksum2[eMAX_JobFileCount];		// 신호기에서 받은 Checksum

	// 현재 신호기의 정보, 상시 Check할 때 상위에 보고를 위한 데이터
	CString	m_strCurrentTMD[eMAX_PGCount];			// 현재 각 신호기의 적용중인 현재 TMD file 이름
	CString	m_strCurrentChecksum[eMAX_PGCount];		// 각 신호기 TMD 파일의 Checksum 정보
	CString	m_strCurrentMD5[eMAX_PGCount];			// 각 신호기 MD5 처리 결과

	CString GetTMDFileList();								// 신호기로 보낼 download file list를 문자열로 만들어준다.
	void SetTMDServerPath(CString sPath);				// MES File server의 경로 지정
	void SetTMDFileCount(int nCount);					// 수신받을 File List의 수량을 지정한다.
	void SetTMDFileInfo(int nIdx, CString sFile, CString sCheck);		// 지정순번의 파일이름과 Checksum을 지정한다.
	void SetTMDRcvFileInfo(int nIdx, CString sFile, CString sCheck);	// 지정순번의 신호기로부터 읽어들인 파일이름과 Checksum을 지정한다.
	void SaveSystemFile(CString strPath);				// System 정보를 저장한다.


	void ReadTMDJobFileInfo();							// 상위에서 보내온 Job file download관련 정보를 읽어온다.
	void SaveTMDJobFileInfo();							// 갱신된 Job file download관련 정보를 저장한다.

	void ReadSystemFile(CString strPath);
	
};


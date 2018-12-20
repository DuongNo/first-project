
#pragma once

// Thread ������ŭ �Ʒ� INDEX�� �ø��� [8/2/2016 OSC]
enum THREAD_INDEX
{
	THREAD_INDEX_MAIN_SEQUENCE,
	THREAD_INDEX_LOADER_INTERFACE,
	THREAD_INDEX_AGV_INTERFACE,
	THREAD_INDEX_TRAY_SEND,
	THREAD_INDEX_IO_CHECK,
	THREAD_INDEX_MODEL_CHANGE_CHECK,//2018wjp
	THREAD_INDEX_FDC_MONITOR,
	THREAD_INDEX_FDC_MONITOR_VACUUM,
	THREAD_INDEX_TMD_DOWNLOAD,
	//THREAD_INDEX_T5_RESTART,    //2017-09-26,SKCHO
	THREAD_INDEX_TRAY_MANUAL_INTERFACE,
	//THREAD_INDEX_MELSEC_MONITOR,   //2018-03-03,SKCHO
	THREAD_INDEX_VT_FILE,
	THREAD_INDEX_EVENT_VT_FILE,
	THREAD_INDEX_MAX
	
};


class CThreadInitializer
{
public:
	CThreadInitializer();
	~CThreadInitializer();


protected:
	CWinThread* m_pThr[THREAD_INDEX_MAX];
	HANDLE		m_hThr[THREAD_INDEX_MAX];

public:
	BOOL		m_bThrExist[THREAD_INDEX_MAX];

	BOOL CreateThread(THREAD_INDEX index, AFX_THREADPROC pThr, LPVOID pParam);
	BOOL ReleaseThread(THREAD_INDEX index);

	BOOL CreateLoadVTFileThread(AFX_THREADPROC pThr, LPVOID pParam);
	DWORD WaitLoadVTFileThread();
	BOOL StartLoadVTFileThread();

};
#include "StdAfx.h"
#include "ConfigSystem.h"
#include "Etc/Ini.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CConfigSystem::CConfigSystem(void)
{
	m_bJobStart = FALSE;
}


CConfigSystem::~CConfigSystem(void)
{
}

void CConfigSystem::ReadSystemFile( CString strPath )
{
	CIni ini(strPath);	

	//2018-03-07,SKCHO , 사용하지 않아삭제
	//20170118 byskcho
	/*m_strEQ_ID = ini.GetString(_T("SETTING"), _T("EQID"), _T(""));
	m_strModel_Name = ini.GetString(_T("SETTING"), _T("MODEL NAME"), _T(""));
	m_strPP_ID = ini.GetString(_T("SETTING"), _T("PPID"), _T(""));
	*/


	m_strPassword = ini.GetString(_T("SETTING"), _T("PASSWORD"), _T("4871"));
	//2017-11-03, YMG 램프 비밀번호 
	m_strLampPassword = ini.GetString(_T("SETTING"), _T("PASSWORD"), _T("3654"));
	
	m_strDataPc_IP = ini.GetString(_T("IP_ADDRESS"), _T("DATA_PC"), _T(""));

	m_strDebugLogPath = ini.GetString(_T("PATH"), _T("DEBUG LOG"), _T("D:\\Cheetah\\Log\\Debug"));
	m_strRecipePath = ini.GetString(_T("PATH"), _T("RECIPE"), _T("D:\\Cheetah\\Recipe"));
	m_strRecipeBackupPath = ini.GetString(_T("PATH"), _T("RECIPE BACKUP"), _T("D:\\Cheetah\\Backup\\Recipe"));
	m_strConfigPath = ini.GetString(_T("PATH"), _T("CONFIG"), _T("D:\\Cheetah\\Config"));
	m_strConfigBackupPath = ini.GetString(_T("PATH"), _T("CONFIG BACKUP"), _T("D:\\Cheetah\\Backup\\Config"));
	m_strCellLogPath = ini.GetString(_T("PATH"), _T("CELL LOG"), _T("D:\\auto-test"));
	
	m_strImagePath = ini.GetString(_T("PATH"), _T("IMAGE"), _T("D:\\Cheetah\\Source\\Project\\res"));

	m_strLossTimeLogPath = ini.GetString(_T("PATH"), _T("LOSS TIME LOG"), _T("D:\\SIRIUS\\LOG\\MAIN"));

	m_strActive1Path = ini.GetString(_T("PATH"), _T("m_strActive1Path"), _T("Z:\\"));
	m_strActive2Path = ini.GetString(_T("PATH"), _T("m_strActive2Path"), _T("Y:\\"));
	m_strVTLocalPath = ini.GetString(_T("PATH"), _T("m_strVTLocalPath"), _T("D:\\Cheetah\\Backup\\VT"));


	m_strFDCPath = ini.GetString(_T("PATH"), _T("FDC"), _T("D:\\FDC"));
  
	m_strLineNo = ini.GetString(_T("SYSTEM_PARAMETER"), _T("LINE NO"), _T("01A"));

	m_strCarrierReleaseReportPath = ini.GetString(_T("PATH"), _T("CARRIERREPORT"),  _T("D:\\Cheetah\\Runtime\\Carrier_ReleaseReport.ini"));//2017-02-27,skcho ,트레이 정보를 저장할 ini 파일 경로 생성
	m_strCarrierCloseReportPath = ini.GetString(_T("PATH"), _T("CARRIERREPORT"),  _T("D:\\Cheetah\\Runtime\\Carrier_CloseReport.ini"));//2017-02-27,skcho ,트레이 정보를 저장할 ini 파일 경로 생성
	m_strCarrierAssignReportPath = ini.GetString(_T("PATH"), _T("CARRIERREPORT"),  _T("D:\\Cheetah\\Runtime\\Carrier_AssignReport.ini"));//2017-02-27,skcho ,트레이 정보를 저장할 ini 파일 경로 생성
	m_strCarrierReqCellIDReportPath=ini.GetString(_T("PATH"), _T("CARRIERREPORT"),  _T("D:\\Cheetah\\Runtime\\Carrier_ReqCellIDReport.ini"));//2018wjpCIM
	m_strCarrierLoadCellIDPath=ini.GetString(_T("PATH"), _T("cCarrierReportCellIdResult_PATH"),  _T("D:\\Cheetah\\Runtime\\CarrierCellID\\Carrier_CellID.ini"));//2018wjpCIM
	
	UINT nTemp = 0;
	nTemp = ini.GetLong(_T("GMS"),_T("PortNum"),23L);
	theSerialInterFace.SetSerialInfo(SERIAL_GMS,nTemp);

	nTemp = ini.GetLong(_T("PC TMEP"),_T("PortNum"),11L);
	theSerialInterFace.SetSerialInfo(SERIAL_TEMPERATURE,nTemp);

	nTemp = ini.GetLong(_T("VACUUM LD"),_T("PortNum"),15L);
	theSerialInterFace.SetSerialInfo(SERIAL_VACUUM_LD,nTemp);

	nTemp = ini.GetLong(_T("VACUUM INSP"),_T("PortNum"),18L);
	theSerialInterFace.SetSerialInfo(SERIAL_VACUUM_INSP,nTemp);

	nTemp = ini.GetLong(_T("VACUUM UD"),_T("PortNum"),14L);
	theSerialInterFace.SetSerialInfo(SERIAL_VACUUM_UD,nTemp);


	nTemp = ini.GetLong(_T("LABELPRINTER"),_T("PortNum"),0L);
	theSerialInterFace.SetSerialInfo(SERIAL_LABELPRINT,nTemp);
	
	nTemp = ini.GetLong(_T("SERVOTORQUE"),_T("PortNum"),0L);
	theSerialInterFace.SetSerialInfo(SERIAL_SERVOTORQUE,nTemp);







}


// System 정보를 저장한다.
void CConfigSystem::SaveSystemFile(CString strPath)
{

}





// 상위에서 보내온 Job file download관련 정보를 읽어온다.
void CConfigSystem::ReadTMDJobFileInfo()
{
	CString sSection;
	CString sKey;
	CString strPath = _T("D:\\Cheetah\\Runtime\\TMDDataList.ini");			// 가온누리에서 S8F1을 수신받을 때 기록한다.
	CString FileServerPath;
	CString FileBackupPath;		
	FileBackupPath = _T("D:\\Cheetah\\Runtime\\TMDBackUp");	
	CString FileBackupFullPath;		
	CIni iniFile(strPath);	

	sSection = "TMDDownload";

	//2017-02-26, jhLee, Job file download 수행을 위한 데이터를 파일에서 읽어온다.
	// Job file download 처리 과정
	m_nJobProcessSeq	= iniFile.GetLong(sSection,	_T("JobProcessSeq"),		0L);		// 현재 진행과정 단계
	m_nJobProcessRcv	= iniFile.GetLong(sSection,	_T("JobProcessRcv"),		0L);		// 현재 신호기로 부터 수신받은 과정
	m_nJobProcessState	= iniFile.GetLong(sSection,	_T("JobProcessState"),		0L);	// = 0,	// 아직 아무런 동작도 하지 않았다.

	// 상위 보고를 위한 데이터
	m_strActionFlag = iniFile.GetString(sSection,	_T("ActionFlag"),	_T(""));		// ACTION FLAG // 동작 과정		
	m_strServerPath = iniFile.GetString(sSection,	_T("ServerPath"),	_T(""));		// MES file server의 경로	
	m_nJobFileCount = iniFile.GetLong(sSection,		_T("ListCount"),	0);				// MES로 부터 요청받은 Job file의 count
			
	for (int i = 0; i < eMAX_JobFileCount; i++)
	{
		if ( i < m_nJobFileCount )					// 저장된 데이터가 존재하는 순번이면
		{
			sKey.Format(_T("FileName_%d"), i);
			m_strJobFilename1[i] = iniFile.GetString(sSection,	sKey,	_T(""));	// S8F1에서 정한 File Name

			sKey.Format(_T("Checksum1_%d"), i);
			m_strChecksum1[i] = iniFile.GetString(sSection,	sKey,	_T(""));		// S8F1에서 정한 Checksum

			sKey.Format(_T("Checksum2_%d"), i);
			m_strChecksum2[i] = iniFile.GetString(sSection,	sKey,	_T(""));		// 현재  Checksum

			// 저장은 되었지만 처리하지 않아서 읽어오지 않는 내용
			// sKey.Format("FileType_%d", i);
			// sKey.Format("FilePath_%d", i);
			
			// 잡파일 다운로드 정보 읽을때 운영으로 해당 파일 복사하여 놓는다. 신호기에서 해당 파일 복사하도록 한다. 2017/06/12 HSK.
			FileBackupFullPath.Format(_T("%s%s"),FileBackupPath,m_strServerPath);
			if(CFileSupport::DirectoryCheck(FileBackupFullPath) == FALSE)
			{
				if(CFileSupport::CreateDirectory(FileBackupFullPath) == FALSE)
					return;
			}
			FileBackupFullPath.Format(_T("%s%s%s"),FileBackupPath,m_strServerPath,m_strJobFilename1[i]);

			FileServerPath.Format(_T("W:%s%s"),m_strServerPath,m_strJobFilename1[i]);
			::CopyFile(FileServerPath,FileBackupFullPath, FALSE);
		}
		else	// 그 외는 초기화를 한다.
		{
			m_strJobFilename1[i]	= _T("");
			m_strJobFilename2[i]	= _T("");
			m_strChecksum1[i]		= _T("");
			m_strChecksum2[i]		= _T("");
		}

	}//for i

	// 현재 신호기의 정보, 상시 Check할 때 상위에 보고를 위한 데이터
	for (int i = 0; i < eMAX_PGCount; i++)
	{
		sKey.Format(_T("TMD_Name_%d"), i+1);
		m_strCurrentTMD[i]		= iniFile.GetString(_T("PG_TMD_INFO"), sKey,	 _T(""));	// 현재 각 신호기의 적용중인 현재 TMD file 이름

		sKey.Format(_T("Checksum_%d"), i+1);
		m_strCurrentChecksum[i]	= iniFile.GetString(_T("PG_TMD_INFO"), sKey,	 _T(""));	// 각 신호기 TMD 파일의 Checksum 정보

		sKey.Format(_T("MD5_Result_%d"), i+1);
		m_strCurrentMD5[i]		= iniFile.GetString(_T("PG_TMD_INFO"), sKey,	_T(""));				// 각 신호기 MD5 처리 결과
	}

}


// 갱신된 Job file download관련 정보를 저장한다.
void CConfigSystem::SaveTMDJobFileInfo()
{
	CString sSection;
	CString sKey;
	CString strPath = _T("D:\\Cheetah\\Runtime\\TMDDataList.ini");			// 가온누리에서 S8F1을 수신받을 때 기록한다.
	CIni iniFile(strPath);	

	sSection = "TMDDownload";

	// Job file download 처리 과정
	iniFile.WriteLong(sSection,	_T("JobProcessSeq"),		m_nJobProcessSeq);			// 현재 진행과정 단계
	iniFile.WriteLong(sSection,	_T("JobProcessRcv"),		m_nJobProcessRcv);			// 현재 신호기로 부터 수신받은 과정
	iniFile.WriteLong(sSection,	_T("JobProcessState"),		m_nJobProcessState);		// = 0,	// 아직 아무런 동작도 하지 않았다.

	// 상위 보고를 위한 데이터
	iniFile.WriteString(sSection,	_T("ActionFlag"),	m_strActionFlag);		// ACTION FLAG // 동작 과정		
			
	for (int i = 0; i < eMAX_JobFileCount; i++)
	{
		if ( i < m_nJobFileCount )					// 저장된 데이터가 존재하는 순번이면
		{
			sKey.Format(_T("Checksum2_%d"), i);
			iniFile.WriteString(sSection,	sKey,	m_strChecksum2[i]);		// 현재  Checksum

			//sKey.Format(_T("FileName_%d"), i);
			//sKey.Format(_T("Checksum1_%d"), i);

			// 저장은 되었지만 처리하지 않아서 읽어오지 않는 내용
			// sKey.Format("FileType_%d", i);
			// sKey.Format("FilePath_%d", i);
		}
	}//for i

	// 현재 신호기의 정보, 상시 Check할 때 상위에 보고를 위한 데이터
	for (int i = 0; i < eMAX_PGCount; i++)
	{
		sKey.Format(_T("TMD_Name_%d"), i+1);
		iniFile.WriteString(_T("PG_TMD_INFO"), sKey,	m_strCurrentTMD[i]);		// 현재 각 신호기의 적용중인 현재 TMD file 이름

		sKey.Format(_T("Checksum_%d"), i+1);
		iniFile.WriteString(_T("PG_TMD_INFO"), sKey,	m_strCurrentChecksum[i]);	// 각 신호기 TMD 파일의 Checksum 정보

		sKey.Format(_T("MD5_Result_%d"), i+1);
		iniFile.WriteString(_T("PG_TMD_INFO"), sKey,	m_strCurrentMD5[i]);		// 각 신호기 MD5 처리 결과
	}

}



// 신호기로 보낼 download file list를 문자열로 만들어준다.
CString CConfigSystem::GetTMDFileList()
{
	CString sResult = _T("");

	sResult.Format(_T("%d"), m_nJobFileCount);

	for (int i = 0; i < m_nJobFileCount; i++)
	{
		sResult.AppendFormat(_T(",%s,%s"), m_strJobFilename1[i], m_strChecksum1[i]);
	}

	return sResult;
}

// MES File server의 경로 지정
void CConfigSystem::SetTMDServerPath(CString sPath)
{
	m_strServerPath = sPath;
}

// 수신받을 File List의 수량을 지정한다.
void CConfigSystem::SetTMDFileCount(int nCount)
{
	if ( nCount > eMAX_JobFileCount )
	{
		m_nJobFileCount = eMAX_JobFileCount;
	}
	else
		m_nJobFileCount = nCount;
}

// 지정순번의 파일이름과 Checksum을 지정한다.
void CConfigSystem::SetTMDFileInfo(int nIdx, CString sFile, CString sCheck)
{
	if ( (nIdx >= 0) && (nIdx < eMAX_JobFileCount) )
	{
		m_strJobFilename1[nIdx] = sFile;
		m_strChecksum1[nIdx]		= sCheck;
	}

}

// 지정순번의 신호기로부터 읽어들인 파일이름과 Checksum을 지정한다.
void CConfigSystem::SetTMDRcvFileInfo(int nIdx, CString sFile, CString sCheck)
{
	if ( (nIdx >= 0) && (nIdx < eMAX_JobFileCount) )
	{
		m_strJobFilename2[nIdx] = sFile;
		m_strChecksum2[nIdx]		= sCheck;
	}

}





// File server 경로
	//AMT	/jobfile/AMT/HIST/	\\107.126.10.111\JOBFILEAMTHIST	/jobfile/AMT/HIST/
	//AMT	/jobfile/AMT/SPEC	\\107.126.10.111\JOBFILEAMTSPEC	/jobfile/AMT/SPEC
	//AMT	/jobfile/AMT/CURR	\\107.126.10.111\JOBFILEAMTCURR	/jobfile/AMT/CURR

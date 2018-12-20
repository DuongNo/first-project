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

	//2018-03-07,SKCHO , ������� �ʾƻ���
	//20170118 byskcho
	/*m_strEQ_ID = ini.GetString(_T("SETTING"), _T("EQID"), _T(""));
	m_strModel_Name = ini.GetString(_T("SETTING"), _T("MODEL NAME"), _T(""));
	m_strPP_ID = ini.GetString(_T("SETTING"), _T("PPID"), _T(""));
	*/


	m_strPassword = ini.GetString(_T("SETTING"), _T("PASSWORD"), _T("4871"));
	//2017-11-03, YMG ���� ��й�ȣ 
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

	m_strCarrierReleaseReportPath = ini.GetString(_T("PATH"), _T("CARRIERREPORT"),  _T("D:\\Cheetah\\Runtime\\Carrier_ReleaseReport.ini"));//2017-02-27,skcho ,Ʈ���� ������ ������ ini ���� ��� ����
	m_strCarrierCloseReportPath = ini.GetString(_T("PATH"), _T("CARRIERREPORT"),  _T("D:\\Cheetah\\Runtime\\Carrier_CloseReport.ini"));//2017-02-27,skcho ,Ʈ���� ������ ������ ini ���� ��� ����
	m_strCarrierAssignReportPath = ini.GetString(_T("PATH"), _T("CARRIERREPORT"),  _T("D:\\Cheetah\\Runtime\\Carrier_AssignReport.ini"));//2017-02-27,skcho ,Ʈ���� ������ ������ ini ���� ��� ����
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


// System ������ �����Ѵ�.
void CConfigSystem::SaveSystemFile(CString strPath)
{

}





// �������� ������ Job file download���� ������ �о�´�.
void CConfigSystem::ReadTMDJobFileInfo()
{
	CString sSection;
	CString sKey;
	CString strPath = _T("D:\\Cheetah\\Runtime\\TMDDataList.ini");			// ���´������� S8F1�� ���Ź��� �� ����Ѵ�.
	CString FileServerPath;
	CString FileBackupPath;		
	FileBackupPath = _T("D:\\Cheetah\\Runtime\\TMDBackUp");	
	CString FileBackupFullPath;		
	CIni iniFile(strPath);	

	sSection = "TMDDownload";

	//2017-02-26, jhLee, Job file download ������ ���� �����͸� ���Ͽ��� �о�´�.
	// Job file download ó�� ����
	m_nJobProcessSeq	= iniFile.GetLong(sSection,	_T("JobProcessSeq"),		0L);		// ���� ������� �ܰ�
	m_nJobProcessRcv	= iniFile.GetLong(sSection,	_T("JobProcessRcv"),		0L);		// ���� ��ȣ��� ���� ���Ź��� ����
	m_nJobProcessState	= iniFile.GetLong(sSection,	_T("JobProcessState"),		0L);	// = 0,	// ���� �ƹ��� ���۵� ���� �ʾҴ�.

	// ���� ���� ���� ������
	m_strActionFlag = iniFile.GetString(sSection,	_T("ActionFlag"),	_T(""));		// ACTION FLAG // ���� ����		
	m_strServerPath = iniFile.GetString(sSection,	_T("ServerPath"),	_T(""));		// MES file server�� ���	
	m_nJobFileCount = iniFile.GetLong(sSection,		_T("ListCount"),	0);				// MES�� ���� ��û���� Job file�� count
			
	for (int i = 0; i < eMAX_JobFileCount; i++)
	{
		if ( i < m_nJobFileCount )					// ����� �����Ͱ� �����ϴ� �����̸�
		{
			sKey.Format(_T("FileName_%d"), i);
			m_strJobFilename1[i] = iniFile.GetString(sSection,	sKey,	_T(""));	// S8F1���� ���� File Name

			sKey.Format(_T("Checksum1_%d"), i);
			m_strChecksum1[i] = iniFile.GetString(sSection,	sKey,	_T(""));		// S8F1���� ���� Checksum

			sKey.Format(_T("Checksum2_%d"), i);
			m_strChecksum2[i] = iniFile.GetString(sSection,	sKey,	_T(""));		// ����  Checksum

			// ������ �Ǿ����� ó������ �ʾƼ� �о���� �ʴ� ����
			// sKey.Format("FileType_%d", i);
			// sKey.Format("FilePath_%d", i);
			
			// ������ �ٿ�ε� ���� ������ ����� �ش� ���� �����Ͽ� ���´�. ��ȣ�⿡�� �ش� ���� �����ϵ��� �Ѵ�. 2017/06/12 HSK.
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
		else	// �� �ܴ� �ʱ�ȭ�� �Ѵ�.
		{
			m_strJobFilename1[i]	= _T("");
			m_strJobFilename2[i]	= _T("");
			m_strChecksum1[i]		= _T("");
			m_strChecksum2[i]		= _T("");
		}

	}//for i

	// ���� ��ȣ���� ����, ��� Check�� �� ������ ���� ���� ������
	for (int i = 0; i < eMAX_PGCount; i++)
	{
		sKey.Format(_T("TMD_Name_%d"), i+1);
		m_strCurrentTMD[i]		= iniFile.GetString(_T("PG_TMD_INFO"), sKey,	 _T(""));	// ���� �� ��ȣ���� �������� ���� TMD file �̸�

		sKey.Format(_T("Checksum_%d"), i+1);
		m_strCurrentChecksum[i]	= iniFile.GetString(_T("PG_TMD_INFO"), sKey,	 _T(""));	// �� ��ȣ�� TMD ������ Checksum ����

		sKey.Format(_T("MD5_Result_%d"), i+1);
		m_strCurrentMD5[i]		= iniFile.GetString(_T("PG_TMD_INFO"), sKey,	_T(""));				// �� ��ȣ�� MD5 ó�� ���
	}

}


// ���ŵ� Job file download���� ������ �����Ѵ�.
void CConfigSystem::SaveTMDJobFileInfo()
{
	CString sSection;
	CString sKey;
	CString strPath = _T("D:\\Cheetah\\Runtime\\TMDDataList.ini");			// ���´������� S8F1�� ���Ź��� �� ����Ѵ�.
	CIni iniFile(strPath);	

	sSection = "TMDDownload";

	// Job file download ó�� ����
	iniFile.WriteLong(sSection,	_T("JobProcessSeq"),		m_nJobProcessSeq);			// ���� ������� �ܰ�
	iniFile.WriteLong(sSection,	_T("JobProcessRcv"),		m_nJobProcessRcv);			// ���� ��ȣ��� ���� ���Ź��� ����
	iniFile.WriteLong(sSection,	_T("JobProcessState"),		m_nJobProcessState);		// = 0,	// ���� �ƹ��� ���۵� ���� �ʾҴ�.

	// ���� ���� ���� ������
	iniFile.WriteString(sSection,	_T("ActionFlag"),	m_strActionFlag);		// ACTION FLAG // ���� ����		
			
	for (int i = 0; i < eMAX_JobFileCount; i++)
	{
		if ( i < m_nJobFileCount )					// ����� �����Ͱ� �����ϴ� �����̸�
		{
			sKey.Format(_T("Checksum2_%d"), i);
			iniFile.WriteString(sSection,	sKey,	m_strChecksum2[i]);		// ����  Checksum

			//sKey.Format(_T("FileName_%d"), i);
			//sKey.Format(_T("Checksum1_%d"), i);

			// ������ �Ǿ����� ó������ �ʾƼ� �о���� �ʴ� ����
			// sKey.Format("FileType_%d", i);
			// sKey.Format("FilePath_%d", i);
		}
	}//for i

	// ���� ��ȣ���� ����, ��� Check�� �� ������ ���� ���� ������
	for (int i = 0; i < eMAX_PGCount; i++)
	{
		sKey.Format(_T("TMD_Name_%d"), i+1);
		iniFile.WriteString(_T("PG_TMD_INFO"), sKey,	m_strCurrentTMD[i]);		// ���� �� ��ȣ���� �������� ���� TMD file �̸�

		sKey.Format(_T("Checksum_%d"), i+1);
		iniFile.WriteString(_T("PG_TMD_INFO"), sKey,	m_strCurrentChecksum[i]);	// �� ��ȣ�� TMD ������ Checksum ����

		sKey.Format(_T("MD5_Result_%d"), i+1);
		iniFile.WriteString(_T("PG_TMD_INFO"), sKey,	m_strCurrentMD5[i]);		// �� ��ȣ�� MD5 ó�� ���
	}

}



// ��ȣ��� ���� download file list�� ���ڿ��� ������ش�.
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

// MES File server�� ��� ����
void CConfigSystem::SetTMDServerPath(CString sPath)
{
	m_strServerPath = sPath;
}

// ���Ź��� File List�� ������ �����Ѵ�.
void CConfigSystem::SetTMDFileCount(int nCount)
{
	if ( nCount > eMAX_JobFileCount )
	{
		m_nJobFileCount = eMAX_JobFileCount;
	}
	else
		m_nJobFileCount = nCount;
}

// ���������� �����̸��� Checksum�� �����Ѵ�.
void CConfigSystem::SetTMDFileInfo(int nIdx, CString sFile, CString sCheck)
{
	if ( (nIdx >= 0) && (nIdx < eMAX_JobFileCount) )
	{
		m_strJobFilename1[nIdx] = sFile;
		m_strChecksum1[nIdx]		= sCheck;
	}

}

// ���������� ��ȣ��κ��� �о���� �����̸��� Checksum�� �����Ѵ�.
void CConfigSystem::SetTMDRcvFileInfo(int nIdx, CString sFile, CString sCheck)
{
	if ( (nIdx >= 0) && (nIdx < eMAX_JobFileCount) )
	{
		m_strJobFilename2[nIdx] = sFile;
		m_strChecksum2[nIdx]		= sCheck;
	}

}





// File server ���
	//AMT	/jobfile/AMT/HIST/	\\107.126.10.111\JOBFILEAMTHIST	/jobfile/AMT/HIST/
	//AMT	/jobfile/AMT/SPEC	\\107.126.10.111\JOBFILEAMTSPEC	/jobfile/AMT/SPEC
	//AMT	/jobfile/AMT/CURR	\\107.126.10.111\JOBFILEAMTCURR	/jobfile/AMT/CURR

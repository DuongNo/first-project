#include "StdAfx.h"
#include "CellBank.h"
#include "Etc\FileSupport.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

void CCellInfo::LinkCellLogItem()
{
	m_listCellLogItem.RemoveAll();

	CCellLogItem item;

	//////////////////////////////////////////////////////////////////////////
	// COMMON [11/16/2016 OSC]
	item = CCellLogItem(_T("INNER ID"),						CELL_LOG_DATA_STRING,		&m_strInnerID);																				m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("MODEL"),						CELL_LOG_DATA_STRING,		&theRecipeBank.m_strRecipeName);															m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("EDGE ALIGN"),					CELL_LOG_DATA_STRING,		&m_strEdgeResult);															                m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("CELL ID"),						CELL_LOG_DATA_STRING,		&m_strCellID);																				m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("READ UNIT"),					CELL_LOG_DATA_STRING,		&m_strReadUnitMCR);																			m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("PA_ALIGN SUCCESS"),				CELL_LOG_DATA_ALIGN_RESULT, &Pre_AlignResultXY);																		m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("JIG NAME"),						CELL_LOG_DATA_INT_ADD1,		&m_JigId);																					m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("CH NAME"),						CELL_LOG_DATA_INT_ADD1,		&m_JigCh);																					m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("1st Contact Result"),			CELL_LOG_DATA_STRING,		&m_strContactResult_1st);																	m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("2st Contact Result"),			CELL_LOG_DATA_STRING,		&m_strContactResult);																		m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("MC_Ver"),						CELL_LOG_DATA_INT,			NULL);																						m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("L_Ver"),						CELL_LOG_DATA_INT,			NULL);																						m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("U_Ver"),						CELL_LOG_DATA_INT,			NULL);																						m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("D_Ver"),						CELL_LOG_DATA_STRING,		&theProcBank.m_strDataPCVersion);															m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("UNIATS VERSION"),				CELL_LOG_DATA_STRING,		&m_strUNIATS_VERSION);																		m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("MICRO DLL VERSION"),			CELL_LOG_DATA_STRING,		&m_strMICRO_DLL_VERSION);																	m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("MACRO DLL VERSION"),			CELL_LOG_DATA_STRING,		&m_strMACRO_DLL_VERSION);																	m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("SETBIN VERSION"),				CELL_LOG_DATA_STRING,		&m_strSETBIN_VERSION);																		m_listCellLogItem.AddTail(item);
	//2017_05_23,JSJUNG UNLOADER OUT TACK TIME ->UNLOADER OUT TIME �̸�����(�Ｚ��û)
	item = CCellLogItem(_T("UNLOADER OUT TIME"),			CELL_LOG_DATA_TIME,			&cellTime.systmUnload_Out_Tack_Time);														m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("BIN2 CELL INFO"),				CELL_LOG_DATA_INT,			NULL);																						m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("PG_UI"),						CELL_LOG_DATA_STRING,		&m_strPG_UI_Version);																		m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("PROGRAM NAME"),					CELL_LOG_DATA_STRING,		&m_Zone_A.m_strTMD_Info);																	m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("CH Card MAC"),					CELL_LOG_DATA_STRING,		&m_strPG_MAC_Address);																		m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("JIG NO"),						CELL_LOG_DATA_INT_ADD1,		&m_JigId);																					m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("CIM_LOG"),						CELL_LOG_DATA_INT,			NULL);																						m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("CIM_MTP"),						CELL_LOG_DATA_INT,			NULL);																						m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("CIM_TSP"),						CELL_LOG_DATA_INT,			NULL);																						m_listCellLogItem.AddTail(item);
	//2017_05_23,JSJUNG CELL UNLOAD TACT ->TACT TIME ����(�������) �� ǥ��
	item = CCellLogItem(_T("TACT TIME"),					CELL_LOG_DATA_DOUBLE,		&UnLoadTact);																				m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("TABLE TURN TIME"),				CELL_LOG_DATA_TACT,			&cellTime.systmTableTurn_StartTime, &cellTime.systmTableTurn_EndTime);						m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("CELL LOAD TACT"),				CELL_LOG_DATA_DOUBLE,		&LoadTact);																					m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("CELL UNLOAD TACT"),				CELL_LOG_DATA_DOUBLE,		&UnLoadTact);																				m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("LAST CLASS"),					CELL_LOG_DATA_CLASS_CELL,	&m_LastClass);																				m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("LAST RESULT"),					CELL_LOG_DATA_STRING,		&m_LastResult);																				m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("PRINT DEFECT"),					CELL_LOG_DATA_STRING,		&PrintDefect);																				m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("PRINT CODE"),					CELL_LOG_DATA_STRING,		&PrintCode);																				m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("MES CODE"),						CELL_LOG_DATA_STRING,		&MesCode);																					m_listCellLogItem.AddTail(item);
	//////////////////////////////////////////////////////////////////////////
	// A Zone [11/16/2016 OSC]
	item = CCellLogItem(_T("AZONE START TIME"),				CELL_LOG_DATA_TIME,			&cellTime.systmA_StartTime);																m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("AZONE END TIME"),				CELL_LOG_DATA_TIME,			&cellTime.systmA_EndTime);																	m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("AZONE TACT TIME"),				CELL_LOG_DATA_TACT,			&cellTime.systmA_StartTime, &cellTime.systmA_EndTime);										m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("AZONE WAITING"),				CELL_LOG_DATA_TACT,			&cellTime.systmA_EndTime, &cellTime.systmA_WaitTime);										m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("AZONE CLASS"),					CELL_LOG_DATA_CLASS_CELL,	&m_Zone_A.AZoneClass);																		m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("AZONE DEFECT"),					CELL_LOG_DATA_STRING,		&m_Zone_A.AZoneDefect);																		m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("AZONE CONTACT START TIME"),		CELL_LOG_DATA_TIME,			&cellTime.systmA_ContactStartTime);															m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("AZONE CONTACT END TIME"),		CELL_LOG_DATA_TIME,			&cellTime.systmA_ContactEndTime);															m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("AZONE CONTACT TACT TIME"),		CELL_LOG_DATA_TACT,			&cellTime.systmA_ContactStartTime, &cellTime.systmA_ContactEndTime);						m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("AZONE CONTACT CLASS"),			CELL_LOG_DATA_CLASS_CELL,	&m_Zone_A.CellLoadingClass);																m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("AZONE Contact IBAT"),			CELL_LOG_DATA_STRING,		&m_Zone_A.m_strCellLoading_IBAT);															m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("AZONE Contact IVSS"),			CELL_LOG_DATA_STRING,		&m_Zone_A.m_strCellLoading_IVSS);															m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("AZONE Contact IVDD"),			CELL_LOG_DATA_STRING,		&m_Zone_A.m_strCellLoading_IVDD);															m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("AZONE Contact IVCI"),			CELL_LOG_DATA_STRING,		&m_Zone_A.m_strCellLoading_IVCI);															m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("AZONE Contact IBAT2"),			CELL_LOG_DATA_STRING,		&m_Zone_A.m_strCellLoading_IBAT2);															m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("AZONE Contact IDD2"),			CELL_LOG_DATA_STRING,		&m_Zone_A.m_strCellLoading_IDD2);															m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("AZONE CURRENT START TIME"),		CELL_LOG_DATA_TIME,			&cellTime.systmA_ContactStartTime);															m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("AZONE CURRENT END TIME"),		CELL_LOG_DATA_TIME,			&cellTime.systmA_ContactEndTime);															m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("AZONE CURRENT TACT TIME"),		CELL_LOG_DATA_TACT,			&cellTime.systmA_ContactStartTime, &cellTime.systmA_ContactEndTime);						m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("AZONE CURRENT CLASS"),			CELL_LOG_DATA_CLASS_CELL,	&m_Zone_A.ContactCurrentClass);																m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("AZONE IBAT"),					CELL_LOG_DATA_STRING,		&m_Zone_A.m_strContactCurrent_IBAT);														m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("AZONE IVSS"),					CELL_LOG_DATA_STRING,		&m_Zone_A.m_strContactCurrent_IVSS);														m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("AZONE IVDD"),					CELL_LOG_DATA_STRING,		&m_Zone_A.m_strContactCurrent_IVDD);														m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("AZONE IVCI"),					CELL_LOG_DATA_STRING,		&m_Zone_A.m_strContactCurrent_IVCI);														m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("AZONE IBAT2"),					CELL_LOG_DATA_STRING,		&m_Zone_A.m_strContactCurrent_IBAT2);														m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("AZONE IDD2"),					CELL_LOG_DATA_STRING,		&m_Zone_A.m_strContactCurrent_IDD2);														m_listCellLogItem.AddTail(item);
	//////////////////////////////////////////////////////////////////////////
	// B Zone [11/16/2016 OSC]
	item = CCellLogItem(_T("BZONE START TIME"),				CELL_LOG_DATA_TIME,			&cellTime.systmB_StartTime);																m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("BZONE END TIME"),				CELL_LOG_DATA_TIME,			&cellTime.systmB_EndTime);																	m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("BZONE TACT TIME"),				CELL_LOG_DATA_TACT,			&cellTime.systmB_StartTime, &cellTime.systmB_EndTime);										m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("BZONE WAITING"),				CELL_LOG_DATA_TACT,			&cellTime.systmB_EndTime, &cellTime.systmB_WaitTime);										m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("BZONE CLASS"),					CELL_LOG_DATA_CLASS_CELL,	&m_Zone_B.BZoneClass);																		m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("BZONE DEFECT"),					CELL_LOG_DATA_STRING,		&m_Zone_B.BZoneDefect);																		m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("BZONE ALIGN START TIME"),		CELL_LOG_DATA_TIME,			&cellTime.systmB_Align_Start_Time);															m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("BZONE ALIGN END TIME"),			CELL_LOG_DATA_TIME,			&cellTime.systmB_Align_End_Time);															m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("BZONE ALIGN TACT TIME"),		CELL_LOG_DATA_TACT,			&cellTime.systmB_Align_Start_Time, &cellTime.systmB_Align_End_Time);						m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("BZONE ALIGN DEGREE"),			CELL_LOG_DATA_DOUBLE,		&m_Zone_B.m_dVI_AlignTH);																	m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("BZONE ALIGN SUCCESS"),			CELL_LOG_DATA_ALIGN_RESULT,	&m_Zone_B.m_nVI_AlignResult);																m_listCellLogItem.AddTail(item);
	//////////////////////////////////////////////////////////////////////////
	// C Zone [11/16/2016 OSC]
	item = CCellLogItem(_T("CZONE START TIME"),				CELL_LOG_DATA_TIME,			&cellTime.systmC_StartTime);																m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("CZONE END TIME"),				CELL_LOG_DATA_TIME,			&cellTime.systmC_EndTime);																	m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("CZONE TACT TIME"),				CELL_LOG_DATA_TACT,			&cellTime.systmC_StartTime, &cellTime.systmC_EndTime);										m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("CZONE WAITING"),				CELL_LOG_DATA_TACT,			&cellTime.systmC_EndTime, &cellTime.systmC_WaitTime);										m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("CZONE CLASS"),					CELL_LOG_DATA_CLASS_CELL,	&m_Zone_C.CZoneClass);																		m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("CZONE DEFECT"),					CELL_LOG_DATA_STRING,		&m_Zone_C.CZoneDefect);																		m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("CZONE VISION START TIME"),		CELL_LOG_DATA_TIME,			&cellTime.systmC_VIStartTime);																m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("CZONE VISION END TIME"),		CELL_LOG_DATA_TIME,			&cellTime.systmC_VIEndTime);																m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("CZONE VISION TACT TIME"),		CELL_LOG_DATA_TACT,			&cellTime.systmC_VIStartTime, &cellTime.systmC_VIEndTime);									m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("CZONE VISION GRAB TIME"),		CELL_LOG_DATA_TACT,			&cellTime.systmC_VIGrabStartTime, &cellTime.systmC_VIGrabEndTime);							m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("CZONE VISION CLASS TIME"),		CELL_LOG_DATA_TACT,			&cellTime.systmC_VIClassStartTime, &cellTime.systmC_VIClassEndTime);						m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("CZONE VISION CLASS"),			CELL_LOG_DATA_CLASS_CELL,	&m_Zone_C.VIClass);																			m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("CZONE VISION DEFECT"),			CELL_LOG_DATA_STRING,		&m_Zone_C.VIDefect);																		m_listCellLogItem.AddTail(item);
	//////////////////////////////////////////////////////////////////////////
	// D Zone [11/16/2016 OSC]
	item = CCellLogItem(_T("DZONE START TIME"),				CELL_LOG_DATA_TIME,			&cellTime.systmD_StartTime);																m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("DZONE END TIME"),				CELL_LOG_DATA_TIME,			&cellTime.systmD_EndTime);																	m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("DZONE TACT TIME"),				CELL_LOG_DATA_TACT,			&cellTime.systmD_StartTime, &cellTime.systmD_EndTime);										m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("DZONE WAITING"),				CELL_LOG_DATA_TACT,			&cellTime.systmD_EndTime, &cellTime.systmD_WaitTime);										m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("DZONE CLASS"),					CELL_LOG_DATA_CLASS_CELL,	&m_Zone_D.DZoneClass);																		m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("DZONE DEFECT"),					CELL_LOG_DATA_STRING,		&m_Zone_D.DZoneDefect);																		m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("TRAY"),							CELL_LOG_DATA_INT,			NULL);																						m_listCellLogItem.AddTail(item);
}

CString CCellInfo::GetCellLogTitle()
{
	CString strBuff;
	strBuff.ReleaseBuffer();
	TCHAR * pBuf = strBuff.GetBuffer(MAX_CELL_LOG_STRING);
	memset(pBuf, NULL, MAX_CELL_LOG_STRING);
	int nWriteStart = 0;
	int nLength;

	CCellLogItem item;
	POSITION pos = m_listCellLogItem.GetHeadPosition();
	while(pos)
	{
		item = m_listCellLogItem.GetNext(pos);
		nLength = item.GetTitle().GetLength();
		if(nWriteStart + nLength > MAX_CELL_LOG_STRING)
			break;
		memcpy(pBuf+nWriteStart, item.GetTitle().GetString() , nLength * sizeof(TCHAR));
		nWriteStart += nLength;

		// �ڿ� �׸��� �� ������ ��ǥ�� �ٿ��ش� [11/16/2016 OSC]
		if(pos)
		{
			memcpy(pBuf+nWriteStart, _T(",") , 1 * sizeof(TCHAR));
			nWriteStart += 1;
		}
	}

	// �� �ѱ�� [11/16/2016 OSC]
	memcpy(pBuf+nWriteStart, _T("\n") , 1 * sizeof(TCHAR));
	strBuff.ReleaseBuffer();

	return strBuff;
}

CString CCellInfo::GetCellLogValue()
{
	CString strBuff;
	strBuff.ReleaseBuffer();
	TCHAR * pBuf = strBuff.GetBuffer(MAX_CELL_LOG_STRING);
	memset(pBuf, NULL, MAX_CELL_LOG_STRING);
	int nWriteStart = 0;
	int nLength;

	CCellLogItem item;
	POSITION pos = m_listCellLogItem.GetHeadPosition();
	while(pos)
	{
		item = m_listCellLogItem.GetNext(pos);
		nLength = item.ValueToString().GetLength();
		if(nWriteStart + nLength > MAX_CELL_LOG_STRING)
			break;
		memcpy(pBuf+nWriteStart, item.ValueToString().GetString() , nLength * sizeof(TCHAR));
		nWriteStart += nLength;

		// �ڿ� �׸��� �� ������ ��ǥ�� �ٿ��ش� [11/16/2016 OSC]
		if(pos)
		{
			memcpy(pBuf+nWriteStart, _T(",") , 1 * sizeof(TCHAR));
			nWriteStart += 1;
		}
	}

	// �� �ѱ�� [11/16/2016 OSC]
	memcpy(pBuf+nWriteStart, _T("\n") , 1 * sizeof(TCHAR));
	strBuff.ReleaseBuffer();

	return strBuff;
}

CCellBank::CCellBank(void)
{
	m_nNextGenIndex = 0;
}


CCellBank::~CCellBank(void)
{

}

CCellTag CCellBank::GetCellTag( CELL_POS pos )
{
	return m_CellTag[pos];
}

CCellTag CCellBank::GetCellTag(ZONE_ID zone, JIG_CH nCh )
{
	// A Zone���� ������� �Ǿ� ������ ä�� ����� ����ع����� �ȴ�
	int nTagIndex = CELL_POS_AZONE_CH1;
	nTagIndex += ((int)zone*JIG_CH_MAX) + (int)nCh;

	return m_CellTag[nTagIndex];
}

CCellTag CCellBank::SearchCellTag( JIG_ID nJig, JIG_CH nCh, CELL_POS startPos /*= CELL_POS_AZONE_CH1*/, CELL_POS endPos /*= CELL_POS_DZONE_CH5*/ )
{
	int nStart = (int)startPos;
	int nEnd = (int)endPos;
	CCellTag tag;
	CCellInfo *pCell;
	for(int i =nStart; i <= nEnd; i++ )
	{
		if(m_CellTag[i].IsExist())
		{
			pCell = GetCellInfo(m_CellTag[i]);
			if( (pCell->m_JigId == nJig) && (pCell->m_JigCh == nCh) )
			{
				tag = m_CellTag[i];
				break;
			}
		}
	}
	return tag;
}

CCellTag CCellBank::SearchCellTag( CString strInnerID, CELL_POS startPos /*= CELL_POS_AZONE_CH1*/, CELL_POS endPos /*= CELL_POS_DZONE_CH5*/ )
{
	int nStart = (int)startPos;
	int nEnd = (int)endPos;
	CCellTag tag;
	CCellInfo *pCell;
	for(int i =nStart; i <= nEnd; i++ )
	{
		if(m_CellTag[i].IsExist())
		{
			pCell = GetCellInfo(m_CellTag[i]);
			if(pCell->m_strInnerID == strInnerID)
			{
				tag = m_CellTag[i];
				break;
			}
		}
	}
	return tag;
}

void CCellBank::SetCellTag( CELL_POS pos, CCellTag tag )
{
	m_CellTag[pos] = tag;
}

void CCellBank::SetCellTag( ZONE_ID zone, JIG_CH nCh, CCellTag tag )
{
	int nTagIndex = CELL_POS_AZONE_CH1;
	nTagIndex += ((int)zone*JIG_CH_MAX) + (int)nCh;

	m_CellTag[nTagIndex] = tag;
}

CCellTag CCellBank::RemoveCellTag( CELL_POS pos )
{
	CCellInfo *pCell;
	CCellTag tag = m_CellTag[pos];
	
	//tray�� ���� �ű�� �ٸ� �� ������ �ʱ�ȭ �Ѵ�.
	if(CELL_POS_GOOD_TRAY == pos || CELL_POS_NG_TRAY == pos)
	{
		if(tag.IsExist())
		{
			pCell = GetCellInfo(pos);
			pCell->Init();
		}
	}
	m_CellTag[pos].Init();
	return tag;
}

//2017-07-15,SKCHO, ���� ����� �� ������ ������ ���
//���� �����Ͽ� �� �±׸� ���쳪 �� ���� ������ �ʱ�ȭ ���� �ʾ� ���� �±� �ε����� ���� Ʈ���� �÷��װ� �ʱ�ȭ �ȵǾ� Ʈ���� ���� �ʴ� ��� �߻�
CCellTag CCellBank::RemoveCellTagCellInfo( CELL_POS pos )
{
	CCellInfo *pCell;
	CCellTag tag = m_CellTag[pos];	
	if(tag.IsExist())
	{
		pCell = GetCellInfo(pos);
		pCell->Init();
	}
	m_CellTag[pos].Init();
	return tag;
}

//kjpark 20161106 CellRemove ����
CCellTag CCellBank::RemoveCellTag( ZONE_ID zone, JIG_CH nCh)
{
	int nTagIndex = CELL_POS_AZONE_CH1;
	nTagIndex += ((int)zone*JIG_CH_MAX) + (int)nCh;

	CCellTag tag =m_CellTag[nTagIndex];
	m_CellTag[nTagIndex].Init();
	
	return tag;
}

CCellTag CCellBank::CreateCellTag()
{
	/*CCellTag tag;
	tag.m_nIndex = m_nNextGenIndex;
	m_nNextGenIndex++;
	if(m_nNextGenIndex >= MAX_CELL_INFO_BUFFER)
		m_nNextGenIndex = 0;

	return tag;*/
	int nIndex = 0;
	BOOL bRe = FALSE;
	CCellTag tag;
	for(int i = CELL_POS_LD_CONV; i < CELL_POS_MAX; i++)
	{

		nIndex = m_CellTag[(CELL_POS)i].m_nIndex;
		if(nIndex == m_nNextGenIndex)
		{
			m_nNextGenIndex++;
			if(m_nNextGenIndex >= MAX_CELL_INFO_BUFFER)
		     m_nNextGenIndex = 0;

			i = CELL_POS_LD_CONV;
			bRe = TRUE;
		}

	}
	tag.m_nIndex = m_nNextGenIndex;
	if(bRe == FALSE)
	{
		m_nNextGenIndex++;
	}
	if(m_nNextGenIndex >= MAX_CELL_INFO_BUFFER)
		m_nNextGenIndex = 0;

	return tag;
}

void CCellBank::CreateCellInfo( CELL_POS pos )
{
	m_CellTag[pos] = CreateCellTag();
	//2017-07-17,SKCHO, TAG �����ϰ� CELL INFO �ʱ�ȭ
	GetCellInfo(m_CellTag[pos])->Init();
}

CCellInfo* CCellBank::GetCellInfo( CCellTag tag )
{
	// ���� Cell�̸� Tag�� �ʱ�ȭ �ؼ� �Ѱ��ش�
	return (tag.IsExist()) ? &m_Cell[tag.m_nIndex] : NULL;
}

CCellInfo* CCellBank::GetCellInfo(CELL_POS pos )
{
	return GetCellInfo(m_CellTag[pos]);
}

CCellInfo* CCellBank::GetCellInfo(ZONE_ID zone, JIG_CH nCh)
{
	return GetCellInfo(GetCellTag(zone, nCh));
}

CCellInfo* CCellBank::SearchCellInfo( JIG_ID nJig, JIG_CH nCh, CELL_POS startPos /*= CELL_POS_AZONE_CH1*/, CELL_POS endPos /*= CELL_POS_DZONE_CH5*/ )
{
	return GetCellInfo(SearchCellTag(nJig, nCh, startPos, endPos));	
}

CCellInfo* CCellBank::SearchCellInfo(int nJig, int nCh, CELL_POS startPos /*= CELL_POS_AZONE_CH1*/, CELL_POS endPos /*= CELL_POS_DZONE_CH5*/ )
{
	return SearchCellInfo((JIG_ID)nJig, (JIG_CH) nCh, startPos, endPos);
}

//kjpark 20161018 �� Tag shift
CELL_POS CCellBank::GetCellPosition(CCellTag tag)
{
	return (CELL_POS)tag.m_nIndex;
}

//kjpark 20161018 �� Tag shift
void CCellBank::SetCellTagNextPosition(CELL_POS CurrentPos, CELL_POS NextPos)
{
	SetCellTag(NextPos, RemoveCellTag(CurrentPos));
}

//kjpark 20161018 �� Tag shift
//kjpark 20161106 CellRemove ����
void CCellBank::SetCellTagNextPositionZone(ZONE_ID beforeZone, ZONE_ID nextZone)
{
#if 1
	CCellTag tag;
	for(int  i = 0 ; i < JIG_CH_MAX ; i++)
	{		
		tag = RemoveCellTag(beforeZone, (JIG_CH)i);
		SetCellTag(nextZone, (JIG_CH)i,tag);
	}
	
#else
	for(int  i = 0 ; i < JIG_CH_MAX ; i++)
	{
		CCellTag nextTag = GetCellTag(beforeZone, (JIG_CH)i);
		CCellTag beforTag = GetCellTag(nextZone, (JIG_CH)i);
		
		SetCellTag(GetCellPosition(nextTag), RemoveCellTag(GetCellPosition(GetCellTag(nextZone, (JIG_CH)i))));
	}
#endif
}

void CCellBank::WriteCellLog( CELL_POS pos )
{
	CCellInfo *pCell = GetCellInfo(pos);
	if(pCell == NULL) return;

	CTime time = CTime::GetCurrentTime();
	CEtc::ApplyTimeOffset(time, theConfigBank.m_Option.m_OffsetHour, 0);

	CString strDirPath;
	strDirPath.Format(_T("%s\\%02d\\CELL_DATA"), theConfigBank.m_System.m_strCellLogPath, time.GetMonth());

	if(CFileSupport::DirectoryCheck(strDirPath) == FALSE)
	{
		if(CFileSupport::CreateDirectory(strDirPath) == FALSE)
			return;
	}
	CString filepath;

	filepath.Format(_T("%s\\%d%02d%02d_CELL_%s.csv"	)	, strDirPath  
		, time.GetYear(), time.GetMonth(), time.GetDay() 
		, theConfigBank.m_System.m_strLineNo);

	char szLogTextbuf[MAX_CELL_LOG_STRING];
	CString strLogText;
	// ó�� �����ϴ� �Ÿ� Title������ ����ϱ� ���� ���ۿ� ��Ƶд� [11/16/2016 OSC]
	if(_taccess(filepath, 0) == -1)	// *csv�� ���� �����ϴ��� �Ǻ����� (0 : ���� -1 : ������);
	{
		strLogText = pCell->GetCellLogTitle();
	}

	// ���ϸ��� �ٲ����� ������ open�� ������ �ݰ� ���� open�Ѵ� [11/16/2016 OSC]
	if( (filepath != m_fileCellData.GetFilePath()) && (m_fileCellData.GetFilePath().GetLength() > 1) )
	{
		m_fileCellData.Close();
	}
	if(m_fileCellData.GetFilePath().GetLength() < 1)
	{
		if(m_fileCellData.Open(filepath, CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate | CFile::shareDenyWrite) == FALSE)  
		{
			return;
		}
	}

	// Title ��� [11/16/2016 OSC]
	if(strLogText.GetLength() > 0)
	{
		CEtc::ConvertMultibyte((wchar_t*)strLogText.GetString(), szLogTextbuf, MAX_CELL_LOG_STRING);
		m_fileCellData.SeekToEnd();
		m_fileCellData.Write(szLogTextbuf,strlen(szLogTextbuf));
	}

	// Cell Log ��� [11/16/2016 OSC]
	strLogText = pCell->GetCellLogValue();
	if(strLogText.GetLength() > 0)
	{
		CEtc::ConvertMultibyte((wchar_t*)strLogText.GetString(), szLogTextbuf, MAX_CELL_LOG_STRING);
		m_fileCellData.SeekToEnd();
		m_fileCellData.Write(szLogTextbuf,strlen(szLogTextbuf));
	}
	WriteCellLog_Temp(pos);
}


void CCellBank::WriteCellLog_Temp( CELL_POS pos )
{
	CCellInfo *pCell = GetCellInfo(pos);
	if(pCell == NULL) return;

	CTime time = CTime::GetCurrentTime();
	CEtc::ApplyTimeOffset(time, theConfigBank.m_Option.m_OffsetHour, 0);

	CString strDirPath;
	strDirPath.Format(_T("%s\\%02d\\CELL_DATA"), theConfigBank.m_System.m_strCellLogPath, time.GetMonth());

	if(CFileSupport::DirectoryCheck(strDirPath) == FALSE)
	{
		if(CFileSupport::CreateDirectory(strDirPath) == FALSE)
			return;
	}
	CString filepath;

	filepath.Format(_T("%s\\%d%02d%02d_CELL_%s_TEMP.csv"	)	, strDirPath  
		, time.GetYear(), time.GetMonth(), time.GetDay() 
		, theConfigBank.m_System.m_strLineNo);

	char szLogTextbuf[MAX_CELL_LOG_STRING];
	CString strLogText;
	// ó�� �����ϴ� �Ÿ� Title������ ����ϱ� ���� ���ۿ� ��Ƶд� [11/16/2016 OSC]
	if(_taccess(filepath, 0) == -1)	// *csv�� ���� �����ϴ��� �Ǻ����� (0 : ���� -1 : ������);
	{
		strLogText = pCell->GetCellLogTitle();
	}

	if( (filepath != m_fileCellData_Temp.GetFilePath()) && (m_fileCellData_Temp.GetFilePath().GetLength() > 1) )
	{
		return;
	}

//	if(m_fileCellData_Temp.GetFilePath().GetLength() < 1)
//	{
		if(m_fileCellData_Temp.Open(filepath, CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate | CFile::shareDenyWrite) == FALSE)  
		{
			return;
		}
//	}

	// Title ��� [11/16/2016 OSC]
	if(strLogText.GetLength() > 0)
	{
		CEtc::ConvertMultibyte((wchar_t*)strLogText.GetString(), szLogTextbuf, MAX_CELL_LOG_STRING);
		m_fileCellData_Temp.SeekToEnd();
		m_fileCellData_Temp.Write(szLogTextbuf,strlen(szLogTextbuf));
	}

	// Cell Log ��� [11/16/2016 OSC]
	strLogText = pCell->GetCellLogValue();
	if(strLogText.GetLength() > 0)
	{
		CEtc::ConvertMultibyte((wchar_t*)strLogText.GetString(), szLogTextbuf, MAX_CELL_LOG_STRING);
		m_fileCellData_Temp.SeekToEnd();
		m_fileCellData_Temp.Write(szLogTextbuf,strlen(szLogTextbuf));
	}

	m_fileCellData_Temp.Close();
}
/*void CCellBank::InitialCell_DataFile()
{
	// ���ο� ����� Cell Log ������ ����Ѵ�.
	CString sHeader;
	CString sFile;

	// �񰡵� Log�� ���� ���� ������ �Ѵ�.
	sHeader = _T("EQP ID,MODEL,ID,ALARM TYPE,START TIME,RESET TIME,END TIME,WAIT TIME,REPAIR TIME,DURATION,FIRST CLASS,SECOND CLASS,THIRD CLASS,TP SYSTEM LOSS NAME,TP SYSTEM LOSS CODE,ALARM CODE,REPAIR CODE");
	m_logCellData.SetHeader( sHeader );
	
	sFile.Format(_T("Cell_%s"), theConfigBank.m_System.m_strLineNo);						// ��: 20150413_LOSSTIME_Y01.csv
	m_logCellData.SetFilePath(theConfigBank.m_System.m_strCellLogPath, _T("CELL_DATA"), sFile);				// Data Log
	
	m_logCellData.m_bIsUnicode		= FALSE;				// MBCS�� �����Ѵ�.
	m_logCellData.m_bKeepOpen		= TRUE;					// ������ ���� ä�� ó���Ͽ� �ٸ��������� ������ ���´�.
	m_logCellData.m_bMirrorCreate	= TRUE; 				// �̷� ������ �����.
	m_logCellData.m_bIsHeaderPrint	= TRUE;																		// Header ���
	m_logCellData.m_bIsFolderCreate	= TRUE;
	m_logCellData.m_sExtention		= _T("csv");			// Ȯ���� ����
	m_logCellData.m_sMirrorSuffix	= _T("Temp");			// �̷��� ������ ���λ� ����
	m_logCellData.m_sDeliMiliSec	= _T(":");				// �и��ʴ� : ���� �������ش�.
	// 6, 2, 1 : D:\UB-AMT\Log\Sample\201509\20150904_LogTest.txt	-> Loss Time (�񰡵� �α�) ��Ͽ� ������
	m_logCellData.m_nDirNameMode	= CGxLog::eNAME_YearMonth;
	m_logCellData.m_nDirDatePos		= CGxLog::ePOS_DateLast;
	m_logCellData.m_nFileDatePos	= CGxLog::ePOS_DateFirst;

}*/

//2018-02-08 JSJUNG, TRACKOUT INFO ���� ����(������ å�� ��û)
void CCellBank::WriteTrackOutInfo( CELL_POS pos )
{
	CCellInfo *pCell = GetCellInfo(pos);
	if(pCell == NULL) return;

	CStdioFile file;
	CFileException e;

	CString strPath;
	strPath.Format(_T("D:\\Cheetah_CIM\\CIM_Config\\Setup.ini"));

	CIni ini(strPath);

	CString strEQID;
	strEQID	= ini.GetString(_T("EQUIP")	, _T("EQID")	, FALSE);

	
	CString strDirPath;
	strDirPath.Format(_T("D:\\auto-test\\TRACKOUT INFO"));

	if(CFileSupport::DirectoryCheck(strDirPath) == FALSE)
	{
		if(CFileSupport::CreateDirectory(strDirPath) == FALSE)
			return;
	}
	CString filepath;
	
	filepath.Format(_T("%s\\TRACKOUT_%s_%s.TXT"	)	, strDirPath  
		, pCell->m_strCellID, pCell->m_strInnerID);
		//, theConfigBank.m_System.m_strLineNo); m_Zone_C.CZoneDefect
	if ( file.Open(filepath.GetBuffer(), CFile::modeCreate|CFile::modeWrite, &e ) )
	{		
		CString szEQID;
		szEQID.Format(_T("EQP_ID=%s"), strEQID);
		CString szCELLID;
		szCELLID.Format(_T("CELL_ID=%s"), pCell->m_strCellID);
		CString szINNERID;
		szINNERID.Format(_T("INNER_ID=%s"), pCell->m_strInnerID);
		CString szDEFECTNAME;
		//2018-02-09 JSJUNG, TRACKOUT INFO ���� ������ GOOD�϶��� 0, NG�϶��� MESCODE����
		if(pCell->m_LastClass == GOOD_CELL)
		{
			szDEFECTNAME.Format(_T("DEFECTNAME=0"));
		}
		else
		{
			szDEFECTNAME.Format(_T("DEFECTNAME=%s"),pCell->MesCode);
		}
	
		file.SeekToEnd();
		
		file.WriteString( szEQID + "\n");
		file.WriteString( szCELLID + "\n");
		file.WriteString( szINNERID + "\n");
		file.WriteString( szDEFECTNAME + "\n");
	}
	file.Close();
}
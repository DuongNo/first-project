#include "StdAfx.h"
#include "CellLogItem.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CCellLogItem::CCellLogItem( void )
{
	Init();
}

CCellLogItem::CCellLogItem( CString strTitle, CELL_LOG_DATATYPE type, void *ptr, void *ptr2 /*= NULL*/ )
{
	SetTitle(strTitle);
	SetValuePtr(type, ptr, ptr2);
}

CCellLogItem::~CCellLogItem( void )
{

}

void CCellLogItem::Init()
{
	m_strTitle.Empty();
	InitValue();
}

void CCellLogItem::InitValue()
{
	m_ptrInt = NULL;
	m_ptrDouble = NULL;
	m_ptrString = NULL;
	m_ptrSystemTime1 = NULL;
	m_ptrSystemTime2 = NULL;
	m_Type = CELL_LOG_DATA_STRING;
}

void CCellLogItem::SetTitle( CString strTitle )
{
	m_strTitle = strTitle;
}

CString CCellLogItem::GetTitle()
{
	return m_strTitle;
}

void CCellLogItem::SetValuePtr( CELL_LOG_DATATYPE type, void *ptr, void *ptr2 /*= NULL*/ )
{
	InitValue();
	m_Type = type;
	switch(m_Type)
	{
	case CELL_LOG_DATA_INT:				
	case CELL_LOG_DATA_INT_ADD1:				
		m_ptrInt =			(int *)ptr;				break;
	case CELL_LOG_DATA_DOUBLE:			m_ptrDouble =		(double *)ptr;			break;
	case CELL_LOG_DATA_STRING:			m_ptrString =		(CString *)ptr;			break;
	case CELL_LOG_DATA_CLASS_RESULT:	m_ptrClassResult =	(CLASS_RESULT *)ptr;	break;
	case CELL_LOG_DATA_ALIGN_RESULT:	m_ptrAlignResult =	(ALIGN_RESULT *)ptr;	break;
	case CELL_LOG_DATA_CLASS_CELL:		m_ptrClassCell =	(CLASS_CELL *)ptr;		break;
	case CELL_LOG_DATA_TIME:			m_ptrSystemTime1 =	(SYSTEMTIME *)ptr;		break;
	case CELL_LOG_DATA_TACT:			
		m_ptrSystemTime1 =	(SYSTEMTIME *)ptr;		
		m_ptrSystemTime2 =	(SYSTEMTIME *)ptr2;		
		break;
	}
}

CString CCellLogItem::ValueToString()
{
	CString strRetrunText;
	switch(m_Type)
	{
	case CELL_LOG_DATA_INT:	
		if(m_ptrInt)	strRetrunText.Format(_T("%d"), *m_ptrInt);			
		break;
	case CELL_LOG_DATA_INT_ADD1:	
		if(m_ptrInt)	strRetrunText.Format(_T("%d"), *m_ptrInt + 1);			
		break;
	case CELL_LOG_DATA_DOUBLE:	
		if(m_ptrDouble)	strRetrunText.Format(_T("%.3f"), *m_ptrDouble);		
		break;
	case CELL_LOG_DATA_STRING:	
		if(m_ptrString)	strRetrunText.Format(_T("%s"), m_ptrString->GetString());		
		break;
	case CELL_LOG_DATA_CLASS_RESULT:	
		if(m_ptrClassResult)	strRetrunText.Format(_T("%s"), *m_ptrClassResult == RESULT_GOOD ? _T("GOOD"):_T("NG"));		
		break;
	case CELL_LOG_DATA_ALIGN_RESULT:	
		if(*m_ptrAlignResult == ALIGN_RESULT_GOOD)
			strRetrunText.Format(_T("GOOD"));		
		else if (*m_ptrAlignResult == ALIGN_RESULT_FAIL)
			strRetrunText.Format(_T("NG"));	
		else
			strRetrunText.Format(_T("SKIP"));	
		break;
	case CELL_LOG_DATA_CLASS_CELL:	
		// 2017.2.9 bgkim
		// AZone Defect¿Í AZone Class ½Ö ¸ÂÃã
//		if(m_ptrClassCell)	strRetrunText.Format(_T("%s"), *m_ptrClassCell == GOOD_CELL ? _T("0"):_T("1"));		
		if(m_ptrClassCell)	strRetrunText.Format(_T("%s"), *m_ptrClassCell == GOOD_CELL ? _T("1"):_T("0"));		
		break;
	case CELL_LOG_DATA_TIME:	
		if(m_ptrSystemTime1)	strRetrunText.Format(_T("%04u-%02u-%02u %02u:%02u:%02u:%03u")
												,m_ptrSystemTime1->wYear
												,m_ptrSystemTime1->wMonth
												,m_ptrSystemTime1->wDay
												,m_ptrSystemTime1->wHour
												,m_ptrSystemTime1->wMinute
												,m_ptrSystemTime1->wSecond
												,m_ptrSystemTime1->wMilliseconds);
		break;
	case CELL_LOG_DATA_TACT:	
		if(m_ptrSystemTime1 && m_ptrSystemTime2)
			strRetrunText.Format(_T("%.3f"), CountTactTime());	
		break;
	}

	return strRetrunText;
}

double CCellLogItem::CountTactTime()
{
	FILETIME fTm1,fTm2;
	ULONGLONG *ullVal1,*ullVal2;
	ULONGLONG ullDif;
	double dTactTime;

	SystemTimeToFileTime(m_ptrSystemTime1,&fTm1);	// start
	SystemTimeToFileTime(m_ptrSystemTime2,&fTm2);	// end
	ullVal1 = (ULONGLONG *)&fTm1;
	ullVal2 = (ULONGLONG *)&fTm2;
	ullDif = *ullVal2 - * ullVal1;

	dTactTime = double(ullDif/10000)/1000;

	// 1½Ã°£ limit
	if(dTactTime > 3600. || dTactTime < 0.)	dTactTime = 0.;

	return dTactTime;
}

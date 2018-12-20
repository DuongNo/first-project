////////////////////////////////////////////////////////////////////////////////
//
//	TGExcelManager.cpp : implementation of the TestGuarantee Function Class.
//
//  ESMLab, Inc. PROPRIETARY INFORMATION.
//  The following contains information proprietary to ESMLab, Inc. and may not be copied
//  nor disclosed except upon written agreement by ESMLab, Inc.
//
//  Copyright (C) 2012 ESMLab, Inc. All rights reserved.
//
// @author	Hongsu Jung (hongsu@esmlab.com)
// @Date	2012-04-04
//
////////////////////////////////////////////////////////////////////////////////


#include "StdAfx.h"
#include "TGExcelManager.h"
#ifdef _TG		//SMILE인경우만
	#include "TGSMILEFunc.h"
#endif
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define COLUMN_HEADING (_T("OSD IDX"))

CTGExcelManager::CTGExcelManager(void)
{
	m_ExcelApp  = NULL;
	m_WorkBook  = NULL;
	m_WorkBooks = NULL;

	m_bOpened = FALSE;

	//-- Initializes the COM library
	//-- 2012-05-14 hongsu
	::CoInitialize(NULL);
}

CTGExcelManager::~CTGExcelManager(void)
{
	Close();
	ERROR_INVALID_FUNCTION;
}


CTGExcelApplication*	CTGExcelManager::GetExcelApplication()
{
	//excelApp = &m_ExcelApp;
	return &m_ExcelApp;
}


//------------------------------------------------------------------------------ 
//! @brief    Initializes Excel automation
//! @date     2010-12-9
//! @owner    keunbae.song
//! @note     
//! @return        
//! @revision 2011-7-15 keunbae.song
//------------------------------------------------------------------------------ 
BOOL CTGExcelManager::Open(CString& strExcelFilePath, CString& strError)
{
	//-- Initializes the COM library
	//-- 2012-05-14 hongsu
	::CoInitialize(NULL);

	if(strExcelFilePath.IsEmpty())
	{
		strError.Format(_T("Failed to open excel file.(path is empty.)"));
		AfxMessageBox(strError);
		return FALSE;
	}

	if(!PathFileExists(strExcelFilePath))
	{
		strError.Format(_T("Failed to open excel file.(%s not found!)"), strExcelFilePath);
		AfxMessageBox(strError);
		return FALSE;
	}

	COleVariant covOptional(DISP_E_PARAMNOTFOUND, VT_ERROR);

	if(NULL == m_ExcelApp.m_lpDispatch) 
	{
		if(!m_ExcelApp.CreateDispatch(_T("Excel.Application")))
		{
			strError.Format(_T("Failed to create and IDispatch interface(Excel.Application)"));
			TRACE(_T("%s\n"),strError);
			return FALSE;
		}
	}

	//Returns a WorkBooks collection that represents all the open workbooks.
	try{
		m_WorkBooks = m_ExcelApp.get_Workbooks();
	}
	catch(COleException* ex)
	{
		CString str;
		str.Format(_T("Error: %s File Open Fail"), strExcelFilePath);
		::AfxMessageBox(str);
		m_ExcelApp.DetachDispatch();
		ex->Delete();
		return FALSE;
	}
	
	//Opens a workbook.
	try{
	m_WorkBook  = m_WorkBooks.Open(strExcelFilePath, covOptional, covOptional, covOptional,
		covOptional, covOptional, covOptional, covOptional, covOptional, covOptional, 
		covOptional, covOptional, covOptional);

	m_bOpened = TRUE;
	}
	catch(COleException* ex)
	{
		CString str;
		str.Format(_T("Error: %s File Open Fail"), strExcelFilePath);
		::AfxMessageBox(str);
		m_ExcelApp.DetachDispatch();
		ex->Delete();
		return FALSE;
	}

	return TRUE;
}

//------------------------------------------------------------------------------ 
//! @brief    Initializes Excel automation
//! @date     2014-07-31
//! @owner    cygil@esmlab.com
//! @note     
//! @return        
//------------------------------------------------------------------------------ 
BOOL CTGExcelManager::Open(CString& strExcelFilePath, CString& strError, BOOL bReadOnly)
{
	//-- Initializes the COM library
	//-- 2012-05-14 hongsu
	::CoInitialize(NULL);

	if(strExcelFilePath.IsEmpty())
	{
		strError.Format(_T("Failed to open excel file.(path is empty.)"));
		AfxMessageBox(strError);
		return FALSE;
	}

	if(!PathFileExists(strExcelFilePath))
	{
		strError.Format(_T("Failed to open excel file.(%s not found!)"), strExcelFilePath);
		AfxMessageBox(strError);
		return FALSE;
	}

	if(NULL == m_ExcelApp.m_lpDispatch) 
	{
		if(!m_ExcelApp.CreateDispatch(_T("Excel.Application")))
		{
			strError.Format(_T("Failed to create and IDispatch interface(Excel.Application)"));
			TRACE(_T("%s\n"),strError);
			return FALSE;
		}
	}

	//Returns a WorkBooks collection that represents all the open workbooks.
	try{
		m_WorkBooks = m_ExcelApp.get_Workbooks();
	}
	catch(COleException* ex)
	{
		CString str;
		str.Format(_T("Error: %s File Open Fail"), strExcelFilePath);
		::AfxMessageBox(str);
		m_ExcelApp.DetachDispatch();
		ex->Delete();
		return FALSE;
	}

	COleVariant covOptional(DISP_E_PARAMNOTFOUND, VT_ERROR);
	COleVariant covTrue( (short) TRUE );
	COleVariant covFalse( (short) FALSE );
	//Opens a workbook.
	try{
		if(bReadOnly)
			m_WorkBook  = m_WorkBooks.Open(strExcelFilePath, covOptional, covTrue, covOptional, covOptional, covOptional, covOptional, covOptional, covOptional, covOptional, covOptional, covOptional, covOptional);
		else
			m_WorkBook  = m_WorkBooks.Open(strExcelFilePath, covOptional, covOptional, covOptional, covOptional, covOptional, covOptional, covOptional, covOptional, covOptional, covOptional, covOptional, covOptional);
		m_bOpened = TRUE;
	}
	catch(COleException* ex)
	{
		CString str;
		str.Format(_T("Error: %s File Open Fail"), strExcelFilePath);
		::AfxMessageBox(str);
		m_ExcelApp.DetachDispatch();
		ex->Delete();
		return FALSE;
	}

	return TRUE;
}
#ifdef _TG		//SMILE인경우만
//------------------------------------------------------------------------------ 
//! @brief    Initializes Excel automation
//! @date     2014-05-28
//! @owner    cygil@esmlab.com
//! @note     
//! @return        
//! @revision 2014-05-28 cygil@esmlab.com
//------------------------------------------------------------------------------ 
BOOL CTGExcelManager::Open(CString& strExcelFilePath, BOOL bModalMsgBox)
{
	CString strError = _T("");
	if(strExcelFilePath.IsEmpty())
	{
		if(bModalMsgBox)
			TGCloseModalMessageBox();
		strError.Format(_T("Failed to open excel file.(path is empty.)"));
		AfxMessageBox(strError);
		return FALSE;
	}

	if(!PathFileExists(strExcelFilePath))
	{
		if(bModalMsgBox)
			TGCloseModalMessageBox();
		strError.Format(_T("Failed to open excel file.(%s not found!)"), strExcelFilePath);
		AfxMessageBox(strError);
		return FALSE;
	}

	COleVariant covOptional(DISP_E_PARAMNOTFOUND, VT_ERROR);

	if(NULL == m_ExcelApp.m_lpDispatch) 
	{
		if(!m_ExcelApp.CreateDispatch(_T("Excel.Application")))
		{
			if(bModalMsgBox)
				TGCloseModalMessageBox();
			strError.Format(_T("Failed to create and IDispatch interface(Excel.Application)"));
			TRACE(_T("%s\n"),strError);
			return FALSE;
		}
	}

	//Returns a WorkBooks collection that represents all the open workbooks.
	try{
		m_WorkBooks = m_ExcelApp.get_Workbooks();
	}
	catch(COleException* ex)
	{
		if(bModalMsgBox)
			TGCloseModalMessageBox();
		CString str;
		str.Format(_T("Error: %s File Open Fail"), strExcelFilePath);
		::AfxMessageBox(str);
		m_ExcelApp.DetachDispatch();
		ex->Delete();
		return FALSE;
	}
	
	//Opens a workbook.
	try{
	m_WorkBook  = m_WorkBooks.Open(strExcelFilePath, covOptional, covOptional, covOptional,
		covOptional, covOptional, covOptional, covOptional, covOptional, covOptional, 
		covOptional, covOptional, covOptional);

	m_bOpened = TRUE;
	}
	catch(COleException* ex)
	{
		if(bModalMsgBox)
			TGCloseModalMessageBox();
		CString str;
		str.Format(_T("Error: %s File Open Fail"), strExcelFilePath);
		::AfxMessageBox(str);
		m_ExcelApp.DetachDispatch();
		ex->Delete();
		return FALSE;
	}

	return TRUE;
}
#endif

BOOL CTGExcelManager::Close()
{
	if(!m_bOpened)
		return TRUE;

	if(m_WorkBook.m_lpDispatch)
	{
		COleVariant covOptional(DISP_E_PARAMNOTFOUND,VT_ERROR);
		m_WorkBook.Close(COleVariant((short)FALSE),covOptional,covOptional);	
		m_WorkBook = NULL;
	}

	if(m_WorkBooks.m_lpDispatch)
	{
		m_WorkBooks.Close();
		m_WorkBooks = NULL;
	}

	m_ExcelApp.Quit();
	m_ExcelApp.ReleaseDispatch();
	m_ExcelApp = NULL;

	//-- 2010-12-9 keunbae.song
	//-- Unloads any DLLs that are no longer in use
	//CoFreeUnusedLibraries();

	//-- Closes the COM library on the current thread, unloads all DLLs loaded by the thread
	::CoUninitialize();

	m_bOpened = FALSE;

	return TRUE;
}

//nSheetIndx : index of the specified sheet( 1-based)
BOOL CTGExcelManager::ReadLanguageTable(int nSheetIndex, TGMultiLanguageSheetVector& vecLanguageSheet)
{
	COleVariant covOptional(DISP_E_PARAMNOTFOUND, VT_ERROR);

	CTGExcelWorksheets Worksheets = m_WorkBook.get_Worksheets();
	CTGExcelWorksheet  WorkSheet  = Worksheets.get_Item(COleVariant((short)nSheetIndex));
	
	//엑셀 시트 A2셀 위치에 "OSD IDX" 컬럼 헤딩이 존재해야 함
	CTGExcelRange Range = WorkSheet.get_Range(COleVariant(_T("A2")), covOptional);
  CString strMenuText;
	strMenuText = (Range.get_Text().bstrVal);
	if(strMenuText.CompareNoCase(COLUMN_HEADING))
	{
		//AfxMessageBox("abnormal OSD Language sheet(\"OSD IDX\" not exist)");
		return FALSE;
	}

	//Get a CTGExcelRange object that represents the used range on the worksheet.
	CTGExcelRange UsedRange = WorkSheet.get_UsedRange();
	int nRowCount    = ((CTGExcelRange)UsedRange.get_EntireRow()).get_Count();
	
	CString strCell;
	CStringW strID, strValue;
	map<CStringW, CStringW> mapMenuStr;

	for(int nRow=3; nRow <= nRowCount; ++nRow)
	{
		CString strCell;
		CStringW strID;
		CStringW strValue;

		for(int nCol=_T('A'); nCol<=_T('D'); nCol++)
		{
			//A3,A4,A5,... or C3,C4,C5,...
			strCell.Format(_T("%c%d"), nCol, nRow);
			Range = WorkSheet.get_Range(COleVariant(strCell), covOptional);
			strID = Range.get_Text().bstrVal;
  
			++nCol;

			//B3,B4,B5,... or D3,D4,D5,...
			strCell.Format(_T("%c%d"), nCol, nRow);
			Range    = WorkSheet.get_Range(COleVariant(strCell), covOptional);
			strValue = Range.get_Text().bstrVal;

			//if strID isn't empty, insert new item to the map, 
			//strValue don't care.
			if(FALSE == strID.IsEmpty())
			{
				pair<CStringW, CStringW> newItem(strID, strValue);
				mapMenuStr.insert(newItem);
				//TRACE(_T("strID : %s, strValue : %s\n"), strID, strValue);
			}
		}
	}

	TGMultiLanguageSheet sheet;
	sheet.sheetname  = WorkSheet.get_Name();
	sheet.menustrmap = mapMenuStr;

	vecLanguageSheet.push_back(sheet);

	return TRUE;
}

//
BOOL CTGExcelManager::GetIndexBySheetName(CString strSheetName, int& nIndex)
{
	nIndex = -1;
	CTGExcelWorksheets oSheets;
	CTGExcelWorksheet oSheet;
	COleVariant covOptional(DISP_E_PARAMNOTFOUND, VT_ERROR);

	oSheets = m_WorkBook.get_Worksheets();
	int nSheetCount = (int)oSheets.get_Count();

	if(nSheetCount <= 0)
		return FALSE;

	//1-based
	for(int i=1; i<=nSheetCount; i++)
	{
		oSheet	= oSheets.get_Item(COleVariant((short)i));
		CString strName = oSheet.get_Name();
		if(0 == strName.CompareNoCase(strSheetName))
		{
			nIndex = i;
			break;
		}
	}

	if(nIndex <= 0)
		return FALSE;

	return TRUE;
}

BOOL CTGExcelManager::GetLanguageSheets(int& nNumofSheets, CStringArray& arstrLangNames)
{
	CTGExcelWorksheets oSheets;
	CTGExcelWorksheet oSheet;
	COleVariant covOptional(DISP_E_PARAMNOTFOUND, VT_ERROR);

	oSheets = m_WorkBook.get_Worksheets();
	int nSheetCount = (int)oSheets.get_Count();

	if(nSheetCount <= 0)
		return 0;

	short iSheetNo = 1;
	short iValidSheetCount = 0;
	while(nSheetCount > 0 )
	{		
		oSheet	= oSheets.get_Item(COleVariant((short)iSheetNo));
		int nRow = 2;
		CString strStartCell = _T("");
		CString strMenuText  = _T(""); //check this implies empty string or not
		CTGExcelRange oRange;
		while(strMenuText.IsEmpty() && nRow <= 6)
		{
			strStartCell.Empty();
			strStartCell.Format(_T("A%d"),nRow);
			oRange = oSheet.get_Range(COleVariant(strStartCell),covOptional);
			strMenuText = (oRange.get_Text().bstrVal/*oRange.get_Value().bstrVal*/);
			strMenuText.Trim();
			++nRow;
		}
		if(0 == strMenuText.CompareNoCase(COLUMN_HEADING))
		{
			arstrLangNames.Add(oSheet.get_Name());
			oSheet = oSheet.get_Next();
			++iValidSheetCount;
		}

		--nSheetCount;
		++iSheetNo;
	}

  nNumofSheets = iValidSheetCount;

	if(0 >= iValidSheetCount)
		return FALSE; 

	return TRUE;
}

//
//nSheetIndex : 1-based
//strCol      : "A", "B", "C", ...
//nIndex      : 1-based
//strVal      : Value string
BOOL CTGExcelManager::GetColValue(CString strSheetName, CString strCol, int nIndex, CString& strVal)
{
	int nSheetIndex = -1;
	if(FALSE == GetIndexBySheetName(strSheetName, nSheetIndex))
		return FALSE;

	CTGExcelWorksheets Worksheets = m_WorkBook.get_Worksheets();
	CTGExcelWorksheet  WorkSheet  = Worksheets.get_Item(COleVariant((short)nSheetIndex));

	//"A1", "A2",..., "B1", "B2", ..., "C1", ...
	CString strCell;
	strCell.Format(_T("%s%d"), strCol, nIndex); 
	
	COleVariant covOptional(DISP_E_PARAMNOTFOUND, VT_ERROR);
	CTGExcelRange Range;
  Range  = WorkSheet.get_Range(COleVariant(strCell), covOptional);
	strVal = Range.get_Text().bstrVal;

	return TRUE;
}

//nToIndex : -1이면, 
BOOL CTGExcelManager::GetColValues(CString strSheetName, CString strCol, int nFromIndex, CStringArray& arrStrValues)
{
	int nSheetIndex = -1;
	if(FALSE == GetIndexBySheetName(strSheetName, nSheetIndex))
		return FALSE;

	CTGExcelWorksheets Worksheets = m_WorkBook.get_Worksheets();
	CTGExcelWorksheet  WorkSheet  = Worksheets.get_Item(COleVariant((short)nSheetIndex));

	CString strCell;
	CTGExcelRange Range;
	COleVariant covOptional(DISP_E_PARAMNOTFOUND, VT_ERROR);
	CString strValue;
  
	//empty string이 얻어지면, break
	for(int i= nFromIndex; i <= USHRT_MAX; ++i) 
	{
		//"C3", "C4", "C5", "C6" 
		strCell.Format(_T("%s%d"), strCol, i); 
		Range = WorkSheet.get_Range(COleVariant(strCell), covOptional);
		strValue = Range.get_Text().bstrVal;
		if(strValue.IsEmpty())
			break;

    arrStrValues.Add(strValue);
	}

	return TRUE;
}

//------------------------------------------------------------------------------ 
//! @brief
//! @date     2011-11-18
//! @owner    keunbae.song
//! @note
//! @return        
//! @revision 
//------------------------------------------------------------------------------ 
BOOL CTGExcelManager::ClearSheet(int nSheetIndex)
{
	CTGExcelWorksheets Worksheets = m_WorkBook.get_Worksheets();
  int nSheetCount = Worksheets.get_Count();
	if(nSheetCount < 3)
		return FALSE;

	CTGExcelWorksheet WorkSheet = Worksheets.get_Item(COleVariant((short)nSheetIndex));
	CString str = WorkSheet.get_Name();

	CTGExcelRange UsedRange = WorkSheet.get_UsedRange();
	int nRowCount = ((CTGExcelRange)UsedRange.get_EntireRow()).get_Count();

  UsedRange.Clear();
  
	//WorkSheet.Delete();
	m_WorkBook.Save();

	return TRUE;
}

//------------------------------------------------------------------------------ 
//! @brief
//! @date     2011-11-18
//! @owner    keunbae.song
//! @note     nSheetIndex : 1-based.
//! @return        
//! @revision 
//------------------------------------------------------------------------------ 
BOOL CTGExcelManager::GetColumnIndex(int nSheetIndex, map<CString, int> & mapColumnIndex)
{
	CTGExcelWorksheets Worksheets = m_WorkBook.get_Worksheets();
	int nSheetCount = Worksheets.get_Count();


	CTGExcelWorksheet WorkSheet = Worksheets.get_Item(COleVariant((short)nSheetIndex));

	CTGExcelRange UsedRange = WorkSheet.get_UsedRange();
	int nColumnCount = ((CTGExcelRange)UsedRange.get_EntireColumn()).get_Count();

	
	COleVariant covOptional(DISP_E_PARAMNOTFOUND, VT_ERROR);

	int nCol = _T('A');
	for(int i=1; i<=nColumnCount; i++)
	{
		CString strTemp;
		strTemp.Format(_T("%c1"), nCol++); 

		CTGExcelRange Range = WorkSheet.get_Range(COleVariant(strTemp), covOptional);
		CString strColumn = Range.get_Text().bstrVal;
		
		pair<CString, int> newItem(strColumn, i-1);  //zero-based.
		mapColumnIndex.insert(newItem);
	}


	return TRUE;
}

//------------------------------------------------------------------------------ 
//! @brief    Determine whether opened or not.
//! @date     2011-11-23
//! @owner    keunbae.song
//! @note
//! @return        
//! @revision 
//------------------------------------------------------------------------------ 
BOOL CTGExcelManager::IsOpened()
{
	return m_bOpened;
}

//------------------------------------------------------------------------------ 
//! @brief    Get the count of sheets in excel file.
//! @date     2011-11-23
//! @owner    keunbae.song
//! @note     count : count of the sheets
//!           error : error string
//! @return        
//! @revision 
//------------------------------------------------------------------------------ 
BOOL CTGExcelManager::GetSheetsCount(int& count)
{
	count = 0;
	if(!IsOpened())
		return FALSE;

	CTGExcelWorksheets oSheets;
	CTGExcelWorksheet oSheet;
	COleVariant covOptional(DISP_E_PARAMNOTFOUND, VT_ERROR);

	oSheets = m_WorkBook.get_Worksheets();
	count = (int)oSheets.get_Count();

	return TRUE;
}

//------------------------------------------------------------------------------ 
//! @brief    Get the sheet's index of the specified name.
//! @date     2011-11-25
//! @owner    keunbae.song
//! @note     sheetname : name of the specified sheet
//!           index     : sheet index(1-based)
//! @return        
//! @revision 
//------------------------------------------------------------------------------ 
BOOL CTGExcelManager::GetSheetIndex(wstring sheetname, int& index)
{
	index = 0;

	CTGExcelWorksheets sheets = m_WorkBook.get_Worksheets();

	int count;
	GetSheetsCount(count);

	for(int i=1; i<=count; i++)
	{
		CTGExcelWorksheet sheet = sheets.get_Item(COleVariant((short)i));
		wstring name = sheet.get_Name();
		if(name == sheetname)
		{
			index = i;
			break;
		}
	}

	if(0 == index)
		return FALSE;

	return TRUE;
}


//------------------------------------------------------------------------------ 
//! @brief    Get the name of the specified sheet index.
//! @date     2011-11-23
//! @owner    keunbae.song
//! @note     index : index of the specified sheet(1-based).
//!           name  : sheetname
//!           error : error string
//! @return        
//! @revision 
//------------------------------------------------------------------------------ 
BOOL CTGExcelManager::GetSheetName(int index, wstring& name)
{
	name  = _T("");
	//error = _T("");

	if(!IsOpened())
		return FALSE;

	int count = 0;
	if(!GetSheetsCount(count))
		return FALSE;

	if((count < index) || (0 == index))
		return FALSE;

	COleVariant covOptional(DISP_E_PARAMNOTFOUND, VT_ERROR);

	CTGExcelWorksheets oSheets = m_WorkBook.get_Worksheets();
	CTGExcelWorksheet oSheet	= oSheets.get_Item(COleVariant((short)index));
	name = oSheet.get_Name();

	return TRUE;
}
//------------------------------------------------------------------------------
/// <pre>
/// @brief
/// @date        2013-05-22
/// @owner       Ryumin (ryumin@esmlab.com)
/// @attention   
/// @note        
/// @revision    
/// @date        Writer      Comment
/// 
/// \Param       index
/// \Param       name
/// @return      BOOL 
/// </pre>
//------------------------------------------------------------------------------
BOOL CTGExcelManager::SetSheetName(int index, wstring name)
{
	if ( name.empty() ) return FALSE;

	if(!IsOpened())
		return FALSE;

	int count = 0;
	if(!GetSheetsCount(count))
		return FALSE;

	if((count < index) || (0 == index))
		return FALSE;

	COleVariant covOptional(DISP_E_PARAMNOTFOUND, VT_ERROR);

	CTGExcelWorksheets oSheets	= m_WorkBook.get_Worksheets();
	CTGExcelWorksheet oSheet	= oSheets.get_Item(COleVariant((short)index));
	oSheet.put_Name( name.c_str() );

	return TRUE;
}

//------------------------------------------------------------------------------
//! @brief    Add Sheet
//! @note     
//! @return        
//! @revision 
//------------------------------------------------------------------------------
BOOL CTGExcelManager::AddSheet()
{

	COleVariant VOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
	CTGExcelWorksheets Worksheets = m_WorkBook.get_Worksheets();

	VARIANT     varValue;
	COleVariant varCount(Worksheets.get_Count());
	V_VT(&varValue)       = VT_DISPATCH;
	V_DISPATCH(&varValue) = Worksheets.get_Item(varCount);

	Worksheets.Add(VOptional, varValue, VOptional, VOptional);
	return TRUE;
}

//------------------------------------------------------------------------------ 
//! @brief    Get the count of the columns in the specified sheet.
//! @date     2011-11-23
//! @owner    keunbae.song
//! @note     
//! @return        
//! @revision 
//------------------------------------------------------------------------------ 
BOOL CTGExcelManager::GetColumnCount(int index, int& count)
{
	CTGExcelWorksheets Worksheets = m_WorkBook.get_Worksheets();
	CTGExcelWorksheet WorkSheet = Worksheets.get_Item(COleVariant((short)index));

	CTGExcelRange UsedRange = WorkSheet.get_UsedRange();
	count = ((CTGExcelRange)UsedRange.get_EntireColumn()).get_Count();

	return TRUE;
}

//------------------------------------------------------------------------------ 
//! @brief    Set the value in the specified cell.
//! @date     2011-11-24
//! @owner    keunbae.song
//! @note     sheetname : name of the specified worksheet
//!           colindex  : "A", "B", "C", ....
//!           rowindex  : 1,2,3,4,....
//!           cellvalue : setting text value int the specified cell
//! @return        
//! @revision 
//------------------------------------------------------------------------------ 
BOOL CTGExcelManager::SetCellValue(wstring sheetname, wstring colindex, int rowindex,  wstring cellvalue)
{
	int index = 0;
	if(!GetIndexBySheetName(CString(sheetname.c_str()), index))
		return FALSE;

	CTGExcelWorksheets worksheets = m_WorkBook.get_Worksheets();
	CTGExcelWorksheet  worksheet  = worksheets.get_Item(COleVariant((short)index));

	//"A1","A2",...,"B1","B2",...,"C1",...
	wostringstream cell;
	cell << colindex.c_str() << rowindex;

	CTGExcelRange Range;
	//CString strCell;
	//strCell.Format(_T("%s%d"), colindex.c_str(), rowindex);
	Range  = worksheet.get_Range(COleVariant(cell.str().c_str()), COleVariant(cell.str().c_str()));
	Range.put_Value(COleVariant(cellvalue.c_str()));
	//Range = worksheet.get_Range(COleVariant(strCell), COleVariant(strCell));
	//Range.put_Value(COleVariant(cellvalue.c_str()));

	return TRUE;
}

//------------------------------------------------------------------------------ 
//! @brief    Set Cell Color
//! @return        
//! @revision 
//------------------------------------------------------------------------------ 
BOOL CTGExcelManager::SetCellColor(wstring sheetname, wstring colindex, int rowindex,  COLORREF color)
{
	CTGExcelInterior interior;

	int index = 0;
	if(!GetIndexBySheetName(CString(sheetname.c_str()), index))
		return FALSE;

	CTGExcelWorksheets worksheets = m_WorkBook.get_Worksheets();
	CTGExcelWorksheet  worksheet  = worksheets.get_Item(COleVariant((short)index));

	wostringstream cell;
	cell << colindex.c_str() << rowindex;

	CTGExcelRange Range;
	try
	{
		Range  = worksheet.get_Range(COleVariant(cell.str().c_str()), COleVariant(cell.str().c_str()));
	}
	catch (...)
	{
		return FALSE;
	}
	interior =  Range.get_Interior();  // 해당셀 Interior 를 가져오고...
	interior.put_Color(color, TRUE);

	return TRUE;
}

//------------------------------------------------------------------------------ 
//! @brief    Set Cell Color
//! @return        
//! @revision 
//------------------------------------------------------------------------------ 
BOOL CTGExcelManager::SetRangeColor(wstring sheetname, wstring startindex, int nStartindex, wstring endindex, int nEndindex, COLORREF color)
{
	CTGExcelInterior interior;

	int index = 0;
	if(!GetIndexBySheetName(CString(sheetname.c_str()), index))
		return FALSE;

	CTGExcelWorksheets worksheets = m_WorkBook.get_Worksheets();
	CTGExcelWorksheet  worksheet  = worksheets.get_Item(COleVariant((short)index));

	wostringstream startCell;
	startCell << startindex.c_str() << nStartindex;

	wostringstream endCell;
	endCell << endindex.c_str() << nEndindex;

	CTGExcelRange Range;
	Range  = worksheet.get_Range(COleVariant(startCell.str().c_str()), COleVariant(endCell.str().c_str()));

	interior = Range.get_Interior();  // 해당셀 Interior 를 가져오고...
	interior.put_Color(color, TRUE);

	return TRUE;
}


//------------------------------------------------------------------------------ 
//! @brief    Set Cell Color
//! @return        
//! @revision 
//------------------------------------------------------------------------------ 
BOOL CTGExcelManager::SetCellFontColor(wstring sheetname, wstring colindex, int rowindex,  COLORREF color)
{
	CTGExcelFont exelFont;

	int index = 0;
	if(!GetIndexBySheetName(CString(sheetname.c_str()), index))
		return FALSE;

	CTGExcelWorksheets worksheets = m_WorkBook.get_Worksheets();
	CTGExcelWorksheet  worksheet  = worksheets.get_Item(COleVariant((short)index));

	wostringstream cell;
	cell << colindex.c_str() << rowindex;

	CTGExcelRange Range;
	Range  = worksheet.get_Range(COleVariant(cell.str().c_str()), COleVariant(cell.str().c_str()));

	exelFont =  Range.get_Font();  // 해당셀 Font 를 가져오고...
	exelFont.put_Color(color);

	return TRUE;
}

BOOL CTGExcelManager::SetCellFont(wstring sheetname, wstring colindex, int rowindex, wstring fontname, int nsize, COLORREF color, BOOL bBold, BOOL bItalic)
{
	CTGExcelFont exelFont;

	int index = 0;
	if(!GetIndexBySheetName(CString(sheetname.c_str()), index))
		return FALSE;

	CTGExcelWorksheets worksheets = m_WorkBook.get_Worksheets();
	CTGExcelWorksheet  worksheet  = worksheets.get_Item(COleVariant((short)index));

	wostringstream cell;
	cell << colindex.c_str() << rowindex;

	CTGExcelRange Range;
	Range  = worksheet.get_Range(COleVariant(cell.str().c_str()), COleVariant(cell.str().c_str()));

	exelFont =  Range.get_Font();  // 해당셀 Font 를 가져오고...
	exelFont.put_Font(COleVariant(fontname.c_str()), nsize, color, bBold, bItalic);

	return TRUE;
}

//------------------------------------------------------------------------------ 
//! @brief
//! @date     2011-11-25
//! @owner    keunbae.song
//! @note
//! @return        
//! @revision 
//------------------------------------------------------------------------------ 
BOOL CTGExcelManager::GetCellValue(wstring sheetname, wstring colindex, int rowindex, wstring& cellvalue)
{
	int index = 0;
	GetSheetIndex(sheetname, index);

	CTGExcelWorksheets worksheets = m_WorkBook.get_Worksheets();
	CTGExcelWorksheet  worksheet  = worksheets.get_Item(COleVariant((short)index));

	//"A1","A2",...,"B1","B2",...,"C1",...
	wostringstream cell;
	cell << colindex.c_str() << rowindex;

	CTGExcelRange Range = worksheet.get_Range(COleVariant(cell.str().c_str()), COleVariant(cell.str().c_str()));
	cellvalue = Range.get_Text().bstrVal;

	return TRUE;
}

//------------------------------------------------------------------------------ 
//! @brief
//! @date     2011-11-25
//! @owner    keunbae.song
//! @note     sheetname : name of the specified worksheet
//!           colindex  : "A", "B", "C", ....
//!           colvalues : all the values of the specified column.
//! @return        
//! @revision 
//------------------------------------------------------------------------------ 
BOOL CTGExcelManager::GetColumnValues(wstring sheetname, wstring colindex, vector<wstring>& colvalues)
{	
	int index = 0;
	GetSheetIndex(sheetname, index);

	CTGExcelWorksheets Worksheets = m_WorkBook.get_Worksheets();
	CTGExcelWorksheet  WorkSheet  = Worksheets.get_Item(COleVariant((short)index));

	CString strCell;
	CTGExcelRange Range;
	COleVariant covOptional(DISP_E_PARAMNOTFOUND, VT_ERROR);
	CString strValue;

	//Get all the cell values of the same column.
	for(int i=1; i<=USHRT_MAX; ++i) 
	{
		wstring cellvalue = _T("");
		GetCellValue(sheetname, colindex, i, cellvalue);

		//if cellvalue is empty.
		if(cellvalue.empty())
			break;

		colvalues.push_back(cellvalue);
	}

	return TRUE;
}

//------------------------------------------------------------------------------ 
//! @brief
//! @date     2011-11-29
//! @owner    keunbae.song
//! @note     sheetname : name of the specified worksheet
//!           colindex  : "A", "B", "C", ....
//!           colvalues : all the values of the specified column.
//! @return        
//! @revision 
//------------------------------------------------------------------------------ 
BOOL CTGExcelManager::GetColumnValues(CString sheetname, CString colindex, vector<CString>& colvalues)
{
	int index = 0;
	GetSheetIndex(wstring(sheetname), index);

	CTGExcelWorksheets Worksheets = m_WorkBook.get_Worksheets();
	CTGExcelWorksheet  WorkSheet  = Worksheets.get_Item(COleVariant((short)index));

	CString strCell;
	CTGExcelRange Range;
	COleVariant covOptional(DISP_E_PARAMNOTFOUND, VT_ERROR);
	CString strValue;

	//Get all the cell values of the same column.
	for(int i=1; i<=UINT_MAX; ++i) 
	{
		wstring cellvalue = _T("");
		GetCellValue(wstring(sheetname), wstring(colindex), i, cellvalue);
																																									
		//if cellvalue is empty.
		if(cellvalue.empty())
			break;

		if(i==UINT_MAX)
			break;

		//colvalues.push_back(cellvalue);
		colvalues.push_back(cellvalue.c_str());
	}

	return TRUE;
}

//------------------------------------------------------------------------------ 
//! @brief
//! @date     2012-1-2
//! @owner    jeongyong.kim
//! @note     sheetname : name of the specified worksheet
//!           rowindex  : "1", "2", "3", ....
//!           rowvalues : all the values of the specified row.
//! @return        
//! @revision 
//------------------------------------------------------------------------------ 
BOOL CTGExcelManager::GetRowValues(wstring sheetname, int rowindex, vector<CString>& rowvalues, wchar_t chColStart)
{

	int index = 0;
	if(!GetIndexBySheetName(CString(sheetname.c_str()), index))
		return FALSE;

	CTGExcelWorksheets worksheets = m_WorkBook.get_Worksheets();
	CTGExcelWorksheet  worksheet  = worksheets.get_Item(COleVariant((short)index));

	wostringstream startCell;
	startCell << chColStart << rowindex;

	CTGExcelRange Range = worksheet.get_Range(COleVariant(startCell.str().c_str()), COleVariant(startCell.str().c_str()));
	Range = Range.get_Resize(COleVariant((short)1), COleVariant((short)52));	// 'A' ~ 'AZ'
	CTGExcelRange cells = Range.get_Cells();
	COleSafeArray saRet(Range.get_Value());

	long iRows;
    long iCols;
    saRet.GetUBound(1, &iRows);
    saRet.GetUBound(2, &iCols);

	long lCellIdx[2];
	// Loop through the data and report the contents.
	lCellIdx[0]=1;
	for (int nColCnt = 1; nColCnt <= iCols; nColCnt++)
	{
		lCellIdx[1]=nColCnt;
		COleVariant vData;
		vData.Clear();
		saRet.GetElement(lCellIdx, vData);
		if(vData.vt != VT_BSTR && vData.vt != VT_EMPTY)
		{
			vData.Clear();
			CTGExcelRange cell = cells.get_Item(COleVariant(lCellIdx[0]), COleVariant(lCellIdx[1])).pdispVal;
			vData = cell.get_Text();
		}
		CString strData(vData);
		if(strData.IsEmpty())
			break;
		rowvalues.push_back(strData);
	}
	return TRUE;
}
//------------------------------------------------------------------------------ 
//! @brief
//! @date     2012-07-12
//! @owner    keunbae.song
//! @note     sheetname : name of the specified worksheet
//!           rowindex  : "1", "2", "3", ....
//!           colnum    : the number of the columns to read.
//!           rowvalues : all the values of the specified row.
//! @return        
//! @revision 
//! @modify  2014-05-22 cygil@esmlab.com  range get values
//------------------------------------------------------------------------------ 
BOOL CTGExcelManager::GetRowValues(wstring sheetname, int rowindex, int colcount, vector<CString>& rowvalues, wchar_t chColStart)
{
	int index = 0;
	if(!GetIndexBySheetName(CString(sheetname.c_str()), index))
		return FALSE;

	CTGExcelWorksheets worksheets = m_WorkBook.get_Worksheets();
	CTGExcelWorksheet  worksheet  = worksheets.get_Item(COleVariant((short)index));

	wostringstream startCell;
	startCell << chColStart << rowindex;

	CTGExcelRange Range = worksheet.get_Range(COleVariant(startCell.str().c_str()), COleVariant(startCell.str().c_str()));
	Range = Range.get_Resize(COleVariant((short)1), COleVariant((short)colcount));
	CTGExcelRange cells = Range.get_Cells();
	COleSafeArray saRet(Range.get_Value());

	long iRows;
    long iCols;
    saRet.GetUBound(1, &iRows);
    saRet.GetUBound(2, &iCols);

	long lCellIdx[2];
	// Loop through the data and report the contents.
	lCellIdx[0] = 1;
	for (int nColCnt = 1; nColCnt <= iCols; nColCnt++)
	{
		lCellIdx[1]=nColCnt;		   
		COleVariant vData;
		vData.Clear();
		saRet.GetElement(lCellIdx, vData);
		if(vData.vt != VT_BSTR && vData.vt != VT_EMPTY)
		{
			vData.Clear();
			CTGExcelRange cell = cells.get_Item(COleVariant(lCellIdx[0]), COleVariant(lCellIdx[1])).pdispVal;
			vData = cell.get_Text();
		}
		rowvalues.push_back(CString(vData));
	}
	return TRUE;
}

//------------------------------------------------------------------------------ 
//! @brief
//! @date     2014-05-22
//! @owner    cygil@esmlab.com
//! @note     sheetname   : name of the specified worksheet
//!           chColStart  : "A", "B", "C", ... base column index
//!           colcount    : the number of the columns to read.
//!           rowindex    : "1", "2", "3", ....base row index
//!           rowcount    : the number of the rows to read.
//!           tablevalues : all the values of the specified row.
//! @return        
//! @revision 
//! @modify  2014-05-22 cygil@esmlab.com  range get values
//------------------------------------------------------------------------------ 
BOOL CTGExcelManager::GetTableValues(wstring sheetname, int rowindex, int rowcount, vector<vector<CString>>& tablevalues, wchar_t chColStart)
{
	int index = 0;
	if(!GetIndexBySheetName(CString(sheetname.c_str()), index) || rowcount < 1)
		return FALSE;

	CTGExcelWorksheets worksheets = m_WorkBook.get_Worksheets();
	CTGExcelWorksheet  worksheet  = worksheets.get_Item(COleVariant((short)index));

	wostringstream startCell;
	startCell << chColStart << rowindex;

	CTGExcelRange Range = worksheet.get_Range(COleVariant(startCell.str().c_str()), COleVariant(startCell.str().c_str()));
	Range = Range.get_Resize(COleVariant((LONGLONG)rowcount), COleVariant((short)26));	// 'A' ~ 'Z'
	CTGExcelRange cells = Range.get_Cells();
	COleSafeArray saRet(Range.get_Value());

	long iRows;
    long iCols;
    saRet.GetUBound(1, &iRows);
    saRet.GetUBound(2, &iCols);

	long lCellIdx[2];
	// Loop through the data and report the contents.
	for (int nRowCnt = 1; nRowCnt <= iRows; nRowCnt++)
	{
		vector<CString> rowvalues;
		rowvalues.clear();
		for (int nColCnt = 1; nColCnt <= iCols; nColCnt++)
		{
			lCellIdx[0]=nRowCnt;
			lCellIdx[1]=nColCnt;
			COleVariant vData;
			vData.Clear();
			saRet.GetElement(lCellIdx, vData);
			if(vData.vt == VT_EMPTY)
				break;
			if(vData.vt != VT_BSTR)
			{
				vData.Clear();
				CTGExcelRange cell = cells.get_Item(COleVariant(lCellIdx[0]), COleVariant(lCellIdx[1])).pdispVal;
				vData = cell.get_Text();
			}
			rowvalues.push_back(CString(vData));
		}
		tablevalues.push_back(rowvalues);
	}
	return TRUE;
}


//------------------------------------------------------------------------------ 
//! @brief
//! @date     2014-05-22
//! @owner    cygil@esmlab.com
//! @note     sheetname   : name of the specified worksheet
//!           chColStart  : "A", "B", "C", ... base column index
//!           colcount    : the number of the columns to read.
//!           rowindex    : "1", "2", "3", ....base row index
//!           rowcount    : the number of the rows to read.
//!           tablevalues : all the values of the specified row.
//! @return        
//! @revision 
//! @modify  2014-05-22 cygil@esmlab.com  range get values
//------------------------------------------------------------------------------ 
BOOL CTGExcelManager::GetTableValues(wstring sheetname, int colcount, int rowindex, int rowcount, vector<vector<CString>>& tablevalues, wchar_t chColStart)
{
	int index = 0;
	if(!GetIndexBySheetName(CString(sheetname.c_str()), index) || rowcount < 1)
		return FALSE;

	CTGExcelWorksheets worksheets = m_WorkBook.get_Worksheets();
	CTGExcelWorksheet  worksheet  = worksheets.get_Item(COleVariant((short)index));

	wostringstream startCell;
	startCell << chColStart << rowindex;

	CTGExcelRange Range = worksheet.get_Range(COleVariant(startCell.str().c_str()), COleVariant(startCell.str().c_str()));
	Range = Range.get_Resize(COleVariant((LONGLONG)rowcount), COleVariant((LONGLONG)colcount));
	CTGExcelRange cells = Range.get_Cells();
	COleSafeArray saRet(Range.get_Value());

	long iRows;
    long iCols;
    saRet.GetUBound(1, &iRows);
    saRet.GetUBound(2, &iCols);

	long lCellIdx[2];
	// Loop through the data and report the contents.
	for (int nRowCnt = 1; nRowCnt <= iRows; nRowCnt++)
	{
		vector<CString> rowvalues;
		rowvalues.clear();
		for (int nColCnt = 1; nColCnt <= iCols; nColCnt++)
		{
			lCellIdx[0]=nRowCnt;
			lCellIdx[1]=nColCnt;
			COleVariant vData;
			vData.Clear();
			saRet.GetElement(lCellIdx, vData);
			if(vData.vt != VT_BSTR && vData.vt != VT_EMPTY)
			{
				vData.Clear();
				CTGExcelRange cell = cells.get_Item(COleVariant(lCellIdx[0]), COleVariant(lCellIdx[1])).pdispVal;
				vData = cell.get_Text();
			}
			rowvalues.push_back(CString(vData));
		}
		tablevalues.push_back(rowvalues);
	}
	return TRUE;
}

//------------------------------------------------------------------------------ 
//! @brief
//! @date     2014-05-22
//! @owner    cygil@esmlab.com
//! @note     sheetname : name of the specified worksheet
//!           startindex  : startColIndex 'A', 'B', 'C', ...
//!           endindex  : endColIndex 'A', 'B', 'C', ...
//!           nRowindex	: 1, 2, 3, 4, 5, ...
//!           rowvalues : all the values of the specified row.
//! @return        
//! @revision 
//------------------------------------------------------------------------------ 
BOOL CTGExcelManager::SetRowValues(wstring sheetname, int nRowindex, vector<CString> rowvalues, wchar_t chColStart)
{
	int index = 0;
	if(!GetIndexBySheetName(CString(sheetname.c_str()), index))
		return FALSE;

	CTGExcelWorksheets worksheets = m_WorkBook.get_Worksheets();
	CTGExcelWorksheet  worksheet  = worksheets.get_Item(COleVariant((short)index));

	int colcount = rowvalues.size();

	wostringstream startCell;
	startCell << chColStart << nRowindex;

	CTGExcelRange Range;
	Range = worksheet.get_Range(COleVariant(startCell.str().c_str()), COleVariant(startCell.str().c_str()));
	Range = Range.get_Resize(COleVariant((short)1), COleVariant((LONGLONG)colcount));

	COleSafeArray saRet;
	DWORD numElements[2];
	numElements[0] = 1;
	numElements[1] = colcount;
	saRet.Create(VT_BSTR, 2, numElements);

	long lCellIdx[2];
	lCellIdx[0] = 0;
	for (int nCol = 0; nCol < colcount; nCol++)
	{
		lCellIdx[1] = nCol;
		BSTR bstr = rowvalues[nCol].AllocSysString();
		saRet.PutElement(lCellIdx, bstr);
		SysFreeString(bstr);
	}
	// Send the array to Excel.
	Range.put_Value(COleVariant(saRet));

	return TRUE;
}


//------------------------------------------------------------------------------ 
//! @brief
//! @date     2014-05-22
//! @owner    cygil@esmlab.com
//! @note     sheetname : name of the specified worksheet
//!           startindex  : startColIndex 'A', 'B', 'C', ...
//!           endindex  : endColIndex 'A', 'B', 'C', ...
//!           nRowindex	: 1, 2, 3, 4, 5, ...
//!           rowvalues : all the values of the specified row.
//! @return        
//! @revision 
//------------------------------------------------------------------------------ 
BOOL CTGExcelManager::SetTableValues(wstring sheetname, int nRowindex, vector<vector<CString>> tablevalues, wchar_t chColStart, BOOL bAutoFit)
{
	int index = 0;
	if(!GetIndexBySheetName(CString(sheetname.c_str()), index))
		return FALSE;

	CTGExcelWorksheets worksheets = m_WorkBook.get_Worksheets();
	CTGExcelWorksheet  worksheet  = worksheets.get_Item(COleVariant((short)index));

	int rowcount = tablevalues.size();
	int colcount = 0;
	vector<CString> rowvalues;
	for(int nRow = 0; nRow < rowcount; nRow++)
	{
		rowvalues = tablevalues[nRow];
		if(colcount <= (int)rowvalues.size())
			colcount = rowvalues.size();
	}

	wostringstream startCell;
	startCell << chColStart << nRowindex;

	CTGExcelRange Range;
	Range = worksheet.get_Range(COleVariant(startCell.str().c_str()), COleVariant(startCell.str().c_str()));
	Range = Range.get_Resize(COleVariant((LONGLONG)rowcount), COleVariant((LONGLONG)colcount));

	COleSafeArray saRet;
	DWORD numElements[2];
	numElements[0] = rowcount;
	numElements[1] = colcount;
	saRet.Create(VT_BSTR, 2, numElements);

	long lCellIdx[2];
	for (int nRow = 0; nRow < rowcount; nRow++)
	{
		rowvalues = tablevalues[nRow];
		for (int nCol = 0; nCol < colcount; nCol++)
		{
			lCellIdx[0] = nRow;
			lCellIdx[1] = nCol;
			BSTR bstr = rowvalues[nCol].AllocSysString();
			saRet.PutElement(lCellIdx, bstr);
			SysFreeString(bstr);
		}
	}
	// Send the array to Excel.
	Range.put_Value(COleVariant(saRet));

	if(bAutoFit)
	{
		CTGExcelRange colRange = Range.get_EntireColumn();
		colRange.AutoFit();								// 최고 긴 글자 길이 만큼 자동 컬럼 길이 설정.
	}
	return TRUE;
}
//------------------------------------------------------------------------------ 
//! @brief
//! @date     2013-03-06
//! @owner    yongsoo.ko
//! @note     sheetname : name of the specified worksheet
//!           rowindex  : "1", "2", "3", ....
//!           rowvalues : all the values of the specified row.
//! @return        
//! @revision 
//------------------------------------------------------------------------------ 
BOOL CTGExcelManager::GetScheduleTableValues(wstring sheetname, int rowindex, vector<CString>& rowvalues, int nDeviceType)
{	
	int index = 0;
	GetSheetIndex(sheetname, index);

	CTGExcelWorksheets Worksheets = m_WorkBook.get_Worksheets();
	CTGExcelWorksheet  WorkSheet  = Worksheets.get_Item(COleVariant((short)index));
	CTGExcelRange Cells = WorkSheet.get_Cells();
	CTGExcelRange Range;

	COleVariant covOptional(DISP_E_PARAMNOTFOUND, VT_ERROR);
	CString WEEK[] = { _T("SUN"),  _T("MON"), _T("TUE"), _T("WED"), _T("THU"), _T("FRI"), _T("SAT"), _T("HOLIDAY") };

	CString strValue = _T("");
	CString strCell;
	CString strTemp;

	//Get Table Name
	Range = Cells.get_Item(COleVariant((long)rowindex), COleVariant((long)1)).pdispVal;
	strCell = Range.get_Text();
	rowvalues.push_back(strCell);

	//Get Schedule Table
	int nTaebleRow = 0;
	nDeviceType	== 0 ?	nTaebleRow = 7	 :	nTaebleRow  = 8;
	for (int i = 0; i < nTaebleRow; i++)
	{
		for (int j = 0; j < 24; j++)
		{
			Range = Cells.get_Item(COleVariant((long)i+rowindex+1), COleVariant((long)j+3)).pdispVal; //rowindex+1, j+3 은 데이터 위치로 인한 offset 값
			strCell = Range.get_Text();

			if (strCell != _T(""))
			{
				strTemp.Format(_T("%s%d=%s"), WEEK[i], j, strCell);
				if (strValue != _T(""))
					strValue += _T("&");
				strValue += strTemp;
			}
		}
	}

	rowvalues.push_back(strValue);
	
	return TRUE;
}

//------------------------------------------------------------------------------ 
//! @brief
//! @date     2011-11-29
//! @owner    keunbae.song
//! @note
//! @return        
//! @revision 
//------------------------------------------------------------------------------ 
BOOL CTGExcelManager::GetRowCount(int sheetindex, int& count)
{
	COleVariant covOptional(DISP_E_PARAMNOTFOUND, VT_ERROR);

	CTGExcelWorksheets Worksheets = m_WorkBook.get_Worksheets();
	CTGExcelWorksheet  WorkSheet  = Worksheets.get_Item(COleVariant((short)sheetindex));

	//Get a CTGExcelRange object that represents the used range on the worksheet.
	CTGExcelRange UsedRange = WorkSheet.get_UsedRange();
	count = ((CTGExcelRange)UsedRange.get_EntireRow()).get_Count();

	return TRUE;
}

//------------------------------------------------------------------------------ 
//! @brief    Get all the count of rows in excel file. 
//! @date     2011-11-29
//! @owner    keunbae.song
//! @note
//! @return        
//! @revision 
//------------------------------------------------------------------------------ 
BOOL CTGExcelManager::GetAllRowCount(int& count)
{
	count = 0;

  int sheetcount = 0;
	GetSheetsCount(sheetcount);

	int rowcount = 0;
	for(int i=1; i<=sheetcount; i++)
	{
		GetRowCount(i, rowcount);
		count += rowcount;
	}

	return TRUE;
}

//------------------------------------------------------------------------------ 
//! @brief
//! @date     2011-12-2
//! @owner    keunbae.song
//! @note
//! @return        
//! @revision 
//------------------------------------------------------------------------------ 
BOOL CTGExcelManager::Save()
{
	m_WorkBook.Save();

	return TRUE;
}

BOOL CTGExcelManager::AutoFit(wstring sheetname, wstring startCol, wstring endCol, int startRow, int endRow)
{
	int index = 0;
	if(!GetIndexBySheetName(CString(sheetname.c_str()), index))
		return FALSE;

	CTGExcelWorksheets worksheets = m_WorkBook.get_Worksheets();
	CTGExcelWorksheet  worksheet  = worksheets.get_Item(COleVariant((short)index));

	wostringstream startCell;
	startCell << startCol.c_str() << startRow;

	wostringstream endCell;
	endCell << endCol.c_str() << endRow;

	CTGExcelRange Range;
	Range  = worksheet.get_Range(COleVariant(startCell.str().c_str()), COleVariant(endCell.str().c_str()));

	CTGExcelRange colRange = Range.get_EntireColumn();
	colRange.AutoFit();								// 최고 긴 글자 길이 만큼 자동 컬럼 길이 설정.

	return TRUE;
}
//------------------------------------------------------------------------------ 
//! @brief
//! @date     2012-11-15
//! @owner    joonho.kim
//! @note
//! @return        
//! @revision 
//------------------------------------------------------------------------------ 
BOOL CTGExcelManager::CreateExcelFile(CString strFilePath, BOOL bAlertMsg)
{
    CTGExcelApplication objApp;
    CTGExcelWorkbook objBook;
    CTGExcelWorkbooks objBooks;
/*
    COleVariant VOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
    COleVariant VFalse((short)FALSE);
    COleException* pe = new COleException;

    try
    {
        if(!objApp.CreateDispatch(_T("Excel.Application")))
            throw pe;
        objApp.put_Visible(FALSE);
        objApp.put_DisplayAlerts(bAlertMsg);//에러시 내장 경고메시지를 보여/안보여 준다
        objBooks = objApp.get_Workbooks();
        objBook = objBooks.Add(VOptional);

        //XlFileFormat::xlWorkbookNormal = -4143 기본값
        //XlSaveAsAccessMode::xlNoChange = 1 기본값
        objBook.SaveAs(COleVariant(strFilePath), COleVariant((long)-4143), 
                                COleVariant(_T(""), VT_BSTR),  
                                COleVariant(_T(""), VT_BSTR), 
                                 VFalse, VFalse, 1, VOptional, VOptional, VOptional, VOptional);

    }
*/
	COleVariant VTrue((short)TRUE);
	COleVariant VFalse((short)FALSE);
	COleVariant VOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
	COleException* pe = new COleException;

	try
	{
		CLSID clsid;
		HRESULT hr = CLSIDFromProgID(L"Excel.Application", &clsid);

		if(!objApp.CreateDispatch(clsid, pe))
			throw pe;
		objApp.put_Visible(FALSE);
        objApp.put_DisplayAlerts(bAlertMsg);//에러시 내장 경고메시지를 보여/안보여 준다
        objBooks = objApp.get_Workbooks();
        objBook = objBooks.Add(VOptional);

        //XlFileFormat::xlWorkbookNormal = -4143 기본값
        //XlSaveAsAccessMode::xlNoChange = 1 기본값
        objBook.SaveAs(COleVariant(strFilePath), COleVariant((long)-4143), 
                                COleVariant(_T(""), VT_BSTR),  
                                COleVariant(_T(""), VT_BSTR), 
                                 VFalse, VFalse, 1, VOptional, VOptional, VOptional, VOptional);

    }
	catch (COleDispatchException* pe)
    {
        CString strMsg;

        if (!pe->m_strSource.IsEmpty())
            strMsg = pe->m_strSource + _T(" - ");
        if (!pe->m_strDescription.IsEmpty())
            strMsg += pe->m_strDescription;
        else
            strMsg += _T("unknown error");

        TRACE(strMsg + _T("\n"));

        pe->Delete();
        return FALSE;

    }
    catch (CException* pe)
    {
        CStringA strMsg;
        strMsg.Format("%s(%d): OLE Execption caught: SCODE = %x",
            __FILE__, __LINE__, COleException::Process(pe));

        TRACE(CA2T(strMsg + _T("\n")));
        pe->Delete();
        return FALSE;
    }

    objBook.Close(COleVariant((short)FALSE), VOptional, VOptional);
    objApp.Quit();

    objBook.ReleaseDispatch();
    objBooks.ReleaseDispatch();
    objApp.ReleaseDispatch();

    pe->Delete();

    return TRUE;
}
/*
wstring CTGExcelManager::FindEndColumnName(wstring strStart, int nColumnCnt)
{
	enum {
		eColA = 0, eColB, eColC, eColD, eColE, eColF, eColG, eColH, eColI, eColJ, 
		eColK = 0, eColL, eColM, eColN, eColO, eColP, eColQ, eColR, eColS, eColT, 
		eColU = 0, eColV, eColW, eColX, eColY, eColZ
	};
	wchar_t szDef[] = {
		_T('A'), _T('B'), _T('C'), _T('D'), _T('E'), _T('F'), _T('G'), _T('H'), _T('I'), _T('J'),
		_T('K'), _T('L'), _T('M'), _T('N'), _T('O'), _T('P'), _T('Q'), _T('R'), _T('S'), _T('T'),
		_T('U'), _T('V'), _T('W'), _T('X'), _T('Y'), _T('Z')
	};

	wstring strStartCol = strStart;
	wchar_t szColName[5] = {0x00};
	
	int nStrLength = strStartCol.size();
	int nLastIdx = 0;
	int nChCount = nColumnCnt - 1;
	
	if(nStrLength == 0)
		szColName[0] = szDef[eColA];
	else if(nStrLength <= 2)
		memcpy(szColName, strStartCol.c_str(), nStrLength);
	else
		return _T("");

	int nCurrentIdx = nLastIdx;

	if(nColumnCnt > 26)
	{
		int nQuotient = nChCount/26;
		int nRemainder = nChCount%26;
		if(szColName[nLastIdx] + nRemainder > szDef[eColZ])
		{
			szColName[nLastIdx] += nRemainder;
			int nCnt = szColName[nLastIdx] - szDef[eColZ];
			if(nLastIdx == 0)
				szColName[nLastIdx+1]	= szDef[eColA] + (nCnt-1);
			else
				szColName[nLastIdx]		= szDef[eColA] + (nCnt-1);
			nQuotient++;
		}
		if(nQuotient > 0)
		{
			nLastIdx = nLastIdx - 1;
			szColName[nLastIdx] += nQuotient;
		}
	}
	else
	{
		if(szColName[nLastIdx] + nChCount > szDef[eColZ])
		{
			szColName[nLastIdx] += nChCount;
			int nCnt = szColName[nLastIdx] - szDef[eColZ];
			if(nLastIdx == 0)
			{
				szColName[nLastIdx]		= szDef[eColA];
				szColName[nLastIdx+1]	= szDef[eColA] + (nCnt-1);
			}
			else
			{
				szColName[nLastIdx-1]	= szColName[nLastIdx-1]++;
				szColName[nLastIdx]		= szDef[eColA] + (nCnt-1);
			}
		}
	}
}
*/
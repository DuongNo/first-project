////////////////////////////////////////////////////////////////////////////////
//
//	TGExcelManager.h : implementation of the TestGuarantee Function Class.
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


#pragma once

#include "TGExcelApplication.h"
#include "TGExcelRange.h"
#include "TGExcelWorkbook.h"
#include "TGExcelWorkbooks.h"
#include "TGExcelWorksheet.h"
#include "TGExcelWorksheets.h"
#include "TGExcelInterior.h"
#include "TGExcelFont.h"

#include <vector>
#include <map>
#include <sstream>

using namespace std;

//-- 2010-11-26 keunbae.song
typedef struct _TGMultiLanguageSheet
{
	CStringW sheetname;
	map<CStringW, CStringW> menustrmap;
} TGMultiLanguageSheet, *PTGMultiLanguageSheet;

typedef vector<TGMultiLanguageSheet> TGMultiLanguageSheetVector;

class CTGExcelManager
{
public:
	CTGExcelManager();
	~CTGExcelManager();
	
	BOOL Open(CString& strExcelFilePath, CString& strError=CString(_T("")));
	BOOL Open(CString& strExcelFilePath, CString& strError, BOOL bReadOnly);
#ifdef _TG		//SMILE인경우만
	BOOL Open(CString& strExcelFilePath, BOOL bModalMsgBox);
#endif
	BOOL Close();
	BOOL ReadLanguageTable(int nSheetIndex, TGMultiLanguageSheetVector& vecLanguageSheet);
	BOOL GetLanguageSheets(int& nNumofSheets, CStringArray& arstrLangNames);
	BOOL GetIndexBySheetName(CString strSheetName, int& nIndex); 
	BOOL GetColValue(CString strSheetName, CString strCol, int nIndex, CString& strVal);
	BOOL GetColValues(CString strSheetName, CString strCol, int nFromIndex, CStringArray& arrStrValues);
	BOOL ClearSheet(int nSheetIndex);
	BOOL GetColumnIndex(int nSheetIndex, map<CString, int> & mapColumnIndex);

	BOOL IsOpened();
	BOOL GetSheetsCount(int& count);
	BOOL GetSheetIndex(wstring sheetname, int& index); 
	BOOL GetSheetName(int index, wstring& name);
	BOOL GetColumnCount(int index, int& count);
	BOOL SetCellValue(wstring sheetname, wstring colindex, int rowindex, wstring cellvalue);
	BOOL GetCellValue(wstring sheetname, wstring colindex, int rowindex, wstring& cellvalue);
	BOOL GetColumnValues(wstring sheetname, wstring colindex, vector<wstring>& colvalues);
	BOOL GetColumnValues(CString sheetname, CString colindex, vector<CString>& colvalues);
	BOOL SetCellColor(wstring sheetname, wstring colindex, int rowindex, COLORREF color);
	BOOL SetRangeColor(wstring sheetname, wstring startindex, int nStartindex, wstring endindex, int nEndindex, COLORREF color);
	BOOL SetCellFontColor(wstring sheetname, wstring colindex, int rowindex, COLORREF color);
	BOOL SetCellFont(wstring sheetname, wstring colindex, int rowindex, wstring fontname, int nsize, COLORREF color, BOOL bBold, BOOL bItalic);
	//-- 2012-1-3 Jeongyong.kim
	//-- Add GetRowValues
	BOOL SetRowValues(wstring sheetname, int nRowindex, vector<CString> rowvalues, wchar_t chColStart = _T('A'));
	BOOL SetTableValues(wstring sheetname, int nRowindex, vector<vector<CString>> tablevalues, wchar_t chColStart = _T('A'), BOOL bAutoFit = TRUE);
	BOOL GetRowValues(IN wstring sheetname, IN int rowindex, OUT vector<CString>& rowvalues, wchar_t chColStart = _T('A'));
	BOOL GetRowValues(IN wstring sheetname, IN int rowindex, IN int colcount, OUT vector<CString>& rowvalues, wchar_t chColStart = _T('A'));
	BOOL GetTableValues(wstring sheetname, int rowindex, int rowcount, vector<vector<CString>>& tablevalues, wchar_t chColStart = _T('A'));
	BOOL GetTableValues(wstring sheetname, int colcount, int rowindex, int rowcount, vector<vector<CString>>& tablevalues, wchar_t chColStart = _T('A'));
	//BOOL GetScheduleTableValues(IN wstring sheetname, IN int rowindex, OUT vector<CString>& rowvalues);
	BOOL GetScheduleTableValues(IN wstring sheetname, IN int rowindex, OUT vector<CString>& rowvalues, int nDeviceType = 0);
	BOOL GetRowCount(int sheetindex, int& count);
	BOOL GetAllRowCount(OUT int& count);
	BOOL Save();
	BOOL AddSheet();
	BOOL SetSheetName(int index, wstring name);

	//-- 2012-11-15 joonho.kim
	//-- Create Excel File
	BOOL AutoFit(wstring sheetname, wstring startCol, wstring endCol, int startRow, int endRow);
	BOOL CreateExcelFile(CString strFilePath, BOOL bAlertMsg);
	CTGExcelApplication*	GetExcelApplication();
	
private:
	CTGExcelApplication m_ExcelApp;
	CTGExcelWorkbook m_WorkBook;
	CTGExcelWorkbooks m_WorkBooks;
	BOOL m_bOpened;

//	wstring FindEndColumnName(wstring strStart, int nColumnCnt);
	TGMultiLanguageSheetVector m_vecLanguageSheet;
};

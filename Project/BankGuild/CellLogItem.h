#pragma once

enum CELL_LOG_DATATYPE
{
	CELL_LOG_DATA_INT = 0,
	CELL_LOG_DATA_INT_ADD1,
	CELL_LOG_DATA_DOUBLE,
	CELL_LOG_DATA_STRING,
	CELL_LOG_DATA_CLASS_RESULT,
	CELL_LOG_DATA_ALIGN_RESULT,
	CELL_LOG_DATA_CLASS_CELL,
	CELL_LOG_DATA_TIME,
	CELL_LOG_DATA_TACT
};

class CCellLogItem
{
private:
	int				*m_ptrInt;
	double			*m_ptrDouble;
	CString			*m_ptrString;
	CLASS_RESULT	*m_ptrClassResult;
	ALIGN_RESULT	*m_ptrAlignResult;
	CLASS_CELL		*m_ptrClassCell;
	SYSTEMTIME		*m_ptrSystemTime1;
	SYSTEMTIME		*m_ptrSystemTime2;

	CString m_strTitle;
	CELL_LOG_DATATYPE m_Type;

public:
	CCellLogItem(void);
	CCellLogItem(CString strTitle, CELL_LOG_DATATYPE type, void *ptr1, void *ptr2 = NULL);
	~CCellLogItem(void);

	void Init();
	void InitValue();

	void SetTitle(CString strTitle);
	CString GetTitle();

	void SetValuePtr(CELL_LOG_DATATYPE type, void *ptr, void *ptr2 = NULL);
	CString ValueToString();

	double CountTactTime();

};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  
// [ Gaus Project ]
//
// GxIni : INI 파일을 다루기  위한 Utility Class
//
// 2014. 02. 27, jhLee (semsemi)
// 
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Gaus.h"

#define _DELIMITER_ _T(":")			// 구분자, Section 이름과 Key 이름을 구분해준다.

typedef CTypedPtrMap<CMapStringToPtr, CString, BOOL*>	CMapStringToBool;
typedef CTypedPtrMap<CMapStringToPtr, CString, SHORT*>	CMapStringToShort;
typedef CTypedPtrMap<CMapStringToPtr, CString, USHORT*> CMapStringToUshort;
typedef CTypedPtrMap<CMapStringToPtr, CString, int*>	CMapStringToInt;
typedef CTypedPtrMap<CMapStringToPtr, CString, UINT*>	CMapStringToUint;
typedef CTypedPtrMap<CMapStringToPtr, CString, float*>	CMapStringToFloat;
typedef CTypedPtrMap<CMapStringToPtr, CString, double*> CMapStringToDouble;
typedef CTypedPtrMap<CMapStringToPtr, CString, CString*> CMapStringToStr;


class _GAUS_LIB CGxIni // : public CGxObject 
{
public:
	CString		m_sRootPath;								// 기본이 되는 경로
	CString		m_sSect;
	CString		m_sIniFile;
	BOOL		m_bAutoPath;

private:
	CMapStringToBool		m_boolMap;
	CMapStringToShort		m_shortMap;
	CMapStringToUshort		m_ushortMap;
	CMapStringToInt			m_intMap;
	CMapStringToUint		m_uintMap;
	CMapStringToFloat		m_floatMap;
	CMapStringToDouble		m_doubleMap;
	CMapStringToStr			m_stringMap;

public:
	CGxIni();
	CGxIni(CString sIniFile);
	virtual ~CGxIni();

	CString GetPath(BOOL bFullPath=FALSE);
	void MakeFoloer(CString strPath);
	BOOL IsExistFolder(CString strPath);

	virtual void Load();
	virtual void Save();

	void AddBool(LPCTSTR pszKey, BOOL& num);
	void AddShort(LPCTSTR pszKey, SHORT& num);
	void AddUshort(LPCTSTR pszKey, USHORT& num);
	void AddInt(LPCTSTR pszKey, int& num);
	void AddUint(LPCTSTR pszKey, UINT& num);
	void AddFloat(LPCTSTR pszKey, float& num);
	void AddDouble(LPCTSTR pszKey, double& num);
	void AddStr(LPCTSTR pszKey, CString& str);

	
	BOOL	GetBool(LPCTSTR pszKey);
	SHORT	GetShort(LPCTSTR pszKey);
	USHORT	GetUshort(LPCTSTR pszKey);
	int		GetInt(LPCTSTR pszKey);
	UINT	GetUint(LPCTSTR pszKey);
	float	GetFloat(LPCTSTR pszKey);
	double	GetDouble(LPCTSTR pszKey);
	CString GetStr(LPCTSTR pszKey);


	void WriteBool(LPCTSTR pszKey, BOOL nVal);
	void WriteBool(LPCTSTR pszKey, UINT nSize, BOOL *pVal);
	void WriteBool(LPCTSTR pszSect, LPCTSTR pszKey, BOOL nVal);
	void WriteBool(LPCTSTR pszSect, LPCTSTR pszKey, UINT nSize, BOOL *pVal);
	void WriteShort(LPCTSTR pszKey, SHORT nVal);
	void WriteShort(LPCTSTR pszKey, UINT nSize, SHORT *pVal);
	void WriteShort(LPCTSTR pszSect, LPCTSTR pszKey, SHORT nVal);
	void WriteShort(LPCTSTR pszSect, LPCTSTR pszKey, UINT nSize, SHORT *pVal);
	void WriteUshort(LPCTSTR pszKey, USHORT nVal);
	void WriteUshort(LPCTSTR pszKey, UINT nSize, USHORT *pVal);
	void WriteUshort(LPCTSTR pszSect, LPCTSTR pszKey, USHORT nVal);
	void WriteUshort(LPCTSTR pszSect, LPCTSTR pszKey, UINT nSize, USHORT *pVal);
	void WriteInt(LPCTSTR pszKey, int nVal);
	void WriteInt(LPCTSTR pszKey, UINT nSize, int *pVal);
	void WriteInt(LPCTSTR pszSect, LPCTSTR pszKey, int nVal);
	void WriteInt(LPCTSTR pszSect, LPCTSTR pszKey, UINT nSize, int *pVal);
	void WriteUint(LPCTSTR pszKey, UINT nVal);
	void WriteUint(LPCTSTR pszKey, UINT nSize, UINT *pVal);
	void WriteUint(LPCTSTR pszSect, LPCTSTR pszKey, UINT nVal);
	void WriteUint(LPCTSTR pszSect, LPCTSTR pszKey, UINT nSize, UINT *pVal);
	void WriteLong(LPCTSTR pszKey, long nVal);
	void WriteLong(LPCTSTR pszKey, UINT nSize, long *pVal);
	void WriteLong(LPCTSTR pszSect, LPCTSTR pszKey, long nVal);
	void WriteLong(LPCTSTR pszSect, LPCTSTR pszKey, UINT nSize, long *pVal);
	void WriteFloat(LPCTSTR pszKey, float nVal);
	void WriteFloat(LPCTSTR pszKey, UINT nSize, float *pVal);
	void WriteFloat(LPCTSTR pszSect, LPCTSTR pszKey, float nVal);
	void WriteFloat(LPCTSTR pszSect, LPCTSTR pszKey, UINT nSize, float *pVal);
	void WriteDouble(LPCTSTR pszKey, double nVal);
	void WriteDouble(LPCTSTR pszKey, UINT nSize, double *pVal);
	void WriteDouble(LPCTSTR pszSect, LPCTSTR pszKey, double nVal);
	void WriteDouble(LPCTSTR pszSect, LPCTSTR pszKey, UINT nSize, double *pVal);
	void WriteStr(LPCTSTR pszKey, LPCTSTR pszVal);
	void WriteStr(LPCTSTR pszSect, LPCTSTR pszKey, LPCTSTR pszVal);

	void WriteStruct(LPCTSTR pszKey, void* pStruct, UINT nSize);
	void WriteStruct(LPCTSTR pszSect, LPCTSTR pszKey, void* pStruct, UINT nSize);


	BOOL    ReadBool(LPCTSTR pszKey);
	void    ReadBool(LPCTSTR pszKey, BOOL *pVal);
	BOOL    ReadBool(LPCTSTR pszSect, LPCTSTR pszKey);
	void    ReadBool(LPCTSTR pszSect, LPCTSTR pszKey, BOOL *pVal);
	SHORT   ReadShort(LPCTSTR pszKey);
	void    ReadShort(LPCTSTR pszKey, SHORT *pVal);
	SHORT   ReadShort(LPCTSTR pszSect, LPCTSTR pszKey);
	void    ReadShort(LPCTSTR pszSect, LPCTSTR pszKey, SHORT *pVal);
	USHORT  ReadUshort(LPCTSTR pszKey);
	void    ReadUshort(LPCTSTR pszKey, USHORT *pVal);
	USHORT  ReadUshort(LPCTSTR pszSect, LPCTSTR pszKey);
	void    ReadUshort(LPCTSTR pszSect, LPCTSTR pszKey, USHORT *pVal);
	int     ReadInt(LPCTSTR pszKey);
	void    ReadInt(LPCTSTR pszKey, int *pVal);
	int     ReadInt(LPCTSTR pszSect, LPCTSTR pszKey);
	void    ReadInt(LPCTSTR pszSect, LPCTSTR pszKey, int *pVal);
	UINT    ReadUint(LPCTSTR pszKey);
	void    ReadUint(LPCTSTR pszKey, UINT *pVal);
	UINT    ReadUint(LPCTSTR pszSect, LPCTSTR pszKey);
	void    ReadUint(LPCTSTR pszSect, LPCTSTR pszKey, UINT *pVal);
	long    ReadLong(LPCTSTR pszKey);
	void    ReadLong(LPCTSTR pszKey, long *pVal);
	long    ReadLong(LPCTSTR pszSect, LPCTSTR pszKey);
	void    ReadLong(LPCTSTR pszSect, LPCTSTR pszKey, long *pVal);
	float   ReadFloat(LPCTSTR pszKey);
	void    ReadFloat(LPCTSTR pszKey, float *pVal);
	float   ReadFloat(LPCTSTR pszSect, LPCTSTR pszKey);
	void    ReadFloat(LPCTSTR pszSect, LPCTSTR pszKey, float *pVal);
	double  ReadDouble(LPCTSTR pszKey);
	void    ReadDouble(LPCTSTR pszKey, double *pVal);
	double  ReadDouble(LPCTSTR pszSect, LPCTSTR pszKey);
	void    ReadDouble(LPCTSTR pszSect, LPCTSTR pszKey, double *pVal);
	CString ReadStr(LPCTSTR pszKey);
	void    ReadStr(LPCTSTR pszKey, CString *pStr);
	CString ReadStr(LPCTSTR pszSect, LPCTSTR pszKey);
	void    ReadStr(LPCTSTR pszSect, LPCTSTR pszKey, CString *pStr);

	void    ReadStruct(LPCTSTR pszKey, void* pStruct, UINT nSize);
	void    ReadStruct(LPCTSTR pszSect, LPCTSTR pszKey, void* pStruct, UINT nSize);


	// 2010-08-31, jhLee
	CString GetSectionName(LPCTSTR pszKey);
	virtual void Load(LPCTSTR pszSect);			// 지정된 Section 명을 가진 값들만 Load 하고 Save 한다.
	virtual void Save(LPCTSTR pszSect);

	BOOL WriteStringSection(LPCTSTR pszKey, CString sValue);
	BOOL ReadStringSection(LPCTSTR pszKey, CString &sValue);

protected:
	int WriteString(LPCTSTR pszSect, LPCTSTR pszKey, LPCTSTR pszStr);
	int ReadString(LPCTSTR pszSect, LPCTSTR pszKey, LPTSTR pszStr, DWORD nSize);
};

typedef CArray<CGxIni, CGxIni&> CGxIniArray;
typedef CTypedPtrArray<CPtrArray, CGxIni*> CGxIniPtrArray;


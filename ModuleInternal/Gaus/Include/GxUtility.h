//
// 각종 편의 함수 모음


#pragma once


#include <math.h>			// for pow
#include <cmath>			// for floor


#include "Gaus.h"


#define GxROUNDING(x, dig)  ( floor((x) * pow(float(10), dig) + 0.5f) / pow(float(10), dig) )



class _GAUS_LIB CGxUtility
{
public:
	CGxUtility(void);
	~CGxUtility(void);


public : // 폴더 관련 함수
	
	// 지정한 파일/폴더가 존재하는가 ?
	static BOOL IsExistFile(CString sFile);
	static BOOL IsExistFolder(CString sFolder);

	static BOOL MakeDirectory(CString sPath);		// 하위 디렉토리까지 모두 생성해주는 함수
	static BOOL DeleteDirectory(CString sPath,  BOOL bDeletePermanent = TRUE);		// 하위 디렉토리까지 모두 삭제해주는 함수

	// 날짜 관련 함수
	// CString "YYYY-MM-DD HH:NN:SS.ZZZ" -> SystemTime
	static void StringToSystemTime(CString sDate, SYSTEMTIME &tmNew);

	// SystemTime -> CString, "YYYY-MM-DD HH:NN:SS.ZZZ"
	static CString SystemTimeToString(SYSTEMTIME tmTime);
	static CString SystemTimeToStringClock(SYSTEMTIME tmTime);

	// 두 시간사이의 차이를 초 단위로 구해준다.
	static float DiffSeconds(SYSTEMTIME StartTime, SYSTEMTIME EndTime);
	// 두 시간사이의 시간차를 구해준다.
	static void DiffTimes(SYSTEMTIME StartTime, SYSTEMTIME EndTime, SYSTEMTIME &DiffTime);

	// 지정한 초 단위 시각을 SYSTEMTIME형식으로 변환한다.
	static void SecondToSystemTime(double dbSec, SYSTEMTIME &tmNew);

	// 지정한 시간(초) 이전의 시각으로 되돌려준다.
	static void PrevSecondTime(int nSec, SYSTEMTIME &tmNew);


	// 편의 함수
	
	static BOOL IsIn(int nKey, ...);				// 지정 int형 Key값이 주어진 인수 리스트에 포함되어있는가 ? 리스트의 끝은 -1로 마무리해준다.
	static BOOL IsIn_s(int nCount, int nKey, ...);	// 지정 int형 Key값이 주어진 인수 리스트에 포함되어있는가 ? 리스트의 끝은 -1로 마무리해준다.
	static BOOL IsIn(CString sKey, ...);			// 지정 String형 Key값이 주어진 인수 리스트에 포함되어있는가 ? 리스트의 끝은 널( _T("") )로 마무리해준다.
	static BOOL IsIn_s(int nCount, CString sKey, ...);	// 지정 String형 Key값이 주어진 인수 리스트에 포함되어있는가 ? 리스트의 끝은 널( _T("") )로 마무리해준다.

};


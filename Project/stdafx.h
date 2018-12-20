
// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이 
// 들어 있는 포함 파일입니다.

#pragma once



// 본 프로젝트만의 컴파일 옵션 - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
//#pragma warning(disable: 4819)	// MIL 에서 C4819 코드페이 경고 다량발생으로 해당 경고 무시하도록 지정
		// mil.h : warning C4819: 현재 코드 페이지(949)에서 표시할 수 없는 문자가 파일에 들어 있습니다. 
		//				데이터가 손실되지 않게 하려면 해당 파일을 유니코드 형식으로 저장하십시오.

// #define _CRT_SECURE_NO_WARNINGS	// C4996 경고 disable, fopen(), strtok() 류의 함수에서 길이지정을해주는 xxx_s 류의 함수로 대체하도록하는 권고 무시
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
//#define AMT_TYPE 1	// 1차분 2대
#define AMT_TYPE 2  // 2차분

//2017-03-31,skcho, 1이면 ok in, out lift 리미트 센서 위치가 위쪽을
#define LIMIT_TYPE 2

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 일부 CString 생성자는 명시적으로 선언됩니다.

// MFC의 공통 부분과 무시 가능한 경고 메시지에 대한 숨기기를 해제합니다.
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 핵심 및 표준 구성 요소입니다.
#include <afxext.h>         // MFC 확장입니다.


#include <afxdisp.h>        // MFC 자동화 클래스입니다.



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // Internet Explorer 4 공용 컨트롤에 대한 MFC 지원입니다.
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // Windows 공용 컨트롤에 대한 MFC 지원입니다.
#endif // _AFX_NO_AFXCMN_SUPPORT


#include <afxsock.h>            // MFC 소켓 확장



//GDI+ 사용
#include <gdiplus.h>
#pragma comment(lib, "gdiplus")
using namespace Gdiplus;


//
// Visual Studio 2005, 2008 ~ 에서 TRACE 문 사용 시 한글을 사용할 경우,
// _CrtDbgReport: String too long or IO Error 오류가 발생시,
//
// #include <locale.h>     include 한 후
//
// _tsetlocale(LC_ALL, _T("korean"));  호출
//

// Visual Momory Leak Detect
//미사용 #include "VLD\vld.h"			// for Memory Detect !!

#include "Etc/Etc.h"
#include "Socket/DataSocket.h"
#include "BankGuild/CommonDefineBank.h"
#include "BankGuild/AlarmDefine.h"
#include "Etc/Token.h"

#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif

//kjpark 20160908 kjpark Test Mode Set
#define CMiLRe 
//kjpark 20161010 라벨프린터 PG PC 에서 사용
//#define OPERATOR_PC
#define DISPLAY_INSPECT_CHECK
//#define DRY_RUN
//kjpark 20161019 장비 작업 미스로 B<->D 존이 바뀌었음
#define B2D_CHANGE//Worktable 에 B 존과 D 존이 장비 작업 미스로 바꼈음
#include "GeneralPurposeFunction.h"
#include <afxcontrolbars.h>

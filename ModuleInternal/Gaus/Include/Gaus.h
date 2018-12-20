//
// Gaus.h : GausLib DLL의 기본 헤더 파일입니다. 모든 Gaus library header file에 포함 시키십시오.
//
// 2015-04-23, jhLee, 기본 개념 구상
// 2015-08-18, jhLee, 변환 시작
//

#pragma once

//
// #define _DLL_EXPORTS				// DLL 제작
// #define _ORIGIN_SOURCE			// 원본 소스를 통한 디버깅용
//

#ifndef _GAUS_LIB

	#ifndef _ORIGIN_SOURCE

		#ifdef _DLL_EXPORTS
			#define _GAUS_LIB	__declspec(dllexport)
		#else
			#ifdef _DEBUG
				#pragma comment(lib, "GausDll.LIB")		// Gaus Lib for Debug mode 
			#else
				#pragma comment(lib, "GausDll.LIB")		// Gaus Lib for Release mode 
			#endif

			#define _GAUS_LIB	__declspec(dllimport)
		#endif //of #ifdef _DLL_EXPORTS

	#else //of #ifndef _ORIGIN_SOURCE
		#define _GAUS_LIB									// 소스 자체를 사용한다.
	#endif //of #ifndef _ORIGIN_SOURCE

#endif //of #ifndef _GAUS_LIB
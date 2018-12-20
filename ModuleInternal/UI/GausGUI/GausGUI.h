//
// GausGUI.h : GausGUI DLL의 기본 헤더 파일입니다. 모든 GausGUI library header file에 포함 시키십시오.
//
// 2015-10-07, jhLee, DLL로 변환
//

#pragma once

//
// #define _DLL_EXPORTS				// DLL 제작
// #define _ORIGIN_SOURCE			// 원본 소스를 통한 디버깅용
//

#ifndef _GAUS_GUI

	#ifndef _ORIGIN_SOURCE

		#ifdef _DLL_EXPORTS
			#define _GAUS_GUI	__declspec(dllexport)
		#else
			#ifdef _DEBUG
				#pragma comment(lib, "GausGUI.LIB")		// Gaus Lib for Debug mode 
			#else
				#pragma comment(lib, "GausGUI.LIB")		// Gaus Lib for Release mode 
			#endif

			#define _GAUS_GUI	__declspec(dllimport)
		#endif //of #ifdef _DLL_EXPORTS

	#else //of #ifndef _ORIGIN_SOURCE
		#define _GAUS_GUI									// 소스 자체를 사용한다.
	#endif //of #ifndef _ORIGIN_SOURCE

#endif //of #ifndef _GAUS_LIB
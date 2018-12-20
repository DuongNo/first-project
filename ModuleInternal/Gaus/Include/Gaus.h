//
// Gaus.h : GausLib DLL�� �⺻ ��� �����Դϴ�. ��� Gaus library header file�� ���� ��Ű�ʽÿ�.
//
// 2015-04-23, jhLee, �⺻ ���� ����
// 2015-08-18, jhLee, ��ȯ ����
//

#pragma once

//
// #define _DLL_EXPORTS				// DLL ����
// #define _ORIGIN_SOURCE			// ���� �ҽ��� ���� ������
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
		#define _GAUS_LIB									// �ҽ� ��ü�� ����Ѵ�.
	#endif //of #ifndef _ORIGIN_SOURCE

#endif //of #ifndef _GAUS_LIB
////////////////////////////////////////////////////////////////////////////////
//
//	TGExcelInterior.h : implementation of the TestGuarantee Function Class.
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
// CTGExcelFont 래퍼 클래스

class CTGExcelFont : public COleDispatchDriver
{
public:
	CTGExcelFont(){} // COleDispatchDriver 기본 생성자를 호출합니다.
	CTGExcelFont(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CTGExcelFont(const CTGExcelFont& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 특성
public:

	// 작업
public:


	// Filters 메서드
public:
	LPDISPATCH get_Application()
	{
		LPDISPATCH result;
		InvokeHelper(0x94, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	long get_Creator()
	{
		long result;
		InvokeHelper(0x95, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Parent()
	{
		LPDISPATCH result;
		InvokeHelper(0x96, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	long get_Count()
	{
		long result;
		InvokeHelper(0x76, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get__Default(long Index)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms, Index);
		return result;
	}
	LPDISPATCH get_Item(long Index)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xaa, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms, Index);
		return result;
	}
	LPUNKNOWN get__NewEnum()
	{
		LPUNKNOWN result;
		InvokeHelper(0xfffffffc, DISPATCH_PROPERTYGET, VT_UNKNOWN, (void*)&result, NULL);
		return result;
	}
	void put_Color(COLORREF nColor)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x63, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, nColor);
	}
	void put_Size(int nSize)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x68, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, nSize);
	}
	void put_Bold(BOOL bBold)
	{
		int nBold = bBold?1:0;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x60, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, nBold);
	}
	void put_Italic(BOOL bItalic)
	{
		int nItalic = bItalic?1:0;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x65, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, nItalic);
	}
	void put_FontName(VARIANT FontName)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x6e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &FontName);
	}
	void put_Font(VARIANT FontName, int nSize, COLORREF nColor, BOOL bBold, BOOL bItalic)
	{
		static BYTE FontNameParams[] = VTS_VARIANT ;
		InvokeHelper(0x6e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, FontNameParams, &FontName);
		static BYTE FontPropertyParams[] = VTS_I4 ;
		InvokeHelper(0x68, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, FontPropertyParams, nSize);
		InvokeHelper(0x63, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, FontPropertyParams, nColor);
		int nBold = bBold?1:0;
		InvokeHelper(0x60, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, FontPropertyParams, nBold);
		int nItalic = bItalic?1:0;
		InvokeHelper(0x65, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, FontPropertyParams, nItalic);
/*
		put_FontName(FontName);
		put_Size(nSize);
		put_Color(nColor);
		put_Bold(bBold);
		put_Italic(bItalic);
*/
	}
	// Filters 속성
public:

};

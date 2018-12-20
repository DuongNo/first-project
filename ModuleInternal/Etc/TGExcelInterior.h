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

class CTGExcelInterior : public COleDispatchDriver
{
public:
	CTGExcelInterior(){} // COleDispatchDriver 기본 생성자를 호출합니다.
	CTGExcelInterior(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CTGExcelInterior(const CTGExcelInterior& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 특성
public:

	// 작업
public:


	// IAddIns 메서드
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
	LPDISPATCH(Add)(LPCTSTR Filename, VARIANT& CopyFile, LPDISPATCH * * RHS)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_BSTR VTS_VARIANT VTS_PDISPATCH ;
		InvokeHelper(0xb5, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, Filename, &CopyFile, RHS);
		return result;
	}
	long get_Count()
	{
		long result;
		InvokeHelper(0x76, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Item(VARIANT& Index)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0xaa, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms, &Index);
		return result;
	}
	LPUNKNOWN get__NewEnum()
	{
		LPUNKNOWN result;
		InvokeHelper(0xfffffffc, DISPATCH_PROPERTYGET, VT_UNKNOWN, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get__Default(VARIANT& Index)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms, &Index);
		return result;
	}

	void put_Color(COLORREF nColor, int nPattern)
	{
		long color = nColor;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x63, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, color);
		InvokeHelper(0x5f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, nPattern);
	}

	// IAddIns 속성
public:

};

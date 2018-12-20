// 
// CGxCallback : Class간 Callback 함수를 지원하기 위한 Templte 및 class
// 본 Class는 Template기능을 사용하기 위해 별도로 DLL에 편입 시키지 않고 소스형태 그대로 제공된다.
//
//////////////////////////////////////////////////////////////////////
#pragma once



// abstract base class
// [alones] 멤버 함수가 모두 순수 가상 함수 (pure virtual function)
class CGxCallback
{
public:
   // 멤버 함수를 호출할 두 가지 가능한 함수들
   // 가상 함수들을 통해서 오브젝트에 대한 포인터와 멤버 함수에 대한 
   // 포인터를 사용할 상속 받은 클래스들이 함수 콜을 쓸 수 있다.
   virtual void operator()(void* pValue)	= 0;		// 연사자를 이용한 호출
   virtual void Call(void* pValue)			= 0;     // 함수를 이용한 호출
};

//-------------------------------------------------------------------------------

// 상속 받은 템플릿 클래스
template <class TClass> class TCallbackDefine : public CGxCallback
{
private:
   void (TClass::*fpt)(void* );			// 멤버 함수에 대한 포인터
   TClass* pt2Object;                  // 오브젝트에 대한 포인터

public:
   // 생성자 - 오브젝트에 대한 포인터와 멤버 함수에 대한 포인터를 취하며 
   //          그 것들을 두 개의 prviate 변수에 저장한다.
   TCallbackDefine(TClass* _pt2Object, void(TClass::*_fpt)(void*))		{ pt2Object = _pt2Object;  fpt=_fpt; };

	TCallbackDefine()	{};
	void SetFunction(TClass* _pt2Object, void(TClass::*_fpt)(void*))	{ pt2Object = _pt2Object;  fpt=_fpt; };

   // 연산자 "()" 재정의
   virtual void operator()(void* pValue)	{ (*pt2Object.*fpt)(pValue);};              // 멤버 함수 수행

   // "Call" 함수 재정의 
   virtual void Call(void* pValue)			{ (*pt2Object.*fpt)(pValue);};             // 멤버 함수 수행
}; //of template 


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// CString 형을 되돌려주는 Call Back 함수, 
// Motion의 Sefty Interlock 구현부분에서 응용 함.
class CGxCallbackStr
{
public:
   // 멤버 함수를 호출할 두 가지 가능한 함수들
   // 가상 함수들을 통해서 오브젝트에 대한 포인터와 멤버 함수에 대한 
   // 포인터를 사용할 상속 받은 클래스들이 함수 콜을 쓸 수 있다.
   virtual CString operator()(void* pValue)		= 0;		// 연사자를 이용한 호출
   virtual CString Call(void* pValue)			= 0;     // 함수를 이용한 호출
};

//-------------------------------------------------------------------------------

// 상속 받은 템플릿 클래스
template <class TClass> class TCallbackStrDefine : public CGxCallbackStr
{
private:
   CString (TClass::*fpt)(void* );			// 멤버 함수에 대한 포인터
   TClass* pt2Object;                  // 오브젝트에 대한 포인터

public:
   // 생성자 - 오브젝트에 대한 포인터와 멤버 함수에 대한 포인터를 취하며 
   //          그 것들을 두 개의 prviate 변수에 저장한다.
   TCallbackStrDefine(TClass* _pt2Object, CString(TClass::*_fpt)(void*))	{ pt2Object = _pt2Object;  fpt=_fpt; };

	TCallbackStrDefine()	{};
	void SetFunction(TClass* _pt2Object, CString(TClass::*_fpt)(void*))		{ pt2Object = _pt2Object;  fpt=_fpt; };

   // 연산자 "()" 재정의
   virtual CString operator()(void* pValue)	{ return (*pt2Object.*fpt)(pValue);};              // 멤버 함수 수행

   // "Call" 함수 재정의 
   virtual CString Call(void* pValue)		{ return (*pt2Object.*fpt)(pValue);};             // 멤버 함수 수행
}; //of template 



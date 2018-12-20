/////////////////////////////////////////////////////////////////////////////////////
//
// CGxSingleton : Template 기반의 Singleton class
// 본 Class는 Template기능을 사용하기 위해 별도로 DLL에 편입 시키지 않고 소스형태 그대로 제공된다.
//
// #2014-12-23, jhLee
//
/////////////////////////////////////////////////////////////////////////////////////
#pragma once


template<typename T>
class CGxSingleton
{
public:
  static T* GetInstance();				// Instance를 획득, 존재하지 않는다면 생성 후 포인터를 되돌려준다.
  static void ReleaseInstance();		// 생성된 Instance를 파괴한다.
  
private:
  CGxSingleton(CGxSingleton const&) {};
//  CGxSingleton& operator=(CGxSingleton const&) {};

protected:
	static T* m_theInstance;

	CGxSingleton() {};// m_theInstance = static_cast <T*> (this); };
	virtual ~CGxSingleton() {};

};
  
template<typename T>
typename T* CGxSingleton<T>::m_theInstance = NULL;

template<typename T>
T* CGxSingleton<T>::GetInstance()
{
	if ( CGxSingleton<T>::m_theInstance == NULL )
	{
		CGxSingleton<T>::m_theInstance = new T();
	}
	return CGxSingleton<T>::m_theInstance;
}

template<typename T>
void CGxSingleton<T>::ReleaseInstance()
{
	if ( CGxSingleton<T>::m_theInstance != NULL )
	{
		delete CGxSingleton<T>::m_theInstance;
		CGxSingleton<T>::m_theInstance = NULL;
	}
}


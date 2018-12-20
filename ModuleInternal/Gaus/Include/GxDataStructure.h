//
// 자료구조 선언
//
// GxStack
// GxQueue
//


#pragma once


//=============================================================================
// STACK
//=============================================================================

template< class TYPE, class ARG_TYPE > 
class CGxStack : public CList< TYPE, ARG_TYPE >
{
public:
	TYPE Pop();
	BOOL Push(ARG_TYPE newElement);
	TYPE Top();
};

//-----------------------------------------------------------------------------

template< class TYPE, class ARG_TYPE > 
TYPE CGxStack<TYPE, ARG_TYPE>::Pop()
{ 
	return RemoveHead( ); 
}

template< class TYPE, class ARG_TYPE > 
BOOL CGxStack<TYPE, ARG_TYPE>::Push(ARG_TYPE newElement) 
{ 
	return ( NULL != AddHead( newElement ) ); 
}

template< class TYPE, class ARG_TYPE > 
TYPE CGxStack<TYPE, ARG_TYPE>::Top() 
{ 
	return GetHead( ); 
}



//=============================================================================
// QUEUE
//=============================================================================

template< class TYPE, class ARG_TYPE > 
class CGxQueue : public CList< TYPE, ARG_TYPE >
{
public:
	TYPE Back();
	TYPE Front();
	TYPE Pop();
	BOOL Push(ARG_TYPE newElement);
};

//-----------------------------------------------------------------------------

template< class TYPE, class ARG_TYPE > 
TYPE CGxQueue<TYPE, ARG_TYPE>::Back() 
{ 
	return GetTail(); 
}

template< class TYPE, class ARG_TYPE > 
TYPE CGxQueue<TYPE, ARG_TYPE>::Front() 
{ 
	return GetHead(); 
}

template< class TYPE, class ARG_TYPE > 
TYPE CGxQueue<TYPE, ARG_TYPE>::Pop() 
{ 
	return RemoveHead(); 
}

template< class TYPE, class ARG_TYPE > 
BOOL CGxQueue<TYPE, ARG_TYPE>::Push(ARG_TYPE newElement) 
{ 
	return ( NULL != AddTail( newElement ) ); 
}


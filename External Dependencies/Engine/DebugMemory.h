#ifndef DEBUGMEMORY_H
#define DEBUGMEMORY_H
#include <crtdbg.h>

#ifdef _DEBUG
   #define DEBUG_NEW   new( _CLIENT_BLOCK, __FILE__, __LINE__)

   #define new DEBUG_NEW

#endif // _DEBUG

#endif

//if(_heapchk() != _HEAPOK)
//{
//	_HEAPINFO info;
//	info._pentry = NULL;
//	while(_heapwalk(&info) == _HEAPOK)
//	{
//
//	}
//	char buffer[40];
//	sprintf(buffer,"Bad lock at: %p",info._pentry);
//	outputDebugStringA(buffer);
//}
//
//
//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
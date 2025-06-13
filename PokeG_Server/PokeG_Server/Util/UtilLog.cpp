#include "../stdafx.h"
#include "UtilLog.h"

// 가변인자
#include <cstdarg>
#include <iostream>

void UtilLog::ErrorDisplay(int ErrorCode)
{
#ifdef _DEBUG
	WCHAR* lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, ErrorCode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, 0);
	std::wcout << lpMsgBuf << std::endl;
	//while (true);
	LocalFree(lpMsgBuf);
#endif
}

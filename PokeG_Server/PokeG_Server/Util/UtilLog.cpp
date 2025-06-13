#include "../stdafx.h"
#include "UtilLog.h"

// ��������
#include <cstdarg>
#include <iostream>

template<typename Args>
void UtilLog::Log(const Args message)
{
	std::cerr << message << std::endl;
}

template<typename ...Args>
inline void UtilLog::Log(std::format_string<Args...> fmt, Args && ...args)
{
	// template<typename ...Args>
	// Args�� �Լ��� ������ ��� ������ Ÿ�� ����� ����

	// std::format_string<Args...> fmt
	// fmt�� ���� ���ڿ��� ����,
	// Args...�� �´� {}�ڸ��� ��Ȯ�� ��Ī�Ǵ��� ������ Ÿ�ӿ� �˻�

	// Args&&... args
	// ���޹��� ���� ���ڵ�,
	// �Ϻ� ����(perfect forwarding)�� ���� &&�� �Բ� std::forward�� ���

	// std::format(fmt, std::forward<Args>(args)...)
	// std::format�� C++20���� �����Ǵ� ������ ���̺귯��
#ifdef _DEBUG
	std::cerr<< std::format(fmt, std::forward<Args>(args)...) << std::endl;
#endif
}

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

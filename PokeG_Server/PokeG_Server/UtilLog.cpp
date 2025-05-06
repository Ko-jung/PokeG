#include "UtilLog.h"

// ��������
#include <cstdarg>

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

	std::cerr<< std::format(fmt, std::forward<Args>(args)...) << std::endl;
}
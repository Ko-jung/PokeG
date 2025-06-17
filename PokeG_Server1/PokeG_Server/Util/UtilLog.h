#pragma once
#include <iostream>

#include <string>
#include <format>

class UtilLog
{
public:
	template<typename Args>
	static void Log(const Args& message)
	{
		std::cerr << message << std::endl;
	}

	template<typename... Args>
	static void Log(std::format_string<Args...> fmt, Args&&... args)
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
		std::cerr << std::format(fmt, std::forward<Args>(args)...) << std::endl;
#endif
	}

	static void ErrorDisplay(int ErrorCode);
};
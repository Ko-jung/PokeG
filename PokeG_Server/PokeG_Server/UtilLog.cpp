#include "UtilLog.h"

// 가변인자
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
	// Args는 함수에 들어오는 모든 인자의 타입 목록을 담음

	// std::format_string<Args...> fmt
	// fmt는 포맷 문자열을 받음,
	// Args...에 맞는 {}자리가 정확히 매칭되는지 컴파일 타임에 검사

	// Args&&... args
	// 전달받은 실제 인자들,
	// 완벽 전달(perfect forwarding)을 위해 &&와 함께 std::forward를 사용

	// std::format(fmt, std::forward<Args>(args)...)
	// std::format은 C++20부터 지원되는 포맷팅 라이브러리

	std::cerr<< std::format(fmt, std::forward<Args>(args)...) << std::endl;
}
#include <string>
#include <format>

class UtilLog
{
public:
	template<typename Args>
	static void Log(const Args message);

	template<typename... Args>
	static void Log(std::format_string<Args...> fmt, Args&&... args);

	static void ErrorDisplay(int ErrorCode);
};

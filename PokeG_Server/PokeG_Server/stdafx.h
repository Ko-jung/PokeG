#pragma once
#include <WS2tcpip.h>
#include <MSWSock.h>
#include <WinSock2.h>
#pragma comment(lib, "MSWSock.lib")
#pragma comment(lib, "ws2_32.lib")

#include <thread>
#include <vector>
#include <iostream>
#include <unordered_set>
#include <mutex>
#include <cassert>

#include <unordered_map>
#include <functional>
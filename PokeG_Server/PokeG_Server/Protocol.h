#pragma once
constexpr int PORT_NUM = 4000;
constexpr int NAME_SIZE = 20;
constexpr int CHAT_SIZE = 300;

constexpr int MAX_USER = 30000;
constexpr int MAX_NPC = 2000;

constexpr int W_WIDTH = 2000;
constexpr int W_HEIGHT = 2000;

constexpr int SECTORSIZE = 8;
constexpr int VIEW_RANGE = 5;

constexpr int BOARDSIZE = 20;

// Packet ID
constexpr char CS_LOGIN = 0;
constexpr char CS_MOVE = 1;
constexpr char CS_CHAT = 2;
constexpr char CS_ATTACK = 3;			// 4 ���� ����
constexpr char CS_TELEPORT = 4;			// RANDOM�� ��ġ�� Teleport, Stress Test�� �� Hot Spot������ ���ϱ� ���� ����
constexpr char CS_LOGOUT = 5;			// Ŭ���̾�Ʈ���� ���������� ������ �����ϴ� ��Ŷ
constexpr char CS_8DIRECT_MOVE = 6;
constexpr char CS_STATE_CHANGE = 7;

constexpr char SC_LOGIN_INFO = 100;
constexpr char SC_LOGIN_FAIL = 101;
constexpr char SC_ADD_OBJECT = 102;
constexpr char SC_REMOVE_OBJECT = 103;
constexpr char SC_MOVE_OBJECT = 104;
constexpr char SC_CHAT = 105;
constexpr char SC_STAT_CHANGE = 106;
constexpr char SC_8DIRECT_MOVE_OBJECT = 107;
constexpr char SC_STATE_CHANGE = 108;
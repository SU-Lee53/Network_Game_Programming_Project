#pragma once
#include "MsgProtocol.h"
#include "Common.h"

class Rock;
class Player;
//////////////////////////////////////////////////////////////////////////////////////////////
// 2025.11.18
// CreateRock() By 민정원
// Rock생성후 플레이어 위치로 방향 설정
std::unique_ptr<Rock> CreateRock(const Player* Player);

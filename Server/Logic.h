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

//////////////////////////////////////////////////////////////////////////////////////////////
// 2025.11.25
// CheckRayIntersection() By 이승욱
// Rock 과 Player 의 Ray 간의 충돌 검사
void CheckRayIntersection();

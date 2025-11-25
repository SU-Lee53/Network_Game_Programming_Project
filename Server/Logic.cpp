#include "Logic.h"
#include "Rock.h"


std::unique_ptr<Rock> CreateRock(const CLIENT& PlayerPacket)
{
	std::unique_ptr<Rock> rock = std::make_unique<Rock>();
	auto PlayerTransform = PlayerPacket.transformData.mtxPlayerTransform;
	XMFLOAT3 PlayerPosition = XMFLOAT3(PlayerTransform._41, PlayerTransform._42, PlayerTransform._43);
	rock->SetDirection(PlayerPosition);
	return rock;
}

//////////////////////////////////////////////////////////////////////////////////////////////
// 2025.11.25
// CheckRayIntersection() By 이승욱
// Rock 과 Player 의 Ray 간의 충돌 검사
void CheckRayIntersection()
{
	
}

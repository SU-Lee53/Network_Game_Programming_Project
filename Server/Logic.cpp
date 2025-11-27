#include "Logic.h"
#include "Rock.h"
#include "Player.h"


std::unique_ptr<Rock> CreateRock(const Player* Player)
{
	std::unique_ptr<Rock> rock = std::make_unique<Rock>();
	auto PlayerTransform = Player->GetWorldMatrix();
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
	// TODO 구현
	// Ray 와 OBB 충돌검출 함수
	// BoundingOrientedBox::Intersects(XMVECTOR origin, XMVECTOR direction, float& dist);



}

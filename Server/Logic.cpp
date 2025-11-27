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

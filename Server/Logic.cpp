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

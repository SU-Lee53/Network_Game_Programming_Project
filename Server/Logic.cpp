#include "Logic.h"
#include "Rock.h"



void CreateRock(const CLIENT& PlayerPacket)
{
	auto rock = std::make_unique<Rock>();
	auto PlayerTransform = PlayerPacket.transformData.mtxPlayerTransform;
	XMFLOAT3 PlayerPosition = XMFLOAT3(PlayerTransform._41, PlayerTransform._42, PlayerTransform._43);
	rock->SetDirection(PlayerPosition);
}

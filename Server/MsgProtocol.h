#pragma once
#include "Common.h"

struct PlayerTransformData {
	XMFLOAT4X4 mtxPlayerTransofrm;
};

struct PlayerShotData {
	XMFLOAT3 v3RayPosition;
	XMFLOAT3 v3RayDirection;
};

struct RockData {
	XMFLOAT4X4 mtxRockTransform;
	BYTE nrockID;
	BYTE nIsAlive;
};

struct CLIENT {
	int id;
	PlayerTransformData transformData;
	PlayerShotData shotData;
	bool flag;
};

struct ServertoClientPlayerPacket {
	CLIENT client[CLIENT_NUM];
};

struct ServertoClientRockPacket {
	std::vector<RockData> rockData;
};

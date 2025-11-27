#pragma once
#include "Common.h"

struct PlayerTransformData {
	XMFLOAT4X4 mtxPlayerTransform;
};

struct PlayerShotData {
	XMFLOAT3 v3RayPosition;
	XMFLOAT3 v3RayDirection;
};

struct RockData {
	XMFLOAT4X4 mtxRockTransform;
	BYTE nrockID;
	bool nIsAlive;
};

struct StartPacket {
	int id;
	bool startFlag;
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
////////////
//11.27
//ServertoClientRockPacket 수정 by 민정원

struct ServertoClientRockPacket {
	std::array<RockData , 50> rockData;
	int size;
};

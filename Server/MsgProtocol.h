#include "Common.h"


struct PlayerTransformData {
	XMMATRIX mtxPlayerTransofrm;
};

struct PlayerShotData {
	XMFLOAT3 v3RayPosition;
	XMFLOAT3 v3RayDirection;
};

struct RockData {
	XMMATRIX mtxRockTransform;
	BYTE nrockID;
	BYTE nIsAlive;
};

struct CLIENT {
	int id;
	PlayerTransformData transformData;
	PlayerShotData shotData;
};

struct ServertoClientPlayerPacket {
	CLIENT client[CLIENT_NUM];
};

struct ServertoClientRockPacket {
	std::vector<RockData> rockData;
};

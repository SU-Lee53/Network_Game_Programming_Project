#pragma once

enum PACKET_TYPE {
	PACKET_TYPE_PLAYER_TRANSFORM,
	PACKET_TYPE_PLAYER_SHOT,
	/*...*/
};

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
	BYTE nIsAlive;
};

struct ClientToServerPacket {
	int id = 0;
	PlayerTransformData transformData;
	PlayerShotData shotData;
};

#define CLIENT_NUM 3

struct CLIENT {
	int id;
	PlayerTransformData transformData;
	PlayerShotData shotData;
};

struct ServertoClientPlayerPacket {
	CLIENT client[CLIENT_NUM];
};

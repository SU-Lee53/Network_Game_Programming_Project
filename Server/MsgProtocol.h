#include "Common.h"

#define MAX_ROCK_COUNT 10

enum PACKET_TYPE {
	PACKET_TYPE_PLAYER_TRANSFORM,
	PACKET_TYPE_PLAYER_SHOT,
	PACKET_TYPE_GAME_ROCK
};

struct CLIENT_ID {
	int id;
};

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

struct ServertoClientPacket {
	PACKET_TYPE ePacketType;
	CLIENT_ID client_id;
	union {
		PlayerTransformData transformData;
		PlayerShotData shotData;
		RockData rockData[MAX_ROCK_COUNT];
	};
};

#pragma once
#include "WinSockCommon.h"

#define SEPARATE_PACKET

#ifdef SEPARATE_PACKET
#pragma pack(0)
enum PACKET_TYPE : BYTE {
	PACKET_TYPE_PLAYER_TRANSFORM,
	PACKET_TYPE_PLAYER_SHOT,
	PACKET_TYPE_GAME_SYNCED
	/*...*/
};

struct PlayerTransformData {
	XMFLOAT4X4 mtxPlayerTransform;
};

struct PlayerShotData {
	XMFLOAT3 v3RayPosition;
	XMFLOAT3 v3RayDirection;
};

struct GameSyncData {
	BYTE nSynced;
};

struct ClientToServerPacket {
	PACKET_TYPE ePacketType;
	union {
		PlayerTransformData transformData;
		PlayerShotData shotData;
		GameSyncData syncData;
	};
};
#pragma pack()

#else
#pragma pack(0)
struct ClientToServerPacket {
	Matrix mtxPlayerTransform;
	bool bShot;
	Vector3 v3RayPosition;
	Vector3 v3RayDirection;
	BYTE nSynced;
};
#pragma pack()

#endif

struct ServerToClientPacket {

};

class NetworkManager {
public:
	void ConnectToServer();
	void Disconnect();

public:
	bool SendData(ClientToServerPacket* packet, int nPacket);
	bool ReceiveData(ServerToClientPacket& packet);

public:
	SOCKET m_hSocketToClient;
};

bool NetworkManager::SendData(ClientToServerPacket* packet, int nPacket)
{
	if (packet->ePacketType == PACKET_TYPE_PLAYER_TRANSFORM) {
		XMFLOAT4X4 data = packet->transformData.mtxPlayerTransform;
	}


	return true;
}

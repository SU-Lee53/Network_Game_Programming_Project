#pragma once
#define SERVERPORT 9000

enum PACKET_TYPE  {
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

class NetworkManager {
public:
	NetworkManager();

public:
	void ConnectToServer();
	void Disconnect();

public:
	bool SendData(ClientToServerPacket* packet, int nPacket);
	bool ReceiveData(ServertoClientPlayerPacket& packet);


public:
	// 2025.11.03 
	// by 이승욱
	WSADATA m_wsa;
	SOCKET m_hClientSocket;
	char m_cstrServerIP[16] = "000.000.000.000";
	bool m_bConnected = false;
	std::string m_strErrorLog;


};

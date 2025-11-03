#pragma once
#define SERVERPORT 9000

enum PACKET_TYPE : BYTE {
	PACKET_TYPE_PLAYER_TRANSFORM,
	PACKET_TYPE_PLAYER_SHOT,
	/*...*/
};

#pragma pack(0)
struct PlayerTransformData {
	XMFLOAT4X4 mtxPlayerTransform;
};

struct PlayerShotData {
	XMFLOAT3 v3RayPosition;
	XMFLOAT3 v3RayDirection;
};


struct ClientToServerPacket {
	PACKET_TYPE ePacketType;
	union {
		PlayerTransformData transformData;
		PlayerShotData shotData;
	};
};
#pragma pack()


struct ServerToClientPacket {

};

class NetworkManager {
public:
	NetworkManager();

public:
	void ConnectToServer();
	void Disconnect();

public:
	bool SendData(ClientToServerPacket* packet, int nPacket);
	bool ReceiveData(ServerToClientPacket& packet);

public:
	// 2025.11.03 
	// by 이승욱
	WSADATA m_wsa;
	SOCKET m_hClientSocket;
	char m_cstrServerIP[16] = "000.000.000.000";
	bool m_bConnected = false;
	std::string m_strErrorLog;
};

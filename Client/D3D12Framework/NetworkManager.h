#pragma once
#define SERVERPORT 9000

// 11.10 이승욱
// Packet 선언 Packets 로 이동

class NetworkManager {
public:
	NetworkManager();

public:
	void ConnectToServer();
	void Disconnect();

public:
	bool SendData(ClientToServerPacket* packet, int nPacket);
	bool ReceiveData(ServertoClientPlayerPacket& packet);
	//void MakePacketToSend(ClientToServerPacket& packet ,const std::shared_ptr<Player> Player);

public:
	// 2025.11.03 
	// by 이승욱
	WSADATA m_wsa;
	SOCKET m_hClientSocket;
	char m_cstrServerIP[16] = "000.000.000.000";
	bool m_bConnected = false;
	std::string m_strErrorLog;
};

#pragma once
#define SERVERPORT 9000

// 11.10 이승욱
// Packet 선언 Packets 로 이동

class NetworkManager {
public:
	NetworkManager();

public:
	void ConnectToServer();

private:
	void Disconnect();

	bool SendData();
	bool ReceiveData();
	//void MakePacketToSend(ClientToServerPacket& packet ,const std::shared_ptr<Player> Player);

public:
	// 2025.11.19
	// by 이승욱
	void WritePacketData(const ClientToServerPacket& packet);
	ServertoClientPlayerPacket GetReceivedPacketData();
	// 2025.11.27
	// by 이승욱
	ServertoClientRockPacket GetReceivedRockPacketData();
	bool IsConnected() { return m_bConnected; }
	bool IsGameStarted() { return m_bGameBegin; }

private:
	// 2025.11.03 
	// by 이승욱
	WSADATA m_wsa;
	SOCKET m_hClientSocket;
	char m_cstrServerIP[16] = "127.0.0.1";
	bool m_bConnected = false;
	std::string m_strErrorLog;
	int m_nPlayerID;

	// 2025.11.16
	// by 이승욱
	HANDLE g_hNetworkThread;
	static DWORD WINAPI ProcessNetwork(LPVOID arg);

	// 2025.11.19
	// by 이승욱
	static CRITICAL_SECTION g_hCS;	// 이벤트로 순서제어 중이므로 사용 안될듯함 (아직 모름)
	static HANDLE g_hPlayerWritePacketEvent;
	static HANDLE g_hPacketReceivedEvent;

	ClientToServerPacket m_PacketToSend{};
	ServertoClientPlayerPacket m_PacketReceived{};

	// 2025.11.27
	// by 민정원
	ServertoClientRockPacket m_PacketRocksReceived{};

	bool m_bGameBegin = false;

public:
};

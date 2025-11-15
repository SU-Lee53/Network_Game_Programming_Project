#include "pch.h"
#include "NetworkManager.h"
#include "TestScene.h"
#include "SpaceshipPlayer.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////
// 2025.11.03
// NetworkManager() By 이승욱
// ImGui 를 이용하여 IP 를 입력받고 연결을 시도

NetworkManager::NetworkManager()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		__debugbreak();
		PostQuitMessage(0);
	}

	m_hClientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (m_hClientSocket == INVALID_SOCKET) {
		__debugbreak();
		PostQuitMessage(0);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// 2025.11.03
// ConnectToServer() By 이승욱
// ImGui 를 이용하여 IP 를 입력받고 연결을 시도
// 
// 2025.11.04
// Local 서버를 이용한 테스트 완료

void NetworkManager::ConnectToServer()
{
	ImGui::Begin("NetworkManager::ConnectToServer()");
	{
		const char* cstrConnectionStatus = m_bConnected ? "Connected" : "Not connected yet";
		ImGui::Text(cstrConnectionStatus);
		ImGui::InputText("Server IP", m_cstrServerIP, IM_ARRAYSIZE(m_cstrServerIP));

		if (ImGui::Button("Try Connect")) {
			sockaddr_in serveraddr;
			memset(&serveraddr, 0, sizeof(serveraddr));
			serveraddr.sin_family = AF_INET;
			inet_pton(AF_INET, m_cstrServerIP, &serveraddr.sin_addr);
			serveraddr.sin_port = htons(SERVERPORT);
			int retval = connect(m_hClientSocket, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
			if (retval == SOCKET_ERROR) {
				m_strErrorLog = err_display("socket()");
			}
			else {
				m_bConnected = true;
			}
		}
		ImGui::Text(m_strErrorLog.c_str());

		// 하지 말것
		//if (m_bConnected) {
		//	if (ImGui::Button("Start")) {
		//		SCENE->ChangeScene<TestScene>();
		//	}
		//}

	}
	ImGui::End();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// 2025.11.04
// SendData() By 이승욱
// 서버로 ClientToServerPacket 을 전송
// 전송하기 전에 몇개의 패킷을 보낼지 먼저 알려줘야 함
// return value : recv() 의 retval 을 이용한 성공/실패 여부
// 
// 2025.11.04
// Local 서버를 이용한 테스트 완료
//
// 2025.11.14
// 패킷 변경에 따라 Send 방식 변경


bool NetworkManager::SendData(const ClientToServerPacket& packet)
{
	assert(m_bConnected);
	int nBytesToSend = sizeof(ClientToServerPacket);
	int retval{};
	int nSumOfRetval{};

	retval += send(m_hClientSocket, (char*)&packet, sizeof(ClientToServerPacket), 0);
	if (retval == SOCKET_ERROR) {
		m_strErrorLog = err_display("send()");
	}
	else {
		nSumOfRetval += retval;
	}

	ImGui::Text(m_strErrorLog.c_str());		// TODO : 확인 후 지울것

	return nSumOfRetval == nBytesToSend;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// 2025.11.06
// Disconnect() By 민정원
// 연결해제

void NetworkManager::Disconnect()
{
	m_bConnected = false;
	closesocket(m_hClientSocket);
	WSACleanup();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// 2025.11.04
// ReceiveData By 이승욱
// 테스트용 임시

bool NetworkManager::ReceiveData(ServertoClientPlayerPacket& packet)
{
	int retval = 0;
	retval = recv(m_hClientSocket, (char*)&packet, sizeof(ServertoClientPlayerPacket), 0);

	return retval == sizeof(ServertoClientPlayerPacket);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// 2025.11.11
// MakePacketToSend() By 민정원
// 서버로 보낼 패킷 생성.


/////////////////////////////////////////////////////////////////////////////////////////////////////////
// 2025.11.14
// MakePacketToSend() By 민정원
// player에서 다시 구현.

//void NetworkManager::MakePacketToSend(ClientToServerPacket& packet , std::shared_ptr<Player> Player)
//{
//	packet.id = 999;
//	packet.transformData.mtxPlayerTransform = Player->GetTransform().GetWorldMatrix();
//	packet.shotData.v3RayDirection = static_pointer_cast<SpaceshipPlayer>(Player)->GetRayDirection();
//	packet.shotData.v3RayPosition = static_pointer_cast<SpaceshipPlayer>(Player)->GetRayPos();
//}

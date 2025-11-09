#include "pch.h"
#include "NetworkManager.h"
#include "TestScene.h"


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

bool NetworkManager::SendData(ClientToServerPacket* packet, int nPacket)
{
	assert(m_bConnected);
	int nBytesToSend = sizeof(ClientToServerPacket) * nPacket;	// 나중에 retval 의 합이 이것과 같으면 true 임
	int retval{};
	int nSumOfRetval{};

	// 전송할 패킷 갯수를 먼저 보냄
	// 1개라면 Transform  정보만 보낸것
	// 2개라면 Transform + Ray 정보까지 보낸 것
	retval = send(m_hClientSocket, (char*)&nPacket, sizeof(int), 0);
	if (retval == SOCKET_ERROR) {
		m_strErrorLog = err_display("send()");
	}
	else {
		nSumOfRetval += retval;
	}


	for (int i = 0; i < nPacket; ++i) {
		retval += send(m_hClientSocket, (char*)&packet[i], sizeof(ClientToServerPacket), 0);	
		if (retval == SOCKET_ERROR) {
			m_strErrorLog = err_display("send()");
		}
		else {
			nSumOfRetval += retval;
		}

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

bool NetworkManager::ReceiveData(ServerToClientPacket& packet)
{
	return false;
}

#include "pch.h"
#include "NetworkManager.h"


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

	}
	ImGui::End();
}

bool NetworkManager::SendData(ClientToServerPacket* packet, int nPacket)
{
	return false;
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

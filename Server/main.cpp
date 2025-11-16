#include "Common.h"
#include "MsgProtocol.h"
#include "Logic.h"

#define SERVERPORT 9000
#define BUFSIZE    512

CRITICAL_SECTION cs;

int client_count = 0;

std::unordered_map<int, std::queue<CLIENT>> PlayerStates;
ServertoClientPlayerPacket SendPlayerPacket;
ServertoClientRockPacket SendRockPacket;

//////////////////////////////////////////////////////////////////////////////////////////////
// 2025.11.12
// ProcessClient() By 최명준
// recv() 방식 재정의
// send() 방식 재정의
// send를 통해 client쪽으로 보내는 패킷안에 이게 어떤 플레이어 데이터인지
// id를 통해 보내는 패킷마다 늘 확인할 수 있음
// 지금 방식은 recv한번하고 데이터 취합한 다음 send하는 방식
// 느리거나 프레임 끊길시 구조 조정

DWORD WINAPI ProcessClient(LPVOID arg)
{
	int retval;
	SOCKET client_sock = (SOCKET)arg;
	struct sockaddr_in clientaddr;
	char addr[INET_ADDRSTRLEN];
	int addrlen;
	int len;
	int client_id = (int)arg;
	int client_num = 0;

	// 클라이언트 정보 얻기
	addrlen = sizeof(clientaddr);

	// 소켓에서 주소정보 얻기
	getpeername(client_sock, (struct sockaddr*)&clientaddr, &addrlen);
	inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));

	CLIENT recvPacket;

	EnterCriticalSection(&cs);
	client_num = client_count;
	client_count++;
	LeaveCriticalSection(&cs);
	recvPacket.flag = false;
	while (true)
	{
		retval = recv(client_sock, (char*)&recvPacket, sizeof(CLIENT), 0);
		recvPacket.id = client_id;
		
		recvPacket.flag = false;
		SendPlayerPacket.client[client_num] = recvPacket;
		SendPlayerPacket.client[client_num].flag = true;

		if (SendPlayerPacket.client[0].flag == true &&
			SendPlayerPacket.client[1].flag == true &&
			SendPlayerPacket.client[2].flag == true)
		{
			retval = send(client_sock, (char*)&SendPlayerPacket, sizeof(SendPlayerPacket), 0);
		}
	}

	//소켓 닫기
	closesocket(client_sock);
}

//////////////////////////////////////////////////////////////////////////////////////////////
// 2025.11.10
// main() By 최명준
// main에서 send하는 방식 취소
// 일단은 main에서 클라이언트 연결만 하는 방식
// Logic Loop는 잠시 비워둠

int main(int argc, char* argv[])
{
	InitializeCriticalSection(&cs);

	int retval;
	WSADATA wsa;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET)
		err_quit("socket()");

	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);

	retval = bind(listen_sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR)
		err_quit("bind()");

	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR)
		err_quit("listen()");

	SOCKET client_sock;
	struct sockaddr_in clientaddr;
	int addrlen;
	int len;
	HANDLE hThread;

	for (int i = 0; i < CLIENT_NUM; ++i)
	{
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (struct sockaddr*)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET)
		{
			err_display("accept()");
			break;
		}

		hThread = CreateThread(NULL, 0, ProcessClient, (LPVOID)client_sock, 0, NULL);
		if (hThread == NULL)
		{
			closesocket(client_sock);
		}
		else
		{
			CloseHandle(hThread);
		}
	}


	// Logich Loop
	while (true)
	{
	}

	DeleteCriticalSection(&cs);

	closesocket(listen_sock);

	WSACleanup();

	return 0;
}

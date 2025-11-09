#include "Common.h"
#include "MsgProtocol.h"
#include "Logic.h"

#define SERVERPORT 9000
#define BUFSIZE    512

CRITICAL_SECTION cs;

int client_count = 0;
int client_id[CLIENT_NUM];

std::unordered_map<int, std::queue<CLIENT>> PlayerStates;
//std::queue<std::shared_ptr<ServertoClientPlayerPacket>> RecvPlayerPacket;
//std::queue<std::shared_ptr<ServertoClientPlayerPacket>> SendPlayerPacket;
ServertoClientRockPacket SendRockPacket;

//////////////////////////////////////////////////////////////////////////////////////////////
// 2025.11.06
// ProcessClient() By 최명준
// recv() 방식 정의
// send() 방식 추후 개선 필요 (안전하지 않아보임)

DWORD WINAPI ProcessClient(LPVOID arg)
{
	int retval;
	SOCKET client_sock = (SOCKET)arg;
	struct sockaddr_in clientaddr;
	char addr[INET_ADDRSTRLEN];
	int addrlen;
	int len;
	int c_num;

	// 클라이언트 정보 얻기
	addrlen = sizeof(clientaddr);

	// 소켓에서 주소정보 얻기
	getpeername(client_sock, (struct sockaddr*)&clientaddr, &addrlen);
	inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));

	CLIENT recvPacket;

	EnterCriticalSection(&cs);
	client_id[client_count] = (int)arg;
	recvPacket.id = client_id[client_count];
	c_num = client_count;
	client_count++;
	LeaveCriticalSection(&cs);

	while (true)
	{
		retval = recv(client_sock, (char*)&recvPacket, sizeof(CLIENT), 0);
		EnterCriticalSection(&cs);
		PlayerStates[client_id[c_num]].push(recvPacket);
		LeaveCriticalSection(&cs);
	}

	//소켓 닫기
	closesocket(client_sock);
}

//////////////////////////////////////////////////////////////////////////////////////////////
// 2025.11.06
// main() By 최명준
// Test용 Logic Loop 추가

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

	SOCKET client_sock[CLIENT_NUM];
	struct sockaddr_in clientaddr;
	int addrlen;
	int len;
	HANDLE hThread;

	for (int i = 0; i < CLIENT_NUM; ++i)
	{
		addrlen = sizeof(clientaddr);
		client_sock[i] = accept(listen_sock, (struct sockaddr*)&clientaddr, &addrlen);
		if (client_sock[i] == INVALID_SOCKET)
		{
			err_display("accept()");
			break;
		}

		hThread = CreateThread(NULL, 0, ProcessClient, (LPVOID)client_sock[i], 0, NULL);
		if (hThread == NULL)
		{
			closesocket(client_sock[i]);
		}
		else
		{
			CloseHandle(hThread);
		}
	}


	// Logich Loop
	while (true)
	{
		ServertoClientPlayerPacket packet;

		// TODO: Send() 전에 패킷안에 보낼 데이터 다 들어왔는지 확인하는 로직 필요
		for (int i = 0; i < 3; ++i)
		{
			EnterCriticalSection(&cs);
			packet.client[i] = PlayerStates[client_id[i]].front();
			PlayerStates[client_id[i]].pop();
			LeaveCriticalSection(&cs);
		}

		for (int i = 0; i < 3; ++i)
		{
			EnterCriticalSection(&cs);
			send(client_sock[i], (char*)&packet, sizeof(packet), 0);
			LeaveCriticalSection(&cs);
		}
	}

	DeleteCriticalSection(&cs);

	closesocket(listen_sock);

	WSACleanup();

	return 0;
}

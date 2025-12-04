#include "Common.h"
#include "MsgProtocol.h"
#include "Logic.h"
#include "Rock.h"
#include "Player.h"
#include "ObjectCollections.h"

#define SERVERPORT 9000
#define BUFSIZE    512

CRITICAL_SECTION cs;

int client_count = 0;

ServertoClientPlayerPacket SendPlayerPacket;
ServertoClientRockPacket SendRockPacket;
HANDLE hSendEvent;
HANDLE hStartEvent;
HANDLE hRecvEvent;
HANDLE hLogicStartEvent;
HANDLE hLogicEndEvent;
int readyCount = 0;
int sendCount = 0;

std::uniform_int_distribution<int> uid(0, 2);



//////////////////////////////////////////////////////////////////////////////////////////////
// 2025.11.18
// vector<std::unique_ptr<Rock>> By 민정원
// Rock 담아둘 벡터 정의
//////////////////////////////////////////////////////////////////////////////////////////////
// 2025.11.20
// array<std::unique_ptr<Player> , 3> By 민정원
// Rock 담아둘 벡터 정의
//////////////////////////////////////////////////////////////////////////////////////////////
// 2025.11.26
// std::list<std::unique_ptr<Rock>> By 민정원
// Rock 담아둘 벡터 list로 변경

// 11.28 이승욱
// ObjectCollections.h 로 일단 이동
//std::list<std::unique_ptr<Rock>>			Rocks;
//std::array<std::unique_ptr<Player> , 3>		Players;
//int RockIndex = 0;


//////////////////////////////////////////////////////////////////////////////////////////////
// 2025.11.12
// ProcessClient() By 최명준
// recv() 방식 재정의
// send() 방식 재정의
// send를 통해 client쪽으로 보내는 패킷안에 이게 어떤 플레이어 데이터인지
// id를 통해 보내는 패킷마다 늘 확인할 수 있음
// 지금 방식은 recv한번하고 데이터 취합한 다음 send하는 방식
// 느리거나 프레임 끊길시 구조 조정

//////////////////////////////////////////////////////////////////////////////////////////////
// 2025.11.15
// ProcessClient() By 민정원
// 마지막 클라이언트 제외한 클라이언트에서 데드락걸리는 현상 수정
// flag의 사용여부는 생각을 해봐야함
// 개수를 체크해서 동기화에 사용

//////////////////////////////////////////////////////////////////////////////////////////////
// 2025.11.16
// ProcessClient() By 최명준
// 임계영역 구분 Update
// Client_id 먼저 client들에게 send 후 while() 접근
// 시작 전 client별 아이디와 게임 시작 신호 패킷 전송
// 
//////////////////////////////////////////////////////////////////////////////////////////////
// 2025.11.25
// 보낼때 Rock 데이터도 같이 보냄.
//
//////////////////////////////////////////////////////////////////////////////////////////////
// 2025.12.02
// 보낼 때 Player의 hp, alive 정보도 같이 보냄.

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
	StartPacket startPacket;

	EnterCriticalSection(&cs);
	client_num = client_count;
	client_count++;
	LeaveCriticalSection(&cs);

	if (client_count == 3)
	{
		SetEvent(hStartEvent);  // 이벤트 시그널
	}

	WaitForSingleObject(hStartEvent, INFINITE);

	// 시작 전 client별 아이디와 게임 시작 신호 전송
	// 클라이언트 측에선 받은 패킷 중 Flag가 true면 바로 게임 시작	

	startPacket.startFlag = true;

	startPacket.id = client_id;

	retval = send(client_sock, (char*)&startPacket, sizeof(StartPacket), 0);


	while (true)
	{
		WaitForSingleObject(hRecvEvent, INFINITE);
		retval = recv(client_sock, (char*)&recvPacket, sizeof(CLIENT), 0);
		recvPacket.id = client_id;

		recvPacket.flag = false;

		SendPlayerPacket.client[client_num] = recvPacket;
		//SendPlayerPacket.client[client_num].shotData = recvPacket.;
		SendPlayerPacket.client[client_num].flag = true;
		Players[client_num].get()->SetWorldMatrix(SendPlayerPacket.client[client_num].transformData.mtxPlayerTransform);
		Ray receivedRay;
		receivedRay.xmf3RayStartPosition = SendPlayerPacket.client[client_num].shotData.v3RayPosition;
		receivedRay.xmf3RayDirection = SendPlayerPacket.client[client_num].shotData.v3RayDirection;
		Players[client_num]->SetRayData(receivedRay);
		Players[client_num]->SetInformData(
			recvPacket.informData.hp,
			recvPacket.informData.score,
			recvPacket.informData.alive,
			recvPacket.informData.invincible
		);

		EnterCriticalSection(&cs);
		readyCount++;
		LeaveCriticalSection(&cs);

		// 3명 모두 준비되었는지 확인
		if (readyCount == 3)
		{
			ResetEvent(hRecvEvent);
			SetEvent(hSendEvent);  // 이벤트 시그널
			SetEvent(hLogicStartEvent);
		}

		// 2단계: 모두 모일 때까지 대기
		WaitForSingleObject(hSendEvent, INFINITE);
		WaitForSingleObject(hLogicEndEvent, INFINITE);

		// 3단계: 데이터 전송
		retval = send(client_sock, (char*)&SendPlayerPacket, sizeof(SendPlayerPacket), 0);
		retval = send(client_sock, (char*)&SendRockPacket, sizeof(SendRockPacket), 0);

		// 4단계: 전송 완료 후 정리
		EnterCriticalSection(&cs);
		sendCount++;

		// 마지막 스레드가 리셋 및 다음 라운드 준비
		if (sendCount == CLIENT_NUM)
		{
			// 모든 flag 초기화
			for (int i = 0; i < CLIENT_NUM; i++)
				SendPlayerPacket.client[i].flag = false;

			readyCount = 0;
			sendCount = 0;
			ResetEvent(hSendEvent);  // 다음 라운드를 위해 리셋
			ResetEvent(hLogicEndEvent);
			SetEvent(hRecvEvent);
		}
		LeaveCriticalSection(&cs);
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
	hStartEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	hSendEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	hRecvEvent = CreateEvent(NULL, TRUE, TRUE, NULL);
	hLogicStartEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	hLogicEndEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	for (int i = 0; i < Players.size(); ++i) {
		Players[i] = std::make_unique<Player>();
	}
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

	//////////////////////////////////////////////////////////////////////////////////////////////
	// 2025.11.26
	// Rock 생성후 Send By 민정원
	// Client에서 Recv까지 확인.
	//////////////////////////////////////////////////////////////////////////////////////////////
	// 2025.11.28
	// 데드락 현상 수정 By 민정원
	//////////////////////////////////////////////////////////////////////////////////////////////
	// 2025.12.01
	// Rock::Update() By 민정원
	// Delta Time 계산 추가
	//////////////////////////////////////////////////////////////////////////////////////////////
	// 2025.12.01
	// 타이머 계산해서 돌생성으로 변경 By 민정원

	auto prevTime = std::chrono::high_resolution_clock::now();
	float deltaTime = 0.0f;
	float spawnTimer = 0.0f;  // Rock 생성 타이머

	while (true)
	{
		WaitForSingleObject(hLogicStartEvent, INFINITE);

		auto currTime = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> elapsed = currTime - prevTime;
		deltaTime = elapsed.count();
		deltaTime = std::min(deltaTime, 0.2f);
		prevTime = currTime;
		
		Rocks.remove_if([](const auto& rockPtr) {
			return !rockPtr->GetIsAlive() || rockPtr->IsOutOfBounds();
			});

		spawnTimer += deltaTime;
		if (spawnTimer >= 1.0f && Rocks.size() < 50) {
			Rocks.push_back(CreateRock(Players[uid(dre)].get()));
			spawnTimer = 0;
		}

		for (int i = 0; i < CLIENT_NUM; ++i)
		{
			Players[i]->Update(deltaTime);
		}

		for (auto& rockPtr : Rocks) {
			Rock* rock = rockPtr.get();
			bool IsAlive = rock->GetIsAlive();
			if (IsAlive) {
				rock->Update(deltaTime);
			}
		}

		CheckRayIntersection();
		CheckPlayerIntersection();
		CheckRockIntersection();

		int index = 0;
		for (auto& rockPtr : Rocks) {
			Rock* rock = rockPtr.get();
			bool IsAlive = rock->GetIsAlive();
			SendRockPacket.rockData[index].mtxRockTransform = rock->GetWorldMatrix();
			SendRockPacket.rockData[index].nIsAlive = IsAlive;
			SendRockPacket.rockData[index].nrockID = index;
			++index;
		}
		SendRockPacket.size = Rocks.size();

		for (int i = 0; i < CLIENT_NUM; ++i)
		{
			SendPlayerPacket.client[i].informData.alive = Players[i]->IsAlive();
			SendPlayerPacket.client[i].informData.hp = Players[i]->GetHP();
			SendPlayerPacket.client[i].informData.score = Players[i]->GetScore();
		}

		ResetEvent(hLogicStartEvent);
		SetEvent(hLogicEndEvent);
	}

	DeleteCriticalSection(&cs);

	closesocket(listen_sock);

	WSACleanup();

	return 0;
}

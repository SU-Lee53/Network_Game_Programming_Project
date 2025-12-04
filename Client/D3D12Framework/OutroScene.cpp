#include "pch.h"
#include "OutroScene.h"
#include "TestScene.h"

void OutroScene::BuildObjects()
{
	m_pPlayer = std::make_shared<Player>();

	std::shared_ptr<TexturedSprite> pTextureSprite = std::make_shared<TexturedSprite>("Closing", 0.f, 0.f, 1.0f, 1.0f, 0);
	std::shared_ptr<TextSprite> pOverSprite = std::make_shared<TextSprite>("GAME OVER", 0.3f, 0.0f, 0.7f, 0.2f, XMFLOAT4(1, 0, 0, 1), 1, true);
	std::shared_ptr<TextSprite> pRestartSprite = std::make_shared<TextSprite>("PRESS R TO RESPAWN", 0.2f, 0.8f, 0.8f, 1.f, XMFLOAT4(1, 0, 0, 1), 1, true);

	m_pSprites.push_back(pTextureSprite);
	m_pSprites.push_back(pOverSprite);
	m_pSprites.push_back(pRestartSprite);

	InitializeObjects();
}

void OutroScene::ProcessInput()
{
}

void OutroScene::Update()
{
	UpdateObjects();

	if (!NETWORK->IsOffline()) {
		Matrix mtxTransform = Matrix::Identity;
		float dX = RandomGenerator::GenerateRandomFloatInRange(-100.f, 100.f);
		float dY = RandomGenerator::GenerateRandomFloatInRange(-100.f, 100.f);
		float dZ = RandomGenerator::GenerateRandomFloatInRange(-100.f, 100.f);
		mtxTransform._41 = 99999.f + dX;
		mtxTransform._42 = 99999.f + dY;
		mtxTransform._43 = 99999.f + dZ;


		ClientToServerPacket packet;
		packet.id = 0;	// 일단 안보내도 됨
		packet.transformData.mtxPlayerTransform = mtxTransform;
		packet.shotData.v3RayPosition = Vector3(0.f, 0.f, 0.f);
		packet.shotData.v3RayDirection = Vector3(0.f, 0.f, 0.f);
		packet.informData.alive = false;
		packet.informData.hp = 0.f;
		packet.informData.score = 0.f;

		NETWORK->WritePacketData(packet);
	}

	SyncSceneWithServer();
	if (INPUT->GetButtonDown('R')) {
		SCENE->ChangeScene<TestScene>();
	}
}

void OutroScene::Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommansList)
{
	RenderObjects(pd3dCommansList);
}

void OutroScene::SyncSceneWithServer()
{
	// 데이터 한번 빼줘야 맞음(한번 보냈으니까)
	ServertoClientPlayerPacket receivedPacket = NETWORK->GetReceivedPacketData();
	ServertoClientRockPacket rockPacket = NETWORK->GetReceivedRockPacketData();

}

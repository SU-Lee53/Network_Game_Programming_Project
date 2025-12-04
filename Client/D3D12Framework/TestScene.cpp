#include "pch.h"
#include "TestScene.h"
#include "Transform.h"
#include "CubeObject.h"
#include "MercuryObject.h"
#include "VenusObject.h"
#include "EarthObject.h"
#include "MarsObject.h"
#include "SunObject.h"
#include "RockObject.h"
#include "SpaceshipPlayer.h"
#include "PlayerRenderer.h"
#include "OutroScene.h"
#include "Packets.h"

using namespace std::string_literals;

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// 2025.11.15
// InitializeOtherPlayers() By 이승욱
// 상대 Player 들은 SpaceshipPlayer::Initialize() 를 사용하면 모델 포인터가 겹치므로 안됨
// 따라서 별도의 Initialize 를 거침

void TestScene::InitializeOtherPlayers()
{
	int cnt{};
	for (auto& pOther : m_pOtherPlayers) {
		pOther = std::make_shared<SpaceshipPlayer>();
		std::shared_ptr<GameObject> pSpaceship = MODEL->Get("Spaceship")->CopyObject<GameObject>();	// 1
		pOther->SetChild(pSpaceship);

		auto& p = pOther->FindMeshedFrame("ship");
		p->GetMeshRenderer()->SetTexture(TEXTURE->Get("Spaceship_Diffuse"), 0, TEXTURE_TYPE_DIFFUSE);
		p->GetMeshRenderer()->SetTexture(TEXTURE->Get("Spaceship_Normal"), 0, TEXTURE_TYPE_NORMAL);
		static_pointer_cast<PlayerRenderer>(p->GetMeshRenderer())->SetPlayer(pOther);
	}
}

void TestScene::OnEnterScene()
{
	SOUND->Play("bgm");
}

void TestScene::OnLeaveScene()
{
}

void TestScene::BuildObjects()
{
	std::shared_ptr<CubeObject> pCube1 = std::make_shared<CubeObject>();
	pCube1->Initialize();
	//pCube1->GetTransform().SetPosition(15.f, 0.f, 30.f);
	//AddObject(pCube1);

	std::shared_ptr<GameObject> pObj1 = std::make_shared<GameObject>();
	pObj1->GetTransform().SetPosition(0.f, 0.f, 0.f);
	pObj1->SetChild(GameObject::CopyObject(pCube1));
	pObj1->GetTransform().Scale(0.2f);
	AddObject(pObj1);

	auto pMercury = std::make_shared<MercuryObject>();
	auto pVenus = std::make_shared<VenusObject>();
	auto pEarth = std::make_shared<EarthObject>();
	auto pMars = std::make_shared<MarsObject>();
	auto pSun = std::make_shared<SunObject>();
	AddObject(pMercury);
	AddObject(pVenus);
	AddObject(pEarth);
	AddObject(pMars);
	AddObject(pSun);

	m_pRockObj = std::make_shared<RockObject>();
	m_pRockObj->Initialize();

	m_pPlayer = std::make_shared<SpaceshipPlayer>();

	Vector2 v2ScreenCenter{ 0.5f, 0.5f };
	Vector2 v2CrosshairSize{ 0.03f, 0.03f };
	float fLeft = v2ScreenCenter.x - v2CrosshairSize.x;
	float fTop = v2ScreenCenter.y - v2CrosshairSize.y;
	float fRight = v2ScreenCenter.x + v2CrosshairSize.x;
	float fBottom = v2ScreenCenter.y + v2CrosshairSize.y;

	std::shared_ptr<TexturedSprite> pCrosshairSprite = std::make_shared<TexturedSprite>("Crosshair", fLeft, fTop, fRight, fBottom);
	m_pSprites.push_back(pCrosshairSprite);

	InitializeOtherPlayers();
	InitializeObjects();
	BuildLights();
}

void TestScene::BuildLights()
{
	m_pLights.resize(5);

	auto pSpotLight = std::make_shared<SpotLight>();
	pSpotLight->m_v3Position = m_pPlayer->GetTransform().GetPosition();
	pSpotLight->m_v3Direction = m_pPlayer->GetTransform().GetLook();
	pSpotLight->m_v4Ambient = Vector4(0.1f, 0.1f, 0.1f, 1.0f);
	pSpotLight->m_v4Diffuse = Vector4(0.5f, 0.5f, 0.5f, 1.0f);
	pSpotLight->m_v4Specular = Vector4(0.5f, 0.5f, 0.5f, 0.0f);
	pSpotLight->m_fAttenuation0 = 1.0f;
	pSpotLight->m_fAttenuation1 = 0.001f;
	pSpotLight->m_fAttenuation2 = 0.0001f;
	pSpotLight->m_fRange = 100.0f;
	pSpotLight->m_fFalloff = 8.0f;
	pSpotLight->m_fPhi = (float)cos(XMConvertToRadians(40.0f));
	pSpotLight->m_fTheta = (float)cos(XMConvertToRadians(20.0f));
	pSpotLight->m_bEnable = TRUE;
	m_pLights[0] = pSpotLight;

	auto pLight1 = std::make_shared<PointLight>();
	pLight1->m_v3Position = Vector3(-200, 0, -40);	// TODO : Sun의 위치로 수정
	pLight1->m_v4Ambient = Vector4(0.05f, 0.05f, 0.05f, 1.0f);
	pLight1->m_v4Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	pLight1->m_v4Specular = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	pLight1->m_fAttenuation0 = 1.0f;
	pLight1->m_fAttenuation1 = 0.001f;
	pLight1->m_fAttenuation2 = 0.0001f;
	pLight1->m_fRange = 1000.f;
	pLight1->m_bEnable = TRUE;
	m_pLights[1] = pLight1;

	auto pLight2 = std::make_shared<PointLight>();
	pLight2->m_v3Position = Vector3(0.7f, 0.2f, 2.0f);
	pLight2->m_v4Ambient = Vector4(0.05f, 0.05f, 0.05f, 1.0f);
	pLight2->m_v4Diffuse = Vector4(0.8f, 0.8f, 0.8f, 1.0f);
	pLight2->m_v4Specular = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	pLight2->m_fAttenuation0 = 1.0f;
	pLight2->m_fAttenuation1 = 0.001f;
	pLight2->m_fAttenuation2 = 0.0001f;
	pLight2->m_fRange = 1000.f;
	pLight2->m_bEnable = TRUE;
	m_pLights[2] = pLight2;

	auto pLight3 = std::make_shared<PointLight>();
	pLight3->m_v3Position = Vector3(2.3f, -3.3f, -4.0f);
	pLight3->m_v4Ambient = Vector4(0.05f, 0.05f, 0.05f, 1.0f);
	pLight3->m_v4Diffuse = Vector4(0.8f, 0.8f, 0.8f, 1.0f);
	pLight3->m_v4Specular = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	pLight3->m_fAttenuation0 = 1.0f;
	pLight3->m_fAttenuation1 = 0.001f;
	pLight3->m_fAttenuation2 = 0.0001f;
	pLight3->m_fRange = 1000.f;
	pLight3->m_bEnable = TRUE;
	m_pLights[3] = pLight3;

	auto pLight4 = std::make_shared<PointLight>();
	pLight4->m_v3Position = Vector3(0.0f, 0.0f, -3.0f);
	pLight4->m_v4Ambient = Vector4(0.05f, 0.05f, 0.05f, 1.0f);
	pLight4->m_v4Diffuse = Vector4(0.8f, 0.8f, 0.8f, 1.0f);
	pLight4->m_v4Specular = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	pLight4->m_fAttenuation0 = 1.0f;
	pLight4->m_fAttenuation1 = 0.001f;
	pLight4->m_fAttenuation2 = 0.0001f;
	pLight4->m_fRange = 1000.f;
	pLight4->m_bEnable = TRUE;
	m_pLights[4] = pLight4;

	//auto pDirectionalLight = std::make_shared<DirectionalLight>();
	//pDirectionalLight->m_v4Ambient = Vector4(0.3f, 0.3f, 0.3f, 1.0f);
	//pDirectionalLight->m_v4Diffuse = Vector4(0.8f, 0.8f, 0.8f, 1.0f);
	//pDirectionalLight->m_v4Specular = Vector4(0.4f, 0.4f, 0.4f, 0.0f);
	//pDirectionalLight->m_v3Direction = XMFLOAT3(-1.0f, 0.0f, 0.0f);
	//pDirectionalLight->m_bEnable = TRUE;
	//m_pLights[4] = pDirectionalLight;

}

void TestScene::Update()
{
	auto pPlayerLight = static_pointer_cast<SpotLight>(m_pLights[0]);
	pPlayerLight->m_v3Position = m_pPlayer->GetTransform().GetPosition();
	pPlayerLight->m_v3Direction = m_pPlayer->GetTransform().GetLook();


	// NETWORK TEST ZONE
	//ImGui::Begin("Test");
	//{
	//}
	//ImGui::End();

	UpdateObjects();

	if (!NETWORK->IsOffline()) {
		ClientToServerPacket packet = m_pPlayer->MakePacketToSend();
		NETWORK->WritePacketData(packet);
	}

	if (!NETWORK->IsOffline()) {
		SyncSceneWithServer();
	}

	for (auto& pOtherPlayer : m_pOtherPlayers) {
		pOtherPlayer->Update();
	}

	if (static_pointer_cast<SpaceshipPlayer>(m_pPlayer)->m_bAlive == false) {
		if (m_fEndSceneTimer >= m_fEndSceneTime) {
			auto pPlayer = static_pointer_cast<SpaceshipPlayer>(m_pPlayer);
			m_fEndSceneTimer = 0.f;
			SCENE->ChangeScene<OutroScene>();
		}
		else {
			m_fEndSceneTimer += DT;
		}
	}
}

void TestScene::Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommansList)
{
	RenderObjects(pd3dCommansList);
}

void TestScene::SyncSceneWithServer()
{
	// send recv every frame
	ServertoClientPlayerPacket receivedPacket = NETWORK->GetReceivedPacketData();

	int nOtherPlayerIndex = 0;
	for (int i = 0; i < 3; ++i) {
		if (receivedPacket.client[i].id == NETWORK->GetPlayerID()) {
			auto pPlayer = static_pointer_cast<SpaceshipPlayer>(m_pPlayer);
			pPlayer->m_nScore = receivedPacket.client[i].informData.score;

			// 기존의 알던 체력과 새로 받은 체력이 다르면 흔든다
			// 체력이 갑자기 늘어날일은 없음(없어야함)
			if (pPlayer->m_fHP > receivedPacket.client[i].informData.hp) {
				pPlayer->SetShake(true);
				SOUND->Play("damage_sound");
				pPlayer->m_fHP = receivedPacket.client[i].informData.hp;
				pPlayer->m_nScore = receivedPacket.client[i].informData.score;
			}

			pPlayer->m_bAlive = receivedPacket.client[i].informData.alive;
			if (pPlayer->m_bAlive == false) {
				EffectParameter effectParam;
				effectParam.xmf3Position = pPlayer->GetTransform().GetPosition();
				effectParam.xmf3Force = Vector3(0, 0, 0);
				effectParam.fElapsedTime = 0.f;
				effectParam.fAdditionalData = 0.f;

				if (m_fEndSceneTimer == 0.f) {
					EFFECT->AddEffect<ExplosionEffect>(effectParam);
				}
			}
			continue;
		}

		auto pPlayer = static_pointer_cast<SpaceshipPlayer>(m_pOtherPlayers[nOtherPlayerIndex]);
		pPlayer->GetTransform().SetWorldMatrix(receivedPacket.client[i].transformData.mtxPlayerTransform);
		pPlayer->m_fHP = receivedPacket.client[i].informData.hp;
		pPlayer->m_bAlive = receivedPacket.client[i].informData.alive;

		if (receivedPacket.client[i].shotData.v3RayDirection != Vector3(0, 0, 0)) {
			EffectParameter param;
			param.xmf3Position = m_pOtherPlayers[nOtherPlayerIndex]->GetRayPos();
			param.xmf3Force = receivedPacket.client[i].shotData.v3RayDirection;	// use force to direction
			param.fElapsedTime = 0.f;

			EFFECT->AddEffect<RayEffect>(param);
		}
		nOtherPlayerIndex++;
	}

	ServertoClientRockPacket rockPacket = NETWORK->GetReceivedRockPacketData();
	for (int i = 0; i < rockPacket.size; ++i) {

		if (rockPacket.rockData[i].nIsAlive) {
			m_pRockObj->GetTransform().SetWorldMatrix(rockPacket.rockData[i].mtxRockTransform);
			m_pRockObj->Update();
		}
		else {
			Matrix mtxRock = rockPacket.rockData[i].mtxRockTransform;
			EffectParameter effectParam;
			effectParam.xmf3Position = mtxRock.Translation();
			effectParam.xmf3Force = Vector3(0, 0, 0);
			effectParam.fElapsedTime = 0.f;
			effectParam.fAdditionalData = 0.f;

			EFFECT->AddEffect<ExplosionEffect>(effectParam);
			SOUND->Play("explosion_effect");
		}
	}

}

void TestScene::ProcessInput()
{
	if (INPUT->GetButtonDown(VK_ESCAPE)) {
		PostQuitMessage(0);
	}
}

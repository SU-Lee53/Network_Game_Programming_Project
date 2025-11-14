#include "pch.h"
#include "TestScene.h"
#include "Transform.h"
#include "CubeObject.h"
#include "MercuryObject.h"
#include "VenusObject.h"
#include "EarthObject.h"
#include "MarsObject.h"
#include "SunObject.h"
#include "SpaceshipPlayer.h"

using namespace std::string_literals;

void TestScene::BuildObjects()
{
	/*
		- 현재 코드로는 인스턴싱이 안됨
		- 나중에 Model을 전역에서 관리하도록 하고, 그걸 공유하는 식으로 가면 될 "수도" 있음
			- 아직 안해봐서 모름
	
	*/
	std::shared_ptr<CubeObject> pCube1 = std::make_shared<CubeObject>();
	pCube1->Initialize();
	//pCube1->GetTransform().SetPosition(15.f, 0.f, 30.f);
	//AddObject(pCube1);

	std::shared_ptr<GameObject> pObj1 = std::make_shared<GameObject>();
	pObj1->GetTransform().SetPosition(0.f, 0.f, 0.f);
	pObj1->SetChild(GameObject::CopyObject(pCube1));
	pObj1->GetTransform().Scale(0.2f);
	AddObject(pObj1);

	//std::shared_ptr<GameObject> pObj2 = std::make_shared<GameObject>();
	//pObj2->GetTransform().SetPosition(-15.f, 0.f, 30.f);
	//pObj2->SetChild(GameObject::CopyObject(pCube1));
	//AddObject(pObj2);

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


	m_pPlayer = std::make_shared<SpaceshipPlayer>();

	//m_pMainCamera = std::make_shared<Camera>();
	//m_pMainCamera->SetViewport(0, 0, WinCore::sm_dwClientWidth, WinCore::sm_dwClientHeight, 0.f, 1.f);
	//m_pMainCamera->SetScissorRect(0, 0, WinCore::sm_dwClientWidth, WinCore::sm_dwClientHeight);
	//m_pMainCamera->GenerateViewMatrix(XMFLOAT3(0.f, 0.f, -15.f), XMFLOAT3(0.f, 0.f, 1.f), XMFLOAT3(0.f, 1.f, 0.f));
	//m_pMainCamera->GenerateProjectionMatrix(1.01f, 5000.0f, (WinCore::sm_dwClientWidth / WinCore::sm_dwClientHeight), 60.0f);

	//m_pLights.resize(MAX_LIGHTS);

	//MODELLOADER->Load<CubeObject, TexturedIlluminatedRenderer>("../Resource/plane.obj");

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
	pLight1->m_v4Diffuse = Vector4(10.0f, 10.0f, 10.0f, 1.0f);
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

	Vector2 v2ScreenCenter{ 0.5f, 0.5f };
	Vector2 v2CrosshairSize{ 0.03f, 0.03f };

	RenderParameter spriteParam{};
	spriteParam.eType = RENDER_ITEM_SPRITE;
	spriteParam.spriteParams.fLeft = v2ScreenCenter.x - v2CrosshairSize.x;
	spriteParam.spriteParams.fTop = v2ScreenCenter.y - v2CrosshairSize.y;
	spriteParam.spriteParams.fRight = v2ScreenCenter.x + v2CrosshairSize.x;
	spriteParam.spriteParams.fBottom = v2ScreenCenter.y + v2CrosshairSize.y;

	RENDER->Add(TEXTURE->Get("Crosshair"), spriteParam);

	ServertoClientPlayerPacket receivePacket;

	ImGui::Begin("Test");
	{
		ClientToServerPacket packet;
		

		if (ImGui::Button("Send")) {
			NETWORK->SendData(&packet, 1);

			NETWORK->ReceiveData(receivePacket);

			strReceived.clear();
			strReceived += std::format("ID : {}\n", receivePacket.client[0].id);
			strReceived += std::format("Position : {} {} {} {}\n\n", receivePacket.client[0].transformData.mtxPlayerTransform._41, receivePacket.client[0].transformData.mtxPlayerTransform._42, receivePacket.client[0].transformData.mtxPlayerTransform._43, receivePacket.client[0].transformData.mtxPlayerTransform._44);
			
			strReceived += std::format("ID : {}\n", receivePacket.client[0].id);
			strReceived += std::format("Position : {} {} {} {}\n\n", receivePacket.client[1].transformData.mtxPlayerTransform._41, receivePacket.client[1].transformData.mtxPlayerTransform._42, receivePacket.client[1].transformData.mtxPlayerTransform._43, receivePacket.client[1].transformData.mtxPlayerTransform._44);
			
			strReceived += std::format("ID : {}\n", receivePacket.client[0].id);
			strReceived += std::format("Position : {} {} {} {}\n\n", receivePacket.client[2].transformData.mtxPlayerTransform._41, receivePacket.client[2].transformData.mtxPlayerTransform._42, receivePacket.client[2].transformData.mtxPlayerTransform._43, receivePacket.client[2].transformData.mtxPlayerTransform._44);

		}
		ImGui::Text(strReceived.c_str());

	}
	ImGui::End();

	UpdateObjects();
}

void TestScene::Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommansList)
{
	RenderObjects(pd3dCommansList);
}

void TestScene::ProcessInput()
{
	if (INPUT->GetButtonDown(VK_ESCAPE)) {
		PostQuitMessage(0);
	}
}

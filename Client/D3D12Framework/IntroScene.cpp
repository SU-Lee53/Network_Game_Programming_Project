#include "pch.h"
#include "IntroScene.h"
#include "TestScene.h"

void IntroScene::BuildObjects()
{
	m_pPlayer = std::make_shared<Player>();

	std::shared_ptr<TexturedSprite> pTextureSprite = std::make_shared<TexturedSprite>("Opening", 0.f, 0.f, 1.0f, 1.0f);
	m_pSprites.push_back(pTextureSprite);

	InitializeObjects();
}

void IntroScene::Update()
{
	NETWORK->ConnectToServer();

	UpdateObjects();

	// Start game when game started
	if (NETWORK->IsGameStarted()) {
		SCENE->ChangeScene<TestScene>();
	}
	else {
	}
}

void IntroScene::Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommansList)
{
	RenderObjects(pd3dCommansList);
}

void IntroScene::ProcessInput()
{
	if (INPUT->GetButtonDown(VK_SPACE)) {
		SCENE->ChangeScene<TestScene>();
	}
}

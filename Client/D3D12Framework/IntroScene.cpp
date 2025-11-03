#include "pch.h"
#include "IntroScene.h"
#include "TestScene.h"

void IntroScene::BuildObjects()
{
	m_pPlayer = std::make_shared<Player>();

	InitializeObjects();
}

void IntroScene::Update()
{
	RenderParameter spriteParam{};
	spriteParam.eType = RENDER_ITEM_SPRITE;
	spriteParam.spriteParams.fLeft = 0;
	spriteParam.spriteParams.fTop = 0;
	spriteParam.spriteParams.fRight = 1;
	spriteParam.spriteParams.fBottom = 1;

	RENDER->Add(TEXTURE->Get("Opening"), spriteParam);

	NETWORK->ConnectToServer();


	UpdateObjects();
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

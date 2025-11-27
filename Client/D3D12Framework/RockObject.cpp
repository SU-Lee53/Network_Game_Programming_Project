#include "pch.h"
#include "RockObject.h"

// 10.23
// TODO : 여기는 그냥 처음부터 다시하는게 빠를듯ㅇㅇ....

RockObject::RockObject()
{
}

RockObject::~RockObject()
{
}

void RockObject::Initialize()
{
	if (!m_bInitialized) {
		std::shared_ptr<GameObject> pSphere = MODEL->Get("Sphere_4");	// 1
		SetChild(pSphere);


		auto& p = FindMeshedFrame("Sphere001");
		p->GetMeshRenderer()->SetTexture(TEXTURE->Get("Rock_Diffuse"), 0, TEXTURE_TYPE_DIFFUSE);
		p->GetMeshRenderer()->SetTexture(TEXTURE->Get("Rock_Normal"), 0, TEXTURE_TYPE_NORMAL);

		m_bInitialized = true;
	}

	GameObject::Initialize();
}

void RockObject::Update()
{
	GameObject::Update();
}

void RockObject::Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList)
{
}

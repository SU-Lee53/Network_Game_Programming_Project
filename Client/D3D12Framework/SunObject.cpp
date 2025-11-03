#include "pch.h"
#include "SunObject.h"

SunObject::SunObject()
{
}

SunObject::~SunObject()
{
}

void SunObject::Initialize()
{
	if (!m_bInitialized) {
		std::shared_ptr<GameObject> pSphere = MODEL->Get("Sphere_5")->CopyObject<GameObject>();	// 1
		SetChild(pSphere);


		auto& p = FindMeshedFrame("Sphere001");
		p->GetMeshRenderer()->SetTexture(TEXTURE->Get("Sun_Diffuse"), 0, TEXTURE_TYPE_DIFFUSE);
		p->GetMeshRenderer()->SetTexture(TEXTURE->Get("Sun_Normal"), 0, TEXTURE_TYPE_NORMAL);

		m_Transform.SetPosition(-200, 0, -40);
		m_Transform.Scale(20);

		m_bInitialized = true;
	}

	GameObject::Initialize();
}

void SunObject::Update()
{
	GameObject::Update();
}

void SunObject::Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList)
{
}

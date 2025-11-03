#include "pch.h"
#include "MarsObject.h"

MarsObject::MarsObject()
{
}

MarsObject::~MarsObject()
{
}

void MarsObject::Initialize()
{
	if (!m_bInitialized) {
		std::shared_ptr<GameObject> pSphere = MODEL->Get("Sphere_2")->CopyObject<GameObject>();	// 1
		SetChild(pSphere);

		auto& p = FindMeshedFrame("Sphere001");
		p->GetMeshRenderer()->SetTexture(TEXTURE->Get("Mars_Diffuse"), 0, TEXTURE_TYPE_DIFFUSE);
		p->GetMeshRenderer()->SetTexture(TEXTURE->Get("Mars_Normal"), 0, TEXTURE_TYPE_NORMAL);

		m_Transform.SetPosition(200 + 100, 0, -40);
		m_Transform.Scale(2);
		m_Transform.Rotate(Vector3::Right, 25.2f);

		m_bInitialized = true;
	}

	GameObject::Initialize();
}

void MarsObject::Update()
{
	m_Transform.Rotate(Vector3(0.f, 1.f, 0.f), 0.97f * 20.f * DT);
	m_Transform.RotateWorld(Vector3(0.f, 1.f, 0.f), -0.53f * DT, Vector3(-200, 0, -40));

	GameObject::Update();
}

void MarsObject::Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList)
{
}

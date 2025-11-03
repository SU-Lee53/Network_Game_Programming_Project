#include "pch.h"
#include "MercuryObject.h"

MercuryObject::MercuryObject()
{
}

MercuryObject::~MercuryObject()
{
}

void MercuryObject::Initialize()
{
	if (!m_bInitialized) {
		std::shared_ptr<GameObject> pSphere = MODEL->Get("Sphere_3")->CopyObject<GameObject>();	// 1
		SetChild(pSphere);

		auto& p = FindMeshedFrame("Sphere001");
		p->GetMeshRenderer()->SetTexture(TEXTURE->Get("Mercury_Diffuse"), 0, TEXTURE_TYPE_DIFFUSE);
		p->GetMeshRenderer()->SetTexture(TEXTURE->Get("Mercury_Normal"), 0, TEXTURE_TYPE_NORMAL);

		m_Transform.SetPosition(30 + 100, 0, -40);
		m_Transform.Rotate(Vector3::Right, 0.034f);

		m_bInitialized = true;
	}

	GameObject::Initialize();
}

void MercuryObject::Update()
{
	m_Transform.Rotate(Vector3(0.f, 1.f, 0.f), 0.017f * 20.f * DT);
	m_Transform.RotateWorld(Vector3(0.f, 1.f, 0.f), -4.17f * DT, Vector3(-200, 0, -40));

	GameObject::Update();
}

void MercuryObject::Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList)
{
}

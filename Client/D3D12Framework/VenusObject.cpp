#include "pch.h"
#include "VenusObject.h"

VenusObject::VenusObject()
{
}

VenusObject::~VenusObject()
{
}

void VenusObject::Initialize()
{
	if (!m_bInitialized) {
		std::shared_ptr<GameObject> pSphere = MODEL->Get("Sphere_6")->CopyObject<GameObject>();	// 1
		SetChild(pSphere);

		auto& p = FindMeshedFrame("Sphere001");
		p->GetMeshRenderer()->SetTexture(TEXTURE->Get("Venus_Diffuse"), 0, TEXTURE_TYPE_DIFFUSE);
		p->GetMeshRenderer()->SetTexture(TEXTURE->Get("Venus_Normal"), 0, TEXTURE_TYPE_NORMAL);

		m_Transform.SetPosition( 80 + 100,0,-40 );
		m_Transform.Scale(1.5);
		m_Transform.Rotate(Vector3::Right, 177.4f);

		m_bInitialized = true;
	}

	GameObject::Initialize();
}

void VenusObject::Update()
{
	m_Transform.Rotate(Vector3(0.f, 1.f, 0.f), 0.041f * 20.f * DT);
	m_Transform.RotateWorld(Vector3(0.f, 1.f, 0.f), -1.61f * DT, Vector3(-200, 0, -40));

	GameObject::Update();
}

void VenusObject::Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList)
{
}

#include "pch.h"
#include "EarthObject.h"

EarthObject::EarthObject()
{
}

EarthObject::~EarthObject()
{
}

void EarthObject::Initialize()
{
	// 10.22
	// TODO : 이거 어떻게 초기화할지 좀 생각해봐야할듯
	// 1. ModelManager 에서 하고 Scene 에서 불러온다 -> 이걸로 ㄱㄱ
	// 2. 직접 모델 가져와서 SetChild 한다
	if (!m_bInitialized) {
		std::shared_ptr<GameObject> pSphere = MODEL->Get("Sphere_1")->CopyObject<GameObject>();	// 1
		SetChild(pSphere);

		auto& p = FindMeshedFrame("Sphere001");
		p->GetMeshRenderer()->SetTexture(TEXTURE->Get("Earth_Diffuse"), 0, TEXTURE_TYPE_DIFFUSE);
		p->GetMeshRenderer()->SetTexture(TEXTURE->Get("Earth_Normal"), 0, TEXTURE_TYPE_NORMAL);

		m_Transform.SetPosition(120 + 100, 0, -40);
		m_Transform.Scale(2);
		m_Transform.Rotate(Vector3::Right, 23.4f);

		m_bInitialized = true;
	}

	GameObject::Initialize();
}

void EarthObject::Update()
{
	m_Transform.Rotate(Vector3(0.f, 1.f, 0.f), 0.041f * 20.f * DT);
	m_Transform.RotateWorld(Vector3(0.f, 1.f, 0.f), -1.0f * DT, Vector3(-200, 0, -40));
	
	GameObject::Update();
}

void EarthObject::Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList)
{
}

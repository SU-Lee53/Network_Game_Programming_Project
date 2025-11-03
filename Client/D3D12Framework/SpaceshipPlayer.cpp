#include "pch.h"
#include "SpaceshipPlayer.h"
#include "ThirdPersonCamera.h"
#include "PlayerRenderer.h"

SpaceshipPlayer::SpaceshipPlayer()
{
}

SpaceshipPlayer::~SpaceshipPlayer()
{
}

void SpaceshipPlayer::Initialize()
{
	if (!m_bInitialized) {
		// 아래 방식 멀티플레이 들어가면 PlayerRenderer 의 PlayerRef 가 분명 문제가 될거같음
		// 문제되면 그냥 같은모델 하나 더 로딩
		std::shared_ptr<GameObject> pSphere = MODEL->Get("Spaceship");	// 1
		SetChild(pSphere);

		auto& p = FindMeshedFrame("ship");
		p->GetMeshRenderer()->SetTexture(TEXTURE->Get("Spaceship_Diffuse"), 0, TEXTURE_TYPE_DIFFUSE);
		p->GetMeshRenderer()->SetTexture(TEXTURE->Get("Spaceship_Normal"), 0, TEXTURE_TYPE_NORMAL);
		static_pointer_cast<PlayerRenderer>(p->GetMeshRenderer())->SetPlayer(static_pointer_cast<SpaceshipPlayer>(shared_from_this()));

		// Camera
		auto pCamera = std::make_shared<ThirdPersonCamera>();
		pCamera->SetViewport(0, 0, WinCore::sm_dwClientWidth, WinCore::sm_dwClientHeight, 0.f, 1.f);
		pCamera->SetScissorRect(0, 0, WinCore::sm_dwClientWidth, WinCore::sm_dwClientHeight);
		pCamera->GenerateViewMatrix(XMFLOAT3(0.f, 0.f, -15.f), XMFLOAT3(0.f, 0.f, 1.f), XMFLOAT3(0.f, 1.f, 0.f));
		pCamera->GenerateProjectionMatrix(1.01f, 400.0f, (WinCore::sm_dwClientWidth / WinCore::sm_dwClientHeight), 60.0f);
		pCamera->SetOffset(Vector3(0.f, 5.f, -10.f));
		pCamera->SetOwner(shared_from_this());
		pCamera->SetTimeLag(0.05f);
		m_pCamera = pCamera;


		//m_Transform.SetPosition(0, 0, -100);

		m_bInitialized = true;
	}

	GameObject::Initialize();
}

void SpaceshipPlayer::ProcessInput()
{
	m_pCamera->ProcessInput();

	if (INPUT->GetButtonDown(VK_LBUTTON)) {
		/* 발사 */
		m_bIsFire = true;

		// Ray Direction
		Vector3 v3CamPos = m_pCamera->GetPosition();
		
		POINT m_currentCursorPos = INPUT->GetCurrentCursorPos();

		float fNdcX = 2.0f * ((float)m_currentCursorPos.x / (float)WinCore::sm_dwClientWidth) - 1.0f;
		float fNdcY = 2.0f * ((float)m_currentCursorPos.y / (float)WinCore::sm_dwClientHeight) - 1.0f;

		Vector4 v4RayPosInClipSpace{ fNdcX, fNdcY, 0.f, 1.f };

		Vector3 v4RayPosInViewSpace = Vector4::Transform(v4RayPosInClipSpace, m_pCamera->GetProjectionMatrix().Invert());
		v4RayPosInViewSpace = Vector3(v4RayPosInViewSpace.x, v4RayPosInViewSpace.y, 1.f);

		Vector3 v3RayPositionInWorldSpace = Vector3::Transform(v4RayPosInViewSpace, m_pCamera->GetViewMatrix().Invert());
		v3RayPositionInWorldSpace.Normalize();

		Vector3 v3IntersectionRayCamera = v3CamPos + (v3RayPositionInWorldSpace * m_pCamera->GetFarPlaneDistance());

		m_vRayDirection = v3IntersectionRayCamera - GetRayPos();
		m_vRayDirection.Normalize();
	}

	// Roll 회전 추가
	float fRollAmount = 100.f * DT;
	if (INPUT->GetButtonPressed('D')) {
		if (m_fRoll > -45.f) {
			m_fRoll -= fRollAmount;
		}
	}
	else {
		if (m_fRoll < 0) {
			m_fRoll += fRollAmount;
		}
	}

	if (INPUT->GetButtonPressed('A')) {
		if (m_fRoll <= 45.f) {
			m_fRoll += fRollAmount;
		}
	}
	else {
		if (m_fRoll > 0) {
			m_fRoll -= fRollAmount;
		}
	}

	// Barrel Roll
	if (INPUT->GetButtonDown('R')) {
		m_bBarrelRoll = true;
	}

}

void SpaceshipPlayer::Update()
{
	// 발사 타이머 로직
	if (m_bIsFire && m_fFireTimer <= m_fFireDuration) {
		m_fFireTimer += DT;
	}
	else if (m_bIsFire && m_fFireTimer > m_fFireDuration) {
		m_bIsFire = false;
		m_fFireTimer = 0.f;
	}
	
	// Barrel Roll
	if (m_bBarrelRoll) {
		if (m_fRoll < 0) {
			if (m_fRoll > -720.f) {
				m_fAngularVelocity -= 10 * DT;
				m_fRoll += m_fAngularVelocity;
			}
			else {
				m_bBarrelRoll = false;
				m_fRoll = 0.f;
				m_fAngularVelocity = 3.f;
			}
		}
		else {
			if (m_fRoll < 720.f) {
				m_fAngularVelocity += 10 * DT;
				m_fRoll += m_fAngularVelocity;
			}
			else {
				m_bBarrelRoll = false;
				m_fRoll = 0.f;
				m_fAngularVelocity = 3.f;
			}
		}
	}



	m_Transform.SetRotation(m_pCamera->GetPitch(), m_pCamera->GetYaw(), XMConvertToRadians(m_fRoll));

	// view 먼저 한번 만들어서 카메라 기저 정상화
	m_pCamera->Update();

	// 카메라에 이동 맞춤
	Vector3 v3CamPosition = m_pCamera->GetPosition();
	Vector3 v3CamRight = m_pCamera->GetRight();
	Vector3 v3CamUp = m_pCamera->GetUp();
	Vector3 v3CamLook = m_pCamera->GetLook();

	Vector3 v3PlayerPosition = v3CamPosition;
	v3PlayerPosition -= v3CamUp * 3.f;
	v3PlayerPosition += v3CamLook * 8.0f;
	m_Transform.SetPosition(v3PlayerPosition);

	Player::Update();

}

void SpaceshipPlayer::Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList)
{
}

Vector3 SpaceshipPlayer::GetRayPos()
{
	Vector3 v3PlayerPosition = m_Transform.GetPosition();
	v3PlayerPosition += Vector3(0.0f, 0.2f, 3.0f);

	return Vector3::Transform(v3PlayerPosition, m_Transform.GetWorldMatrix());
}

Vector3 SpaceshipPlayer::GetRayDirection()
{
	return m_vRayDirection;
}

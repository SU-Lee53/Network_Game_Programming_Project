#include "pch.h"
#include "SpaceshipPlayer.h"
#include "ThirdPersonCamera.h"
#include "PlayerRenderer.h"
#include "NetworkManager.h"
#include "Effect.h"

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
		m_bRayDataSended = false;	// Ray 발사시 Data 가 보내야 함을 알림

		//float fScreenX = WinCore::sm_dwClientWidth;
		//float fScreenY = WinCore::sm_dwClientHeight;

		float fNdcX = 0.f;	// (fScreenX / WinCore::sm_dwClientWidth) * 2.f - 1.f;
		float fNdcY = 0.f;	// (fScreenY / WinCore::sm_dwClientHeight) * -2.f + 1.f;

		Vector3 v3NearPoint = Vector3(fNdcX, fNdcY, 0.f);
		Vector3 v3FarPoint = Vector3(fNdcX, fNdcY, 1.f);

		Matrix mtxInvVP = CUR_SCENE->GetCamera()->GetViewProjectMatrix().Invert();

		v3NearPoint = XMVector3TransformCoord(v3NearPoint, mtxInvVP);
		v3FarPoint = XMVector3TransformCoord(v3FarPoint, mtxInvVP);

		m_vRayDirection = v3FarPoint - v3NearPoint;
		m_vRayDirection.Normalize();

		// Draw Ray Effect
		EffectParameter param;
		param.xmf3Position = GetRayPos();
		param.xmf3Force = m_Transform.GetLook();	// use force to direction
		param.fElapsedTime = 0.f;

		EFFECT->AddEffect<RayEffect>(param);

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

	// view 먼저 한번 만들어서 카메라 기저 정상화
	if (m_pCamera) {
		m_Transform.SetRotation(m_pCamera->GetPitch(), m_pCamera->GetYaw(), XMConvertToRadians(m_fRoll));

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

	}

	Player::Update();

}

void SpaceshipPlayer::Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList)
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// 2025.11.10
// MakePacketToSend() By 이승욱
// 11.10 기준으로 정해진 패킷 데이터를 채움 - 언제든지 바뀔 수 있음
ClientToServerPacket SpaceshipPlayer::MakePacketToSend()
{
	ClientToServerPacket packet;
	packet.id = 0;	// 일단 안보내도 됨
	packet.transformData.mtxPlayerTransform = m_Transform.GetWorldMatrix();
	if (m_bIsFire && !m_bRayDataSended) {
		packet.shotData.v3RayPosition = GetRayPos();
		packet.shotData.v3RayDirection = GetRayDirection();
		m_bRayDataSended = true;	// 한번 보냈음을 알림 (== 패킷을 만들었음)
	}
	else {
		packet.shotData.v3RayPosition = Vector3(0.f, 0.f, 0.f);
		packet.shotData.v3RayDirection = Vector3(0.f, 0.f, 0.f);
	}

	return packet;
}

Vector3 SpaceshipPlayer::GetRayPos()
{
	Vector3 v3PlayerPosition = Vector3(0,0,0);
	v3PlayerPosition += Vector3(0.0f, 0.2f, 3.0f);

	return Vector3::Transform(v3PlayerPosition, m_Transform.GetWorldMatrix());
}

Vector3 SpaceshipPlayer::GetRayDirection()
{
	return m_vRayDirection; // m_Transform.GetLook();
}

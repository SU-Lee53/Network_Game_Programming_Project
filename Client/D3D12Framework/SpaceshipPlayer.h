#pragma once
#include "Player.h"

class SpaceshipPlayer : public Player {
public:
	SpaceshipPlayer();
	virtual ~SpaceshipPlayer();

public:
	virtual void Initialize() override;
	virtual void ProcessInput() override;
	virtual void Update() override;
	virtual void Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList) override;

	// 2025.11.10
	// MakePacketToSend() By 이승욱
	// 정의는 override 받은 SpaceshipPlayer에서 함
	virtual ClientToServerPacket MakePacketToSend() override;

public:
	bool IsFiring() { return m_bIsFire; }
	Vector3 GetRayPos();
	Vector3 GetRayDirection();

	float GetRoll() const { return m_fRoll; }

public:
	// 11.15 
	// 타 플레이어 발사 확인을 위해 잠시 false 로 만듬
	bool m_bIsFire = false;

private:
	float m_fFireTimer = 0.f;
	const float m_fFireDuration = 0.01f;
	Vector3 m_vRayDirection;


	POINT m_oldCursorPos;

	float m_fPlayerSpeed = 50.f;
	float m_fRoll = 0.f;
	bool m_bBarrelRoll = false;
	float m_fAngularVelocity = 3.0f;

	// 2025.11.10
	// m_bIsFire 는 m_fFireDuration 동안 켜져있으므로 한번만 패킷을 보내려는 의도와 맞지 않음
	// 따라서 새로운 bool 변수를 만들어 한번만 보내도록 제한함
	bool m_bRayDataSended = false;

};


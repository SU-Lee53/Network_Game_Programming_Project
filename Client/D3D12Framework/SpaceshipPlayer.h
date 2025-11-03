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

public:
	bool IsFiring() { return m_bIsFire; }
	Vector3 GetRayPos();
	Vector3 GetRayDirection();

	float GetRoll() const { return m_fRoll; }

private:
	bool m_bIsFire = false;
	float m_fFireTimer = 0.f;
	const float m_fFireDuration = 0.01f;
	Vector3 m_vRayDirection;


	POINT m_oldCursorPos;

	float m_fPlayerSpeed = 50.f;
	float m_fRoll = 0.f;
	bool m_bBarrelRoll = false;
	float m_fAngularVelocity = 3.0f;
};


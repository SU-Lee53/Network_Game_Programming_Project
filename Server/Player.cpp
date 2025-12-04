#include "Player.h"

Player::Player()
{
	XMStoreFloat4x4(&m_xmf4x4WorldMatrix, XMMatrixIdentity());
	m_fBoundingRadius = 2.0f;

	m_xmBoundingSphere.Center = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_xmBoundingSphere.Radius = m_fBoundingRadius;


}

Player::~Player()
{

}

void Player::Update(float deltaTime)
{

	m_fInvincibleTimer -= deltaTime;
	if (m_fInvincibleTimer <= 0.f)
	{
		m_fInvincibleTimer = 0.f;
	}

}

void Player::SetRayData(const Ray& ray)
{
	m_RayData = ray;
}

const Ray& Player::GetRayData() const
{
	return m_RayData;
}

void Player::SetInformData(int fHP, float nScore, bool bAlive, bool bInvincible)
{
	m_fHP = fHP;
	m_nScore = nScore;
	m_bAlive = bAlive;
	m_bInvincible = bInvincible;
}

void Player::GiveScore(int nScore)
{
	m_nScore += nScore;
}

void Player::SetDamagefromPlayer()
{
	if (m_bInvincible || !m_bAlive)
	{
		m_fInvincibleTimer = 0.5f;
		return;
	}

	if (m_fHP > 0)
	{
		m_fHP -= 10.f;
	}

	// 추가
	if (m_fHP <= 0) {
		m_bAlive = false;
	}

	m_fInvincibleTimer = 0.5f;
}

void Player::SetDamagefromRock()
{

	if (m_bInvincible || !m_bAlive) 
	{
		m_fInvincibleTimer = 0.5f;
		return;
	}

	if (m_fHP > 0)
	{
		m_fHP -= 20.f;
	}

	// 추가
	if (m_fHP <= 0) {
		m_bAlive = false;
	}

	m_fInvincibleTimer = 0.5f;
}

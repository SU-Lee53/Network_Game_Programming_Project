#include "Player.h"

Player::Player()
{
	hp = 10000000;

	XMStoreFloat4x4(&m_xmf4x4WorldMatrix, XMMatrixIdentity());
	m_fBoundingRadius = 2.0f;

	m_xmBoundingSphere.Center = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_xmBoundingSphere.Radius = m_fBoundingRadius;


}

Player::~Player()
{

}

void Player::SetRayData(const Ray& ray)
{
	m_RayData = ray;
}

const Ray& Player::GetRayData() const
{
	return m_RayData;
}

void Player::SetDamagefromPlayer()
{
	if (hp > 0)
	{
		hp -= 0.001;
	}
}

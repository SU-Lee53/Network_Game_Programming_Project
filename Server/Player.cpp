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

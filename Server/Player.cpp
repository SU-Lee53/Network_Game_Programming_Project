#include "Player.h"

Player::Player()
{
	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());
	m_BoundingRadius = 2.0f;

	m_BoundingSphere.Center = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_BoundingSphere.Radius = m_BoundingRadius;
}

Player::~Player()
{

}

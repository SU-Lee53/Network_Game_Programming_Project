#include "Rock.h"

Rock::Rock()
{
	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());
	m_BoundingRadius = 2.0f;

	m_BoundingSphere.Center = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_BoundingSphere.Radius = m_BoundingRadius;
}

Rock::~Rock()
{
}

void Rock::SetDirection(const XMFLOAT3& PlayerPosition)
{
	XMFLOAT3 Direction = Vector3::Subtract(PlayerPosition, GetPosition());
	m_Xmf3Direction = Vector3::Normalize(Direction);
}


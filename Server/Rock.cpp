#include "Rock.h"

Rock::Rock()
{
	XMStoreFloat4x4(&m_xmf4x4WorldMatrix, XMMatrixIdentity());
	m_fBoundingRadius = 2.0f;

	m_xmBoundingSphere.Center = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_xmBoundingSphere.Radius = m_fBoundingRadius;
}

Rock::~Rock()
{
}

void Rock::SetDirection(const XMFLOAT3& PlayerPosition)
{
	XMFLOAT3 Direction = Vector3::Subtract(PlayerPosition, GetPosition());
	m_xmf3Direction = Vector3::Normalize(Direction);
}


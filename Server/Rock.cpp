#include "Rock.h"

Rock::Rock()
{
	XMStoreFloat4x4(&m_xmf4x4WorldMatrix, XMMatrixIdentity());
	m_fBoundingRadius = 4.5f;

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

//////////////////////////////////////////////////////////////////////////////////////////////
// 2025.12.01
// Rock::Update() By 민정원
// Delta Time 계산 추가
void Rock::Update(float deltaTime)
{
	m_nSpeed += 8.f * deltaTime;
	XMFLOAT3 currentPos = GetPosition();

	XMFLOAT3 movement;
	movement.x = m_xmf3Direction.x * m_nSpeed * deltaTime;
	movement.y = m_xmf3Direction.y * m_nSpeed * deltaTime;
	movement.z = m_xmf3Direction.z * m_nSpeed * deltaTime;

	XMFLOAT3 newPos;
	newPos.x = currentPos.x + movement.x;
	newPos.y = currentPos.y + movement.y;
	newPos.z = currentPos.z + movement.z;

	SetPosition(newPos.x, newPos.y, newPos.z);

	UpdateBoundingSphere();
}


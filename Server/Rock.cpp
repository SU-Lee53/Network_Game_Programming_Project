#include "Rock.h"

std::uniform_real_distribution<float> accelDist(10.0f, 15.0f);

Rock::Rock()
{
	XMStoreFloat4x4(&m_xmf4x4WorldMatrix, XMMatrixIdentity());
	m_fBoundingRadius = 4.0f;

	m_xmBoundingSphere.Center = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_xmBoundingSphere.Radius = m_fBoundingRadius;

	
	m_fAcceleration = accelDist(dre);
}

Rock::~Rock()
{
}

void Rock::SetDirection(const XMFLOAT3& PlayerPosition)
{
	XMFLOAT3 Direction = Vector3::Subtract(PlayerPosition, GetPosition());
	m_xmf3Direction = Vector3::Normalize(Direction);
}

bool Rock::IsOutOfBounds() const
{
	XMFLOAT3 pos = GetPosition();
	XMVECTOR posVector = XMLoadFloat3(&pos);
	float distanceSq = XMVectorGetX(XMVector3LengthSq(posVector));
	return distanceSq >= 1100.f * 1100.f;  // 1100² = 1210000
}

//////////////////////////////////////////////////////////////////////////////////////////////
// 2025.12.01
// Rock::Update() By 민정원
// Delta Time 계산 추가
// 2025.12.03
// 가속도를 멤버 변수로 변경 (랜덤)
void Rock::Update(float deltaTime)
{
	m_nSpeed += m_fAcceleration * deltaTime;
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


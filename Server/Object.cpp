#include "Object.h"

Object::Object()
{
	XMStoreFloat4x4(&m_xmf4x4WorldMatrix, XMMatrixIdentity());
	m_fBoundingRadius = 1.0f;

	m_xmBoundingSphere.Center = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_xmBoundingSphere.Radius = m_fBoundingRadius;
}

Object::~Object()
{

}

void Object::SetWorldMatrix(const XMFLOAT4X4& worldMatrix)
{
	m_xmf4x4WorldMatrix = worldMatrix;
	UpdateBoundingSphere();
}

void Object::SetBoundingRadius(float radius)
{
	m_fBoundingRadius = radius;
	UpdateBoundingSphere();
}

XMFLOAT3 Object::GetPosition() const
{
	return XMFLOAT3(m_xmf4x4WorldMatrix._41, m_xmf4x4WorldMatrix._42, m_xmf4x4WorldMatrix._43);
}

void Object::SetPosition(const float& x, const float& y, const float& z)
{
	m_xmf4x4WorldMatrix._41 = x;
	m_xmf4x4WorldMatrix._42 = y;
	m_xmf4x4WorldMatrix._43 = z;
}

void Object::UpdateBoundingSphere()
{
	m_xmBoundingSphere.Center = GetPosition();
	m_xmBoundingSphere.Radius = m_fBoundingRadius;
}

void Object::Update()
{

}

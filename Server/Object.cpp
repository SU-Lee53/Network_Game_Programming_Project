#include "Object.h"

Object::Object()
{
	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());
	m_BoundingRadius = 1.0f;

	m_BoundingSphere.Center = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_BoundingSphere.Radius = m_BoundingRadius;
}

Object::~Object()
{

}

void Object::SetWorldMatrix(const XMFLOAT4X4& worldMatrix)
{
	m_WorldMatrix = worldMatrix;
	UpdateBoundingSphere();
}

void Object::SetBoundingRadius(float radius)
{
	m_BoundingRadius = radius;
	UpdateBoundingSphere();
}

XMFLOAT3 Object::GetPosition() const
{
	return XMFLOAT3(m_WorldMatrix._41, m_WorldMatrix._42, m_WorldMatrix._43);
}

void Object::UpdateBoundingSphere()
{
	m_BoundingSphere.Center = GetPosition();
	m_BoundingSphere.Radius = m_BoundingRadius;
}

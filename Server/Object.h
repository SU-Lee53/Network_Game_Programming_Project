#pragma once
#include "Common.h"

//////////////////////////////////////////////////////////////////////////////////////////////
// 2025.11.20
// 오브젝트 구조 By 민정원
// 오브젝트 구조 정의

class Object
{
public:
	Object();
	virtual ~Object();

	virtual void SetWorldMatrix(const XMFLOAT4X4& worldMatrix);
	virtual const XMFLOAT4X4& GetWorldMatrix() const { return m_WorldMatrix; }

	virtual void SetBoundingRadius(float radius);
	virtual float GetBoundingRadius() const { return m_BoundingRadius; }

	virtual XMFLOAT3 GetPosition() const;

	virtual const BoundingSphere& GetBoundingSphere() const { return m_BoundingSphere; }

	virtual void UpdateBoundingSphere();

protected:
	XMFLOAT4X4 m_WorldMatrix;           // 월드 변환 행렬
	float m_BoundingRadius;              // 바운딩 스피어 반지름
	BoundingSphere m_BoundingSphere;     // DirectXCollision 바운딩 스피어
};

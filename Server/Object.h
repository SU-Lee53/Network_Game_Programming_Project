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

	void SetWorldMatrix(const XMFLOAT4X4& worldMatrix);
	const XMFLOAT4X4& GetWorldMatrix() const { return m_xmf4x4WorldMatrix; }
	void SetBoundingRadius(float radius);
	float GetBoundingRadius() const { return m_fBoundingRadius; }
	XMFLOAT3 GetPosition() const;
	const BoundingSphere& GetBoundingSphere() const { return m_xmBoundingSphere; }
	void UpdateBoundingSphere();

protected:
	XMFLOAT4X4 m_xmf4x4WorldMatrix;           // 월드 변환 행렬
	float m_fBoundingRadius;              // 바운딩 스피어 반지름
	BoundingSphere m_xmBoundingSphere;     // DirectXCollision 바운딩 스피어
};

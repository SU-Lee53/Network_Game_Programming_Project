#pragma once
#include "Common.h"
//////////////////////////////////////////////////////////////////////////////////////////////
// 2025.11.18
// Class Rock By 민정원
// Class Rock 정의
class Rock 
{
public:
	Rock();
	~Rock();
	void SetDirection(const XMFLOAT3& PlayerPosition);
	const XMFLOAT3& GetPosition(){ return m_Xmf3Position; }
private:
	float speed = 10.f;
	XMFLOAT3 m_Xmf3Direction{};
	XMFLOAT3 m_Xmf3Position{};
};



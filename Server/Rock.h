#pragma once
#include "Common.h"
#include "Object.h"
//////////////////////////////////////////////////////////////////////////////////////////////
// 2025.11.18
// Class Rock By 민정원
// Class Rock 정의 - Object 상속
// 2025.11.20
// Class Rock By 민정원 
// Class Rock 다시정의 - Object 상속

class Rock : public Object
{
public:
	Rock();
	virtual ~Rock();


	void SetDirection(const XMFLOAT3& PlayerPosition);
	const bool& GetIsAlive() { return nIsAlive; }
	void SetDead() { nIsAlive = false; }
	bool IsOutOfBounds() const;  // 거리 1100 이상 체크

	virtual void Update(float deltaTime) override;
private:
	XMFLOAT3 m_xmf3Direction{};
	bool nIsAlive = true;
	float m_nSpeed{};
	float m_fAcceleration{};  // 가속도 (랜덤)
};




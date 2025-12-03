#pragma once
#include "Object.h"
// 2025.11.20
// Class Player By 민정원 
// Class Player 다시정의 - Object 상속

// 11.28 이승욱
// Ray 추가

class Player : public Object
{
public:
	Player();
	virtual ~Player();

	void SetRayData(const Ray& ray);
	const Ray& GetRayData() const;

	void SetInformData(int fHP, float nScore, bool bAlive, bool bInvincible);

	bool IsAlive() { return m_bAlive; }
	bool IsInvincible() { return m_bInvincible; }
	float GetHP() { return m_fHP; }

	int GetScore() { return m_nScore; }
	void GiveScore(int nScore);

	void SetDamagefromPlayer();
	void SetDamagefromRock();

private:
	Ray m_RayData;

private:
	bool m_bAlive = true;
	bool m_bInvincible = false;
	float m_fHP = 100.f;
	int m_nScore = 0;

};

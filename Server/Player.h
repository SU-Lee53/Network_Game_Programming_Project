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


private:
	Ray m_RayData;

};

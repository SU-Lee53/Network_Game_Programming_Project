#pragma once
#include "Object.h"
// 2025.11.20
// Class Player By 민정원 
// Class Player 다시정의 - Object 상속

class Player : public Object
{
	int hp;

public:
	Player();
	virtual ~Player();
};

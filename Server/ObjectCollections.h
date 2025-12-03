#pragma once
#include "Rock.h"
#include "Player.h"

inline std::list<std::unique_ptr<Rock>>			Rocks{};
inline std::array<std::unique_ptr<Player>, 3>	Players{};
inline int RockIndex = 0;

// 12.03 추가 by 이승욱
inline float g_fGamePlayTime = 0.f;

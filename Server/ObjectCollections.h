#pragma once
#include "Rock.h"
#include "Player.h"

inline std::list<std::unique_ptr<Rock>>			Rocks{};
inline std::array<std::unique_ptr<Player>, 3>	Players{};
inline int RockIndex = 0;



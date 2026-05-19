#pragma once

#include "Entity/Enemy/Monster.h"

// 20턴 보스 (Monster 확장, B 타일 표시)
class Boss : public Monster
{
public:
	Boss();
	~Boss() override = default;

};

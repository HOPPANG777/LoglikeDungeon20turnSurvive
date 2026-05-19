#pragma once

#include "Entity/Entity.h"

class Log;

// 플레이어·몬스터 간 교환 전투 처리
class Combat
{
public:
	Combat() = delete;
	~Combat() = delete;

public:
	// 공격자 선공 후 생존 시 반격 1회
	static void PerformExchange(Entity& attacker, Entity& defender, Log& log);

public:
	// 한쪽이 죽을 때까지 교환 전투 반복
	static void FightUntilEnd(Entity& first, Entity& second, Log& log);

};

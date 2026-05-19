#include "Combat.h"
#include "Systems/Log/Log.h"

using namespace std;

// 공격자 선공 후 생존 시 반격 1회
void Combat::PerformExchange(Entity& attacker, Entity& defender, Log& log)
{
	attacker.Attack(defender);
	log.Add(defender.GetName() + " took " + to_string(attacker.GetATK()) + " damage.");

	if (!defender.IsDead())
	{
		defender.Attack(attacker);
		log.Add(attacker.GetName() + " took " + to_string(defender.GetATK()) + " damage.");
	}
}

// 한쪽이 죽을 때까지 교환 전투 반복
void Combat::FightUntilEnd(Entity& first, Entity& second, Log& log)
{
	while (!first.IsDead() && !second.IsDead())
		PerformExchange(first, second, log);
}

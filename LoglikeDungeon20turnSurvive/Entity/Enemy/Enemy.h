#pragma once

#include <set>
#include <vector>
#include "Entity/Enemy/Monster.h"
#include "Type/Position.h"

class Map;
class Player;
class Log;

// 몬스터 목록·이동·전투·위치 동기화
class Enemy
{
private:
	std::vector<Monster> _monsters;
	std::set<Position> _monsterPositions;

public:
	Enemy() = default;
	~Enemy() = default;

public:
	void Clear();

public:
	void AddMonster(Monster monster);

public:
	const std::vector<Monster>& GetMonsters() const { return _monsters; }

public:
	bool HasMonsterAt(Position pos) const;

public:
	const Monster* FindMonsterAt(Position pos) const;

public:
	Monster* FindMonsterAt(int x, int y);

public:
	int FindIndexAt(Position pos) const;

public:
	bool IsOccupied(Position pos, int skipIndex = -1) const;

public:
	void SyncPositions();

public:
	void Update(Map& map, Player& player, Log& log);

public:
	bool RemoveDead(bool& bossKilled);

public:
	void OnKilled(int monsterIndex, Log& log, bool& bossKilled);

public:
	void FightWithPlayer(int monsterIndex, Player& player, Log& log);

private:
	void TryMoveMonster(Monster& monster, Map& map, const Player& player);

private:
	void CombatWithMonster(int monsterIndex, Player& player, Log& log);

};

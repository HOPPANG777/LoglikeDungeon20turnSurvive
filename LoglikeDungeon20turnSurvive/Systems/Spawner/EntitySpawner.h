#pragma once

class Map;
class Player;
class Enemy;
class Item;
class Log;

// 몬스터, 아이템, 보스 스폰
class EntitySpawner
{
public:
	EntitySpawner() = default;
	~EntitySpawner() = default;

public:
	void SpawnMonsters(Map& map, const Player& player, Enemy& enemies,
		const Item& items, int count, Log& log);

public:
	void SpawnItems(Map& map, const Player& player, Enemy& enemies, Item& items, int count);

public:
	bool TrySpawnBoss(Map& map, const Player& player, Enemy& enemies,
		const Item& items, Log& log);

};

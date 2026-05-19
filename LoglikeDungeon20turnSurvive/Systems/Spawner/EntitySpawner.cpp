#include "EntitySpawner.h"
#include "Systems/Map/Map.h"
#include "Entity/Player/Player.h"
#include "Entity/Enemy/Enemy.h"
#include "Entity/Enemy/Monster.h"
#include "Entity/Boss/Boss.h"
#include "Item/Item.h"
#include "Systems/Log/Log.h"
#include "Utilities/Random.h"
#include "Type/Tile.h"
#include "Type/Position.h"

using namespace std;

// 일반 몬스터 count마리 랜덤 배치
void EntitySpawner::SpawnMonsters(Map& map, const Player& player, Enemy& enemies,
	const Item& items, int count, Log& log)
{
	for (int i = 0; i < count; ++i)
	{
		for (int attempt = 0; attempt < 200; ++attempt)
		{
			int x = Random::Range(0, Map::width - 1);
			int y = Random::Range(0, Map::height - 1);
			Position pos{ x, y };

			if (map.GetTile(pos) == Tile::TILE_WALL)
				continue;

			if (player.GetPosition() == pos)
				continue;

			if (enemies.IsOccupied(pos))
				continue;

			if (items.HasAt(map, pos))
				continue;

			Monster monster("monster" + to_string(i), 8, 1, 1);
			monster.SetPosition(x, y);
			enemies.AddMonster(std::move(monster));

			log.Add("Monster spawn: " + to_string(x) + ", " + to_string(y));
			break;
		}
	}
}

// 포션 아이템 count개 배치
void EntitySpawner::SpawnItems(Map& map, const Player& player, Enemy& enemies,
	Item& items, int count)
{
	items.Spawn(map, player, count, [&enemies](Position pos)
		{
			return enemies.IsOccupied(pos);
		});
}

// 20턴 보스 1회 스폰 시도
bool EntitySpawner::TrySpawnBoss(Map& map, const Player& player, Enemy& enemies,
	const Item& items, Log& log)
{
	for (int attempt = 0; attempt < 200; ++attempt)
	{
		int x = Random::Range(0, Map::width - 1);
		int y = Random::Range(0, Map::height - 1);
		Position pos{ x, y };

		if (map.GetTile(pos) == Tile::TILE_WALL)
			continue;

		if (player.GetPosition() == pos)
			continue;

		if (enemies.IsOccupied(pos))
			continue;

		if (items.HasAt(map, pos))
			continue;

		Boss boss;
		boss.SetPosition(x, y);
		enemies.AddMonster(std::move(boss));

		log.Add("Boss spawned!");
		return true;
	}

	log.Add("Boss spawn failed (no empty tile).");
	return false;
}

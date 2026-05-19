#include "Item.h"
#include "Systems/Map/Map.h"
#include "Entity/Player/Player.h"
#include "Type/Tile.h"
#include "Utilities/Random.h"
#include <algorithm>
#include <functional>

using namespace std;

// 아이템 위치 목록 초기화
void Item::Clear()
{
	_positions.clear();
}

// 내부 목록에 해당 좌표 아이템이 있는지 확인
bool Item::HasAt(Position pos) const
{
	return find(_positions.begin(), _positions.end(), pos) != _positions.end();
}

// 맵 타일이 아이템인지 확인
bool Item::HasAt(const Map& map, Position pos) const
{
	return map.GetTile(pos) == Tile::TILE_ITEM;
}

// 맵에 포션 타일 count개 랜덤 배치
void Item::Spawn(Map& map, const Player& player, int count,
	const function<bool(Position)>& isBlocked)
{
	int placed = 0;
	int attempts = 0;

	while (placed < count && attempts < 500)
	{
		++attempts;

		int x = Random::Range(0, Map::width - 1);
		int y = Random::Range(0, Map::height - 1);
		Position pos{ x, y };

		if (map.GetTile(pos) != Tile::TILE_EMPTY)
			continue;

		if (player.GetPosition() == pos)
			continue;

		if (HasAt(pos))
			continue;

		if (isBlocked && isBlocked(pos))
			continue;

		_positions.push_back(pos);
		map.SetTile(pos, Tile::TILE_ITEM);
		++placed;
	}
}

// 플레이어 위치에서 아이템 줍기
bool Item::TryPickup(Map& map, Player& player, Position pos)
{
	if (!HasAt(map, pos))
		return false;

	if (!player.TryPickupItem(map))
		return false;

	RemoveAt(map, pos);
	return true;
}

// 맵·목록에서 아이템 제거
void Item::RemoveAt(Map& map, Position pos)
{
	map.SetTile(pos, Tile::TILE_EMPTY);
	_positions.erase(remove(_positions.begin(), _positions.end(), pos), _positions.end());
}

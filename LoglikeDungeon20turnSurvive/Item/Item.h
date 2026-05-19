#pragma once

#include <vector>
#include "Type/Position.h"

class Map;
class Player;

#include <functional>

// 맵에 배치되는 포션 아이템(I 타일) 관리
class Item
{
private:
	std::vector<Position> _positions;

public:
	Item() = default;
	~Item() = default;

public:
	void Clear();

public:
	bool HasAt(Position pos) const;

public:
	bool HasAt(const Map& map, Position pos) const;

public:
	void Spawn(Map& map, const Player& player, int count,
		const std::function<bool(Position)>& isBlocked = nullptr);

public:
	bool TryPickup(Map& map, Player& player, Position pos);

private:
	void RemoveAt(Map& map, Position pos);

};

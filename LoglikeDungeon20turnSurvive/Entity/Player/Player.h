#pragma once

#include <iostream>
#include <vector>
#include <string>
#include "Entity/Entity.h"
#include "Type/Position.h"

class Map;

enum class PlayerMoveResult
{
	BlockedOutside,
	BlockedWall,
	Moved
};

class Player : public Entity
{
private:
	Position _position{};
	int _potionCount = 0;

public:
	Player(std::string name, int health, int attack, int defense)
		: Entity(name, health, attack, defense)
	{
	}

	~Player() override = default;

public:
	void Update() override;
	void Render() override;

	void Dead() override;

public:
	const Position& GetPosition() const { return _position; }
	void SetPosition(int x, int y) { _position = { x, y }; }

public:
	int GetPotionCount() const { return _potionCount; }
	void AddPotion(int n = 1) { _potionCount += n; }

	bool TryUsePotion();

	PlayerMoveResult TryMove(const Map& map, int directionX, int directionY);

	bool TryPickupItem(Map& map);
};

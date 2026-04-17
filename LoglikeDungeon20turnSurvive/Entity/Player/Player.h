#pragma once

#include <iostream>
#include <vector>
#include <string>
#include "Entity/Entity.h"
#include "Type/Position.h"

class Player : public Entity
{
private:
	Position _position{};
	int _potionCount = 0; // 아이템은 "수량"으로만 처리한다.
	int _maxPotion = 0;
	int _potionUsed = 0;

public:
	Player(std::string name, int health, int attack, int defense, int potion)
		: Entity(name, health, attack, defense), _maxPotion(potion)
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
	int GetMaxPotion() const { return _maxPotion; }
	void AddPotion(int n = 1) { _potionCount += n; }

public:
	void TryMove(int directionX, int directionY);
	void TryUsedPotion(int n = 1);

};


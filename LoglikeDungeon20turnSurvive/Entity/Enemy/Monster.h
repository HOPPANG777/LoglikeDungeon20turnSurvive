#pragma once

#include <iostream>
#include <vector>
#include <string>
#include "Type/Position.h"
#include "Entity/Entity.h"

class Monster : public Entity
{
private:
	// 몬스터 Position
	Position _position{};

public:

	// 생성자.
	Monster(std::string name, int health, int attack, int defense)
		: Entity(name, health, attack, defense)
	{

	}

	// 소멸자.
	~Monster() override = default;

public:
	// Position Getter/Setter
	const Position& GetPosition() const { return _position; }
	void SetPosition(int x, int y) { _position = { x,y }; }

public:
	// 몬스터의 이동
	void Move(int directionX, int directionY);

};


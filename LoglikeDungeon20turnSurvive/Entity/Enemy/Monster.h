#pragma once

#include <iostream>
#include <vector>
#include <string>
#include "Type/Position.h"
#include "Entity/Entity.h"

struct ChaseStep
{
	int stepX = 0;
	int stepY = 0;
	bool shouldMove = false;
};

class Monster : public Entity
{
private:
	// 몬스터의 Position
	Position _position{};
	// 몬스터인지 보스인지 판단, 보스라면 B로 표시
	bool _isBoss = false;

public:
	// 기본 생성자
	Monster(std::string name, int health, int attack, int defense, bool isBoss = false)
		: Entity(name, health, attack, defense), _isBoss(isBoss)
	{

	}

	// 기본 소멸자
	~Monster() override = default;

public:
	// 위치 기본 Getter/Setter
	const Position& GetPosition() const { return _position; }
	void SetPosition(int x, int y) { _position = { x,y }; }

	// 보스 인지 확인하는 Getter
	bool IsBoss() const { return _isBoss; }

public:
	// 몬스터 길찾기 알고리즘
	ChaseStep CalcChaseStep(Position playerPos) const;

	// 몬스터 기본 움직임
	void ApplyMove(int stepX, int stepY);

};



#pragma once

#include <iostream>
#include <vector>
#include <string>

class Entity
{
protected:
	// 엔티티 이름
	std::string _name = {};

	// 죽었는지 판단
	bool _isDead = false;

	// 체력
	int _maxHealth = 0;
	int _currentHealth = 0;

	// 기본 스탯
	int _attack = 0;
	int _defense = 0;

public:
	// 기본 생성자
	Entity(const std::string& name);
	
	// 생성자 
	Entity(const std::string& name, int health, int attack, int defense);
	
public:
	// 복사 생성자
	Entity(const Entity& other)
		: Entity(other._name, other._maxHealth, other._attack, other._defense)
	{
		_currentHealth = other._currentHealth;
		_isDead = other._isDead;
	}

	Entity& operator=(const Entity& other)
	{
		if (this != &other)
		{
			_name = other._name;
			_maxHealth = other._maxHealth;
			_currentHealth = other._currentHealth;
			_attack = other._attack;
			_defense = other._defense;
			_isDead = other._isDead;
		}
		return *this;
	}

public:
	// 기본 소멸자
	virtual ~Entity() = default;

public:
	virtual void Update();
	virtual void Render(); 

public:
	// 데미지를 받는 함수
	void TakeDamage(int damage);

	// 공격 하는 함수
	void Attack(Entity& defender);

	// 상대방 공격으로 전환
	bool ExchangeCombat(Entity& other);

	// 사망 함수
	virtual void Dead();

public:
	bool IsDead() const { return _isDead; }
	int  GetHP() const { return _currentHealth; }
	int  GetMaxHP() const { return _maxHealth; }
	int  GetATK() const { return _attack; }
	int  GetDEF() const { return _defense; }
	const std::string& GetName() const { return _name; }

};

#pragma once

#include <iostream>
#include <vector>
#include <string>

class Entity
{
protected:
	// 엔티티 이름
	std::string _name = {};

	// 엔티티의 사망 확인
	bool _isDead = false;

	// 엔티티 체력
	int _health = 0;
	int _maxHealth = 0;
	int _currentHealth = 0;

	// 엔티티 공격력과 방어력
	int _attack = 0;
	int _defense = 0;

public:
	// 기본 생성자(이름만으로 생성)
	Entity(const std::string& name);
	
	// 이름 및 기본 스탯 정의 생성자.
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
	// 기본 소멸자 사용
	virtual ~Entity() = default;

public:
	virtual void Update();
	virtual void Render(); 

public:
	// 기본적으로 피해를 '받는'다 라고 설정
	void TakeDamage(int damage);

	// 엔티티 사망 함수
	virtual void Dead();

	//

public:
	bool IsDead() const { return _isDead; }
	int  GetHP() const { return _currentHealth; }
	int  GetMaxHP() const { return _maxHealth; }
	int  GetATK() const { return _attack; }
	int  GetDEF() const { return _defense; }
	const std::string& GetName() const { return _name; }

};


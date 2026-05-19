#include "Entity.h"

using namespace std;

Entity::Entity(const string& name)
	: _name(name)
{

}

Entity::Entity(const string& name, int health, int attack, int defense)
	: Entity(name)
{
	// 스탯 초기화
	_maxHealth = health;
	_currentHealth = _maxHealth;
	_attack = attack;
	_defense = defense;
}

void Entity::Update()
{

}

void Entity::Render()
{

}

void Entity::TakeDamage(int damage)
{
	// 임시 체력을 만들고
	int resultHealth = 0;

	int dealt = damage - _defense;

	if (dealt < 1) dealt = 1;

	// 엔티티의 체력에서 방어력을 적용한 데미지를 가한다.
	resultHealth = _currentHealth - dealt;

	// 체력이 음수로 적용될 시
	if (resultHealth <= 0)
	{
		// 우선 resultHealth를 0으로 적용하고.
		resultHealth = 0;

		// 엔티티를 사망한 상태로 만든다.
		Dead();
	}

	// 현재 체력에 적용
	_currentHealth = resultHealth;
}

void Entity::Attack(Entity& defender)
{
	defender.TakeDamage(_attack);
}

bool Entity::ExchangeCombat(Entity& other)
{
	Attack(other);

	if (!other.IsDead())
		other.Attack(*this);

	return IsDead() || other.IsDead();
}

// 엔티티가 사망할 경우 해당 함수를 불러온다.
void Entity::Dead()
{
	// _isDead를 true로
	_isDead = true;
}

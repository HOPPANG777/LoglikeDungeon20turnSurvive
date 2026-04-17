#include "Player.h"

using namespace std;

void Player::Update()
{
}

void Player::Render()
{
}

void Player::Dead()
{
	__super::Dead();

	_potionUsed = 0;
}

void Player::TryMove(int directionX, int directionY)
{
	Position current = _position;
	Position next = { current.x + directionX, current.y + directionY };

	/*
		IsWall, IsInsideMap 지금 움직이려고 하는 곳이 벽인지
		맵 안 인지 판단하는 값이 필요함. 
		그렇지 않는다면 플레이어가 맵 바깥으로 나가거나 
		맵을 통과하는 불상사가 일어날수있음.
	*/

	/*
		또한 플레이거 적과 
		싸우는 로직 또한 필요함.
	*/

	_position = next;
}

void Player::TryUsedPotion(int n)
{
	// 현재 체력이 최대 체력과 동일 할 경우
	if (_currentHealth == _maxHealth)
	{
		cout << "현재 체력이 최대 체력과 동일합니다." << '\n';
		return;
	}

	// 만약 포션 개수가 최대 개수보다 크거나 같다면
	if (_potionUsed >= _maxPotion)
	{
		// 이미 모든 포션을 사용하였기 때문에 
		cout << "이미 모든 포션을 사용하셨습니다." << '\n';

		// 함수를 탈출한다.
		return;
	}

	// 임시 체력을 만들어서 사용한 포션만큼 체력을 회복한다.
	int resultHealth = _currentHealth + (10 * n);

	// 만약 회복된 체력이 최대 체력을 넘었을경우
	if (resultHealth > _maxHealth)
	{
		// 임시 체력을 최대 체력으로 clamp한다.
		resultHealth = _maxHealth;
	}
	
	// 현재 체력에 임시 체력을 적용시킨다.
	_currentHealth = resultHealth;

	// 사용한 포션 만큼 포션개수를 증가시킨다.
	_potionUsed += n;
}

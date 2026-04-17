#include "Monster.h"

void Monster::Move(int directionX, int directionY)
{
	_position.x += directionX;
	_position.y += directionY;
}
